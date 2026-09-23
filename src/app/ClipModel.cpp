#include "ClipModel.h"

#include "ClipStore.h"

#include <QFileInfo>

namespace {

QString sectionFor(const ClipItem& it)
{
    if (it.pinned)
        return QStringLiteral("Закреплённые");
    const QDate day = it.createdAt.date();
    const QDate today = QDate::currentDate();
    if (day == today)
        return QStringLiteral("Сегодня");
    if (day == today.addDays(-1))
        return QStringLiteral("Вчера");
    if (day.daysTo(today) < 7)
        return QStringLiteral("На этой неделе");
    return QStringLiteral("Ранее");
}

QString previewFor(const ClipItem& it)
{
    switch (it.type) {
    case ClipType::Image:
        return QStringLiteral("Изображение %1×%2").arg(it.imageWidth).arg(it.imageHeight);
    case ClipType::File: {
        QStringList names;
        const QStringList paths = it.text.split(QLatin1Char('\n'), Qt::SkipEmptyParts);
        for (const QString& p : paths)
            names << QFileInfo(p).fileName();
        return names.join(QStringLiteral(", "));
    }
    default:
        // Collapse whitespace so multi-line snippets still read well in one row.
        return it.text.left(400).simplified().left(160);
    }
}

} // namespace

ClipModel::ClipModel(ClipStore* store, QObject* parent)
    : QAbstractListModel(parent)
    , m_store(store)
{
    connect(m_store, &ClipStore::changed, this, &ClipModel::reload);
    reload();
}

int ClipModel::rowCount(const QModelIndex& parent) const
{
    return parent.isValid() ? 0 : static_cast<int>(m_items.size());
}

QVariant ClipModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || index.row() < 0 || index.row() >= m_items.size())
        return {};
    const ClipItem& it = m_items.at(index.row());

    switch (role) {
    case ClipIdRole: return it.id;
    case ClipTypeRole: return static_cast<int>(it.type);
    case ClipTextRole: return it.text;
    case PreviewRole: return previewFor(it);
    case SourceAppRole: return it.sourceApp;
    case SourceBundleRole: return it.sourceBundle;
    case CreatedAtRole: return it.createdAt;
    case PinnedRole: return it.pinned;
    case UseCountRole: return it.useCount;
    case ImageWidthRole: return it.imageWidth;
    case ImageHeightRole: return it.imageHeight;
    case CharCountRole: return static_cast<int>(it.text.size());
    case LineCountRole: return it.text.isEmpty() ? 0 : static_cast<int>(it.text.count(QLatin1Char('\n')) + 1);
    case SectionNameRole: return sectionFor(it);
    default: return {};
    }
}

QHash<int, QByteArray> ClipModel::roleNames() const
{
    return {
        { ClipIdRole, "clipId" },
        { ClipTypeRole, "clipType" },
        { ClipTextRole, "clipText" },
        { PreviewRole, "preview" },
        { SourceAppRole, "sourceApp" },
        { SourceBundleRole, "sourceBundle" },
        { CreatedAtRole, "createdAt" },
        { PinnedRole, "pinned" },
        { UseCountRole, "useCount" },
        { ImageWidthRole, "imageWidth" },
        { ImageHeightRole, "imageHeight" },
        { CharCountRole, "charCount" },
        { LineCountRole, "lineCount" },
        { SectionNameRole, "sectionName" },
    };
}

void ClipModel::setSearchText(const QString& text)
{
    if (text == m_search)
        return;
    m_search = text;
    emit searchTextChanged();
    reload();
}

void ClipModel::setFilter(int filter)
{
    if (filter == m_filter)
        return;
    m_filter = filter;
    emit filterChanged();
    reload();
}

void ClipModel::reload()
{
    QList<ClipItem> fresh = m_store->query(m_search, static_cast<ClipStore::Filter>(m_filter), kLimit);
    const bool countChanges = fresh.size() != m_items.size();
    beginResetModel();
    m_items = std::move(fresh);
    endResetModel();
    if (countChanges)
        emit countChanged();
}

int ClipModel::indexOfId(qint64 id) const
{
    for (qsizetype i = 0; i < m_items.size(); ++i) {
        if (m_items.at(i).id == id)
            return static_cast<int>(i);
    }
    return -1;
}
