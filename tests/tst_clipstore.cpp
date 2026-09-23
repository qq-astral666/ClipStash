#include "ClipItem.h"
#include "ClipStore.h"

#include <QSignalSpy>
#include <QTemporaryDir>
#include <QtTest>

#include <memory>

class TestClipStore : public QObject {
    Q_OBJECT

private:
    QTemporaryDir m_dir;
    std::unique_ptr<ClipStore> m_store;
    int m_dbCounter = 0;

    static ClipItem textItem(const QString& text, qint64 secondsAgo = 0)
    {
        ClipItem it;
        it.type = clip::classifyText(text);
        it.text = text;
        it.hash = clip::textHash(text);
        it.createdAt = QDateTime::currentDateTime().addSecs(-secondsAgo);
        return it;
    }

private slots:
    void init()
    {
        QVERIFY(m_dir.isValid());
        m_store = std::make_unique<ClipStore>(m_dir.filePath(QStringLiteral("db%1.sqlite").arg(++m_dbCounter)));
        QVERIFY2(m_store->open(), qPrintable(m_store->lastError()));
    }

    void cleanup() { m_store.reset(); }

    void classify_data()
    {
        QTest::addColumn<QString>("input");
        QTest::addColumn<int>("expected");
        QTest::newRow("plain") << QStringLiteral("hello world") << int(ClipType::Text);
        QTest::newRow("https") << QStringLiteral("https://example.com/a?b=1") << int(ClipType::Link);
        QTest::newRow("www") << QStringLiteral("www.example.com") << int(ClipType::Link);
        QTest::newRow("url with spaces") << QStringLiteral("see https://example.com") << int(ClipType::Text);
        QTest::newRow("hex6") << QStringLiteral("#7C5CFF") << int(ClipType::Color);
        QTest::newRow("hex3") << QStringLiteral("#fff") << int(ClipType::Color);
        QTest::newRow("not hex") << QStringLiteral("#zzzzzz") << int(ClipType::Text);
        QTest::newRow("multiline") << QStringLiteral("https://a.com\nhttps://b.com") << int(ClipType::Text);
    }

    void classify()
    {
        QFETCH(QString, input);
        QFETCH(int, expected);
        QCOMPARE(int(clip::classifyText(input)), expected);
    }

    void upsertDeduplicates()
    {
        const qint64 a = m_store->upsert(textItem(QStringLiteral("hello"), 60));
        m_store->upsert(textItem(QStringLiteral("other"), 30));
        const qint64 b = m_store->upsert(textItem(QStringLiteral("hello")));
        QCOMPARE(a, b);
        QCOMPARE(m_store->count(), 2);

        // Re-copying moves the entry to the top.
        const auto rows = m_store->query({}, ClipStore::Filter::All, 10);
        QCOMPARE(rows.first().text, QStringLiteral("hello"));
    }

    void upsertEmitsChanged()
    {
        QSignalSpy spy(m_store.get(), &ClipStore::changed);
        m_store->upsert(textItem(QStringLiteral("x")));
        QCOMPARE(spy.count(), 1);
    }

    void searchIsCaseInsensitiveForCyrillic()
    {
        m_store->upsert(textItem(QStringLiteral("Счёт за МАРТ №12")));
        m_store->upsert(textItem(QStringLiteral("unrelated")));
        QCOMPARE(int(m_store->query(QStringLiteral("март"), ClipStore::Filter::All, 10).size()), 1);
        QCOMPARE(int(m_store->query(QStringLiteral("счёт март"), ClipStore::Filter::All, 10).size()), 1);
        QCOMPARE(int(m_store->query(QStringLiteral("апрель"), ClipStore::Filter::All, 10).size()), 0);
    }

    void searchEscapesLikeWildcards()
    {
        m_store->upsert(textItem(QStringLiteral("100% done")));
        m_store->upsert(textItem(QStringLiteral("100 done")));
        QCOMPARE(int(m_store->query(QStringLiteral("100%"), ClipStore::Filter::All, 10).size()), 1);
    }

