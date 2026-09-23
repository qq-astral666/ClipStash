#include "ClipStore.h"

#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <QStringList>
#include <QThread>
#include <QUuid>
#include <QVariant>

#include <utility>

namespace {

constexpr int kSchemaVersion = 1;

QString escapeLike(QString s)
{
    s.replace(QLatin1Char('\\'), QStringLiteral("\\\\"));
    s.replace(QLatin1Char('%'), QStringLiteral("\\%"));
    s.replace(QLatin1Char('_'), QStringLiteral("\\_"));
    return s;
}

bool execOrWarn(QSqlQuery& q, const char* what)
{
    if (q.exec())
        return true;
    qWarning().noquote() << "ClipStore:" << what << "failed:" << q.lastError().text();
    return false;
}

ClipItem rowToItem(const QSqlQuery& q)
{
    // Column order must match kListColumns below.
    ClipItem it;
    it.id = q.value(0).toLongLong();
    it.type = static_cast<ClipType>(q.value(1).toInt());
    it.text = q.value(2).toString();
    it.imageWidth = q.value(3).toInt();
    it.imageHeight = q.value(4).toInt();
    it.sourceApp = q.value(5).toString();
    it.sourceBundle = q.value(6).toString();
    it.createdAt = QDateTime::fromMSecsSinceEpoch(q.value(7).toLongLong());
    it.pinned = q.value(8).toBool();
    it.useCount = q.value(9).toInt();
    return it;
}

const char* const kListColumns =
    "id, type, text, image_w, image_h, source_app, source_bundle, created_at, pinned, use_count";

} // namespace

ClipStore::ClipStore(QString databasePath, QObject* parent)
    : QObject(parent)
    , m_path(std::move(databasePath))
    , m_connectionPrefix(QStringLiteral("clipstash-") + QUuid::createUuid().toString(QUuid::WithoutBraces))
{
}

ClipStore::~ClipStore()
{
    const QString name = connectionName();
    {
        QSqlDatabase d = QSqlDatabase::database(name, false);
        if (d.isOpen())
            d.close();
    }
    QSqlDatabase::removeDatabase(name);
}

QString ClipStore::connectionName() const
{
    const auto tid = reinterpret_cast<quintptr>(QThread::currentThread());
    return m_connectionPrefix + QLatin1Char('-') + QString::number(tid, 16);
}

QSqlDatabase ClipStore::db() const
{
    const QString name = connectionName();
    if (QSqlDatabase::contains(name))
        return QSqlDatabase::database(name);

    if (!QSqlDatabase::isDriverAvailable(QStringLiteral("QSQLITE")))
        qWarning() << "ClipStore: QSQLITE driver not found. Available:" << QSqlDatabase::drivers();

    QSqlDatabase d = QSqlDatabase::addDatabase(QStringLiteral("QSQLITE"), name);
    d.setDatabaseName(m_path);
    if (!d.open()) {
        m_lastError = QStringLiteral("cannot open %1: %2").arg(m_path, d.lastError().text());
        qWarning().noquote() << "ClipStore:" << m_lastError;
        return d;
    }
    {
        QSqlQuery q(d);
        q.exec(QStringLiteral("PRAGMA busy_timeout = 3000"));
        q.finish();
    }
    return d;
}

bool ClipStore::open()
{
    QSqlDatabase d = db();
    if (!d.isOpen()) {
        if (m_lastError.isEmpty())
            m_lastError = QStringLiteral("database not open (drivers: %1)").arg(QSqlDatabase::drivers().join(QLatin1Char(',')));
        return false;
    }

    {
        // PRAGMA journal_mode returns a row; the statement must be finished,
        // otherwise SQLite refuses DDL/COMMIT later ("statements in progress").
        QSqlQuery q(d);
        q.exec(QStringLiteral("PRAGMA journal_mode = WAL"));
        q.finish();
    }
    return migrate();
}

