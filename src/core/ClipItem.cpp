#include "ClipItem.h"

#include <QCryptographicHash>
#include <QRegularExpression>

namespace clip {

ClipType classifyText(const QString& text)
{
    static const QRegularExpression urlRe(
        QStringLiteral(R"(^(https?://|www\.)[^\s]+$)"),
        QRegularExpression::CaseInsensitiveOption);
    static const QRegularExpression hexColorRe(
        QStringLiteral(R"(^#(?:[0-9a-fA-F]{3}|[0-9a-fA-F]{6}|[0-9a-fA-F]{8})$)"));

    const QString t = text.trimmed();
    if (t.isEmpty() || t.contains(QLatin1Char('\n')))
        return ClipType::Text;
    if (hexColorRe.match(t).hasMatch())
        return ClipType::Color;
    if (urlRe.match(t).hasMatch())
        return ClipType::Link;
    return ClipType::Text;
}

static QByteArray sha1(const QByteArray& prefix, const QByteArray& payload)
{
    QCryptographicHash h(QCryptographicHash::Sha1);
    h.addData(prefix);
    h.addData(payload);
    return h.result();
}

QByteArray textHash(const QString& text) { return sha1("t:", text.toUtf8()); }
QByteArray fileHash(const QString& paths) { return sha1("f:", paths.toUtf8()); }
QByteArray imageHash(const QByteArray& png) { return sha1("i:", png); }

QString searchHaystack(const ClipItem& item)
{
    QString s;
    switch (item.type) {
    case ClipType::Image:
        s = QStringLiteral("изображение image %1x%2").arg(item.imageWidth).arg(item.imageHeight);
        break;
    default:
        s = item.text;
        break;
    }
    if (!item.sourceApp.isEmpty())
        s += QLatin1Char('\n') + item.sourceApp;
    return s.toLower();
}

} // namespace clip