    void filterByType()
    {
        m_store->upsert(textItem(QStringLiteral("plain text")));
        m_store->upsert(textItem(QStringLiteral("https://qt.io")));
        m_store->upsert(textItem(QStringLiteral("#ff0000")));
        QCOMPARE(int(m_store->query({}, ClipStore::Filter::Links, 10).size()), 1);
        QCOMPARE(int(m_store->query({}, ClipStore::Filter::Text, 10).size()), 2);   // text + color
        QCOMPARE(int(m_store->query({}, ClipStore::Filter::Images, 10).size()), 0);
    }

    void pinnedComeFirst()
    {
        const qint64 old = m_store->upsert(textItem(QStringLiteral("old"), 3600));
        m_store->upsert(textItem(QStringLiteral("new")));
        QVERIFY(m_store->setPinned(old, true));

        const auto rows = m_store->query({}, ClipStore::Filter::All, 10);
        QCOMPARE(rows.first().id, old);
        QVERIFY(rows.first().pinned);
        QCOMPARE(int(m_store->query({}, ClipStore::Filter::Pinned, 10).size()), 1);
    }

    void trimKeepsPinnedAndNewest()
    {
        qint64 pinned = 0;
        for (int i = 0; i < 10; ++i) {
            const qint64 id = m_store->upsert(textItem(QStringLiteral("item %1").arg(i), 100 - i));
            if (i == 0)
                pinned = id;
        }
        m_store->setPinned(pinned, true);

        QCOMPARE(m_store->trim(3), 6);
        QCOMPARE(m_store->count(), 4);   // 3 newest + 1 pinned

        const auto rows = m_store->query({}, ClipStore::Filter::All, 10);
        QCOMPARE(rows.at(0).id, pinned);
        QCOMPARE(rows.at(1).text, QStringLiteral("item 9"));
    }

    void clearUnpinnedKeepsPinned()
    {
        const qint64 keep = m_store->upsert(textItem(QStringLiteral("keep")));
        m_store->upsert(textItem(QStringLiteral("drop")));
        m_store->setPinned(keep, true);
        QCOMPARE(m_store->clearUnpinned(), 1);
        QCOMPARE(m_store->count(), 1);
    }

    void markUsedBumpsCounterAndOrder()
    {
        const qint64 id = m_store->upsert(textItem(QStringLiteral("first"), 600));
        m_store->upsert(textItem(QStringLiteral("second"), 60));
        QVERIFY(m_store->markUsed(id));
        const auto item = m_store->get(id);
        QVERIFY(item.has_value());
        QCOMPARE(item->useCount, 1);
        QCOMPARE(m_store->query({}, ClipStore::Filter::All, 10).first().id, id);
    }

    void imageRoundTrip()
    {
        ClipItem it;
        it.type = ClipType::Image;
        it.imagePng = QByteArray("\x89PNG fake payload", 17);
        it.imageWidth = 10;
        it.imageHeight = 20;
        it.hash = clip::imageHash(it.imagePng);
        const qint64 id = m_store->upsert(it);
        QCOMPARE(m_store->imageData(id), it.imagePng);
        QCOMPARE(int(m_store->query(QStringLiteral("изображение"), ClipStore::Filter::Images, 10).size()), 1);
    }

    void appIcons()
    {
        QVERIFY(!m_store->hasAppIcon(QStringLiteral("com.apple.Safari")));
        m_store->saveAppIcon(QStringLiteral("com.apple.Safari"), QByteArray("png"));
        QVERIFY(m_store->hasAppIcon(QStringLiteral("com.apple.Safari")));
        QCOMPARE(m_store->appIcon(QStringLiteral("com.apple.Safari")), QByteArray("png"));
    }
};

QTEST_GUILESS_MAIN(TestClipStore)
#include "tst_clipstore.moc"
