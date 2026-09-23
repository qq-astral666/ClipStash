#pragma once

#include "ClipItem.h"

#include <QObject>
#include <QTimer>

// Turns clipboard changes into ClipItems.
//
// On macOS there is no clipboard-changed notification, so we poll the cheap
// NSPasteboard.changeCount (every 350 ms) and only read the data when the
// counter moves. Elsewhere we rely on QClipboard::dataChanged.
class ClipboardWatcher : public QObject {
    Q_OBJECT

public:
    explicit ClipboardWatcher(QObject* parent = nullptr);

    void start();

    void setPaused(bool paused) { m_paused = paused; }
    bool isPaused() const { return m_paused; }

    void setIgnoreConcealed(bool ignore) { m_ignoreConcealed = ignore; }

    // Call right after the app itself wrote to the clipboard, so that write
    // is not captured as a new entry.
    void syncAfterOwnWrite();

signals:
    void captured(const ClipItem& item);

private:
    void poll();
    void onClipboardSignal();
    void capture();

    QTimer m_timer;
    qint64 m_lastChangeCount = -1;
    bool m_usePolling = false;
    bool m_ignoreNextSignal = false;
    bool m_paused = false;
    bool m_ignoreConcealed = true;
};