bool ClipStore::migrate()
{
    QSqlDatabase d = db();
    int version = 0;
    {
        QSqlQuery vq(d);
        if (vq.exec(QStringLiteral("PRAGMA user_version")) && vq.next())
            version = vq.value(0).toInt();
        vq.finish();
    }
    if (version >= kSchemaVersion)
        return true;

    QSqlQuery q(d);

    const QStringList ddl = {
        QStringLiteral(R"(
            CREATE TABLE IF NOT EXISTS clips (
                id            INTEGER PRIMARY KEY AUTOINCREMENT,
                type          INTEGER NOT NULL,
                text          TEXT,
                search_text   TEXT,
                image         BLOB,
                image_w       INTEGER NOT NULL DEFAULT 0,
                image_h       INTEGER NOT NULL DEFAULT 0,
                hash          BLOB    NOT NULL UNIQUE,
                source_app    TEXT,
                source_bundle TEXT,
                created_at    INTEGER NOT NULL,
                pinned        INTEGER NOT NULL DEFAULT 0,
                use_count     INTEGER NOT NULL DEFAULT 0
            ))"),
        QStringLiteral("CREATE INDEX IF NOT EXISTS idx_clips_order ON clips(pinned DESC, created_at DESC)"),
        QStringLiteral(R"(
            CREATE TABLE IF NOT EXISTS app_icons (
                bundle_id TEXT PRIMARY KEY,
                png       BLOB NOT NULL
            ))"),
        QStringLiteral("PRAGMA user_version = %1").arg(kSchemaVersion),
    };

    // Every statement is idempotent (IF NOT EXISTS), user_version goes last,
    // so a crash mid-migration is simply retried on the next start.
    for (const QString& stmt : ddl) {
        if (!q.exec(stmt)) {
            m_lastError = QStringLiteral("migration failed: %1 | SQL: %2").arg(q.lastError().text(), stmt.simplified());
            qWarning().noquote() << "ClipStore:" << m_lastError;
            return false;
        }
        q.finish();
    }
    return true;
}

qint64 ClipStore::upsert(const ClipItem& item)
{
    QSqlDatabase d = db();
    QSqlQuery q(d);
    q.prepare(QStringLiteral(R"(
        INSERT INTO clips (type, text, search_text, image, image_w, image_h, hash,
                           source_app, source_bundle, created_at)
        VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?)
        ON CONFLICT(hash) DO UPDATE SET
            created_at    = excluded.created_at,
            source_app    = excluded.source_app,
            source_bundle = excluded.source_bundle,
            search_text   = excluded.search_text)"));
    q.addBindValue(static_cast<int>(item.type));
    q.addBindValue(item.text);
    q.addBindValue(clip::searchHaystack(item));
    q.addBindValue(item.imagePng);
    q.addBindValue(item.imageWidth);
    q.addBindValue(item.imageHeight);
    q.addBindValue(item.hash);
    q.addBindValue(item.sourceApp);
    q.addBindValue(item.sourceBundle);
    const QDateTime ts = item.createdAt.isValid() ? item.createdAt : QDateTime::currentDateTime();
    q.addBindValue(ts.toMSecsSinceEpoch());
    if (!execOrWarn(q, "upsert"))
        return 0;

    QSqlQuery idq(d);
    idq.prepare(QStringLiteral("SELECT id FROM clips WHERE hash = ?"));
    idq.addBindValue(item.hash);
    qint64 id = 0;
    if (execOrWarn(idq, "upsert/id") && idq.next())
        id = idq.value(0).toLongLong();

    emit changed();
    return id;
}

QList<ClipItem> ClipStore::query(const QString& search, Filter filter, int limit) const
{
    QStringList where;
    QVariantList binds;

    switch (filter) {
    case Filter::All: break;
    case Filter::Text: where << QStringLiteral("type IN (0, 3)"); break;
    case Filter::Links: where << QStringLiteral("type = 1"); break;
    case Filter::Images: where << QStringLiteral("type = 2"); break;
    case Filter::Files: where << QStringLiteral("type = 4"); break;
    case Filter::Pinned: where << QStringLiteral("pinned = 1"); break;
    }

    // Every word must match (AND), order-independent: "счёт март" finds
    // "Март: счёт №12".
    const QStringList words = search.toLower().split(QLatin1Char(' '), Qt::SkipEmptyParts);
    for (const QString& w : words) {
        where << QStringLiteral("search_text LIKE ? ESCAPE '\\'");
        binds << QVariant(QStringLiteral("%%1%").arg(escapeLike(w)));
    }

    QString sql = QStringLiteral("SELECT %1 FROM clips").arg(QLatin1String(kListColumns));
    if (!where.isEmpty())
        sql += QStringLiteral(" WHERE ") + where.join(QStringLiteral(" AND "));
    sql += QStringLiteral(" ORDER BY pinned DESC, created_at DESC LIMIT ?");
    binds << limit;

    QSqlQuery q(db());
    q.setForwardOnly(true);
    q.prepare(sql);
    for (const QVariant& v : std::as_const(binds))
        q.addBindValue(v);

    QList<ClipItem> out;
    if (!execOrWarn(q, "query"))
        return out;
    while (q.next())
        out.push_back(rowToItem(q));
    return out;
}

