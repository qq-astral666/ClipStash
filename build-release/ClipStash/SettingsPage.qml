import QtQuick
import QtQuick.Controls.Basic

Flickable {
    id: root

    property var controller: null

    contentHeight: column.implicitHeight + 36
    clip: true
    boundsBehavior: Flickable.StopAtBounds

    ScrollBar.vertical: ScrollBar {
        policy: ScrollBar.AsNeeded
        contentItem: Rectangle {
            implicitWidth: 5
            radius: 3
            color: Theme.textTertiary
        }
    }

    // The Accessibility permission is granted in System Settings, outside the
    // app — poll while this page is on screen so the status updates live.
    Timer {
        interval: 1500
        repeat: true
        running: root.visible && root.controller !== null
        onTriggered: root.controller.refreshAccessibility()
    }

    Column {
        id: column
        x: 28
        y: 8
        width: root.width - 56
        spacing: 0

        SectionTitle { text: "Основное" }

        SettingRow {
            width: column.width
            title: "Горячая клавиша"
            subtitle: "Открывает историю из любого приложения"
            Segmented {
                options: root.controller.hotkeyOptions
                current: root.controller.hotkeyPreset
                onSelected: (i) => root.controller.hotkeyPreset = i
            }
        }

        SettingRow {
            width: column.width
            visible: root.controller.isMac
            title: "Запускать при входе"
            subtitle: "ClipStash стартует вместе с macOS и сразу пишет историю"
            Toggle {
                checked: root.controller.launchAtLogin
                onToggled: (value) => root.controller.launchAtLogin = value
            }
        }

        SettingRow {
            width: column.width
            title: "Вставлять сразу"
            subtitle: "↵ вставляет запись в активное окно, ⌘↵ — только копирует"
            Toggle {
                checked: root.controller.pasteOnSelect
                onToggled: (value) => root.controller.pasteOnSelect = value
            }
        }

        SettingRow {
            width: column.width
            title: "Размер истории"
            subtitle: "Старые записи удаляются автоматически. Закреплённые — никогда"
            Segmented {
                readonly property var values: [100, 500, 1000, 5000]
                options: ["100", "500", "1000", "5000"]
                current: Math.max(0, values.indexOf(root.controller.maxItems))
                onSelected: (i) => root.controller.maxItems = values[i]
            }
        }

        SectionTitle { text: "Внешний вид" }

        SettingRow {
            width: column.width
            title: "Тема"
            Segmented {
                options: ["Система", "Тёмная", "Светлая"]
                current: root.controller.themeMode
                onSelected: (i) => root.controller.themeMode = i
            }
        }

        SettingRow {
            width: column.width
            visible: root.controller.isMac
            title: "Эффект стекла"
            subtitle: "Размытие рабочего стола под окном (NSVisualEffectView)"
            Toggle {
                checked: root.controller.vibrancy
                onToggled: (value) => root.controller.vibrancy = value
            }
        }

        SectionTitle { text: "Приватность" }

        SettingRow {
            width: column.width
            title: "Не сохранять пароли"
            subtitle: "Пропускать данные из 1Password, Bitwarden, Связки ключей и др."
            Toggle {
                checked: root.controller.ignoreConcealed
                onToggled: (value) => root.controller.ignoreConcealed = value
            }
        }

        SettingRow {
            width: column.width
            title: "Пауза записи"
            subtitle: "Временно не сохранять ничего нового"
            Toggle {
                checked: root.controller.paused
                onToggled: (value) => root.controller.paused = value
            }
        }

        SectionTitle {
            text: "Доступ"
            visible: root.controller.isMac
        }

        SettingRow {
            width: column.width
            visible: root.controller.isMac
            title: "Универсальный доступ"
            subtitle: root.controller.accessibilityTrusted
                      ? "Разрешён — автовставка работает"
                      : "Нужен, чтобы вставлять запись в активное окно (эмуляция ⌘V)"
            Row {
                spacing: 10
                Rectangle {
                    anchors.verticalCenter: parent.verticalCenter
                    width: 8
                    height: 8
                    radius: 4
                    color: root.controller.accessibilityTrusted ? Theme.success : Theme.warning
                }
                PillButton {
                    text: root.controller.accessibilityTrusted ? "Открыть" : "Разрешить"
                    onClicked: root.controller.requestAccessibility()
                }
            }
        }

        SectionTitle { text: "Данные" }

        SettingRow {
            width: column.width
            title: "Очистить историю"
            subtitle: "Удалит все записи, кроме закреплённых. Сейчас в базе: " + root.controller.totalCount
            PillButton {
                id: clearButton
                property bool armed: false
                danger: true
                icon: "trash"
                text: armed ? "Точно удалить?" : "Очистить"
                onClicked: {
                    if (armed) {
                        armed = false
                        root.controller.clearHistory()
                    } else {
                        armed = true
                        disarm.restart()
                    }
                }
                Timer {
                    id: disarm
                    interval: 3000
                    onTriggered: clearButton.armed = false
                }
            }
        }

        Text {
            width: column.width
            topPadding: 22
            horizontalAlignment: Text.AlignHCenter
            bottomPadding: 10
            text: "ClipStash " + Qt.application.version + " · C++20 · Qt 6 / QML"
            font.pixelSize: 11
            color: Theme.textTertiary
        }

        PillButton {
            anchors.horizontalCenter: parent.horizontalCenter
            text: "Выйти из ClipStash"
            onClicked: root.controller.quit()
        }
    }
}