std::optional<ClipItem> ClipStore::get(qint64 id) const
{
    QSqlQuery q(db());
    q.prepare(QStringLiteral("SELECT %1, image, hash FROM clips WHERE id = ?").arg(QLatin1String(kListColumns)));
    q.addBindValue(id);
    if (!execOrWarn(q, "get") || !q.next())
        return std::nullopt;
    ClipItem it = rowToItem(q);
    it.imagePng = q.value(10).toByteArray();
    it.hash = q.value(11).toByteArray();
    return it;
}

QByteArray ClipStore::imageData(qint64 id) const
{
    QSqlQuery q(db());
    q.prepare(QStringLiteral("SELECT image FROM clips WHERE id = ?"));
    q.addBindValue(id);
    if (!execOrWarn(q, "imageData") || !q.next())
        return {};
    return q.value(0).toByteArray();
}

bool ClipStore::setPinned(qint64 id, bool pinned)
{
    QSqlQuery q(db());
    q.prepare(QStringLiteral("UPDATE clips SET pinned = ? WHERE id = ?"));
    q.addBindValue(pinned ? 1 : 0);
    q.addBindValue(id);
    const bool ok = execOrWarn(q, "setPinned");
    if (ok)
        emit changed();
    return ok;
}

bool ClipStore::remove(qint64 id)
{
    QSqlQuery q(db());
    q.prepare(QStringLiteral("DELETE FROM clips WHERE id = ?"));
    q.addBindValue(id);
    const bool ok = execOrWarn(q, "remove");
    if (ok)
        emit changed();
    return ok;
}

bool ClipStore::markUsed(qint64 id)
{
    QSqlQuery q(db());
    q.prepare(QStringLiteral("UPDATE clips SET use_count = use_count + 1, created_at = ? WHERE id = ?"));
    q.addBindValue(QDateTime::currentMSecsSinceEpoch());
    q.addBindValue(id);
    const bool ok = execOrWarn(q, "markUsed");
    if (ok)
        emit changed();
    return ok;
}

int ClipStore::clearUnpinned()
{
    QSqlQuery q(db());
    q.prepare(QStringLiteral("DELETE FROM clips WHERE pinned = 0"));
    if (!execOrWarn(q, "clearUnpinned"))
        return 0;
    const int n = q.numRowsAffected();
    emit changed();
    return n;
}

int ClipStore::trim(int maxUnpinned)
{
    if (maxUnpinned <= 0)
        return 0;
    QSqlQuery q(db());
    q.prepare(QStringLiteral(R"(
        DELETE FROM clips
        WHERE pinned = 0 AND id NOT IN (
            SELECT id FROM clips WHERE pinned = 0 ORDER BY created_at DESC LIMIT ?
        ))"));
    q.addBindValue(maxUnpinned);
    if (!execOrWarn(q, "trim"))
        return 0;
    const int n = q.numRowsAffected();
    if (n > 0)
        emit changed();
    return n;
}

int ClipStore::count() const
{
    QSqlQuery q(db());
    q.prepare(QStringLiteral("SELECT COUNT(*) FROM clips"));
    if (!execOrWarn(q, "count") || !q.next())
        return 0;
    return q.value(0).toInt();
}

bool ClipStore::hasAppIcon(const QString& bundleId) const
{
    QSqlQuery q(db());
    q.prepare(QStringLiteral("SELECT 1 FROM app_icons WHERE bundle_id = ?"));
    q.addBindValue(bundleId);
    return execOrWarn(q, "hasAppIcon") && q.next();
}

void ClipStore::saveAppIcon(const QString& bundleId, const QByteArray& png)
{
    QSqlQuery q(db());
    q.prepare(QStringLiteral("INSERT OR REPLACE INTO app_icons (bundle_id, png) VALUES (?, ?)"));
    q.addBindValue(bundleId);
    q.addBindValue(png);
    execOrWarn(q, "saveAppIcon");
}

QByteArray ClipStore::appIcon(const QString& bundleId) const
{
    QSqlQuery q(db());
    q.prepare(QStringLiteral("SELECT png FROM app_icons WHERE bundle_id = ?"));
    q.addBindValue(bundleId);
    if (!execOrWarn(q, "appIcon") || !q.next())
        return {};
    return q.value(0).toByteArray();
}
