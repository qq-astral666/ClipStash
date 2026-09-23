import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Effects
import QtQuick.Layouts
import ClipStash

// Spotlight/Raycast-style popup: search on top, history on the left,
// preview on the right, keyboard hints at the bottom.
Window {
    id: win

    required property AppController controller

    readonly property int shadowMargin: 40
    readonly property int cardWidth: 800
    readonly property int cardHeight: 500
    property bool settingsOpen: false
    // Only auto-hide after the popup really had focus; otherwise a refused
    // activation would close it the instant it appears.
    property bool hadFocus: false

    width: cardWidth + shadowMargin * 2
    height: cardHeight + shadowMargin * 2
    visible: false
    color: "transparent"
    title: "ClipStash"
    flags: Qt.FramelessWindowHint | Qt.Tool | Qt.WindowStaysOnTopHint | Qt.NoDropShadowWindowHint

    Binding { target: Theme; property: "mode"; value: win.controller.themeMode }
    Binding { target: Theme; property: "glass"; value: win.controller.isMac && win.controller.vibrancy }

    // ------------------------------------------------------------ lifecycle

    function present() {
        const g = controller.targetScreenGeometry()
        x = Math.round(g.x + (g.width - width) / 2)
        y = Math.round(g.y + Math.max(0, g.height * 0.16 - shadowMargin))

        settingsOpen = false
        search.text = ""
        controller.model.filter = 0
        list.currentIndex = 0
        list.positionViewAtBeginning()

        hadFocus = false
        visible = true
        controller.bringToFront(win)
        raise()
        requestActivate()
        search.forceActiveFocus()
        appear.restart()
        Qt.callLater(win.updateNativeWindow)
    }

    function dismiss() {
        visible = false
    }

    function updateNativeWindow() {
        if (!visible) return
        controller.prepareWindow(win, Qt.rect(card.x, card.y, card.width, card.height), card.radius, Theme.dark)
    }

    onVisibleChanged: controller.windowVisible = visible
    // Click outside -> the popup goes away, like Spotlight.
    onActiveChanged: {
        if (active)
            hadFocus = true
        else if (visible && hadFocus)
            dismiss()
    }
    onSettingsOpenChanged: settingsOpen ? card.forceActiveFocus() : search.forceActiveFocus()

    Connections {
        target: win.controller
        function onShowRequested() { win.present() }
        function onHideRequested() { win.dismiss() }
        function onSettingsRequested() { win.settingsOpen = true }
        function onToast(message) { toast.show(message) }
        function onSettingsChanged() { Qt.callLater(win.updateNativeWindow) }
    }

    Connections {
        target: Theme
        function onDarkChanged() { Qt.callLater(win.updateNativeWindow) }
    }

    // ------------------------------------------------------------- actions

    function currentId() {
        return list.currentItem ? list.currentItem.clipId : -1
    }

    function move(step) {
        if (list.count === 0) return
        list.currentIndex = Math.max(0, Math.min(list.count - 1, list.currentIndex + step))
        list.positionViewAtIndex(list.currentIndex, ListView.Contain)
    }

    function activateCurrent(paste) {
        const id = currentId()
        if (id >= 0) controller.activate(id, paste)
    }

    function togglePinCurrent() {
        const id = currentId()
        if (id < 0) return
        controller.togglePin(id)
        list.currentIndex = Math.max(0, controller.model.indexOfId(id))
        list.positionViewAtIndex(list.currentIndex, ListView.Contain)
    }

    function removeCurrent() {
        const id = currentId()
        if (id < 0) return
        const row = list.currentIndex
        controller.remove(id)
        list.currentIndex = Math.min(row, list.count - 1)
    }

    function openCurrent() {
        if (list.currentItem && list.currentItem.clipType === 1)
            controller.openUrl(list.currentItem.clipText)
    }

    function setFilter(index) {
        controller.model.filter = index
        list.currentIndex = 0
        list.positionViewAtBeginning()
    }

    // Single keyboard entry point. On macOS Qt maps ⌘ to ControlModifier.
    function handleKey(event) {
        const cmd = (event.modifiers & Qt.ControlModifier) !== 0

        if (settingsOpen) {
            if (event.key === Qt.Key_Escape || (cmd && event.key === Qt.Key_Comma)) {
                settingsOpen = false
                event.accepted = true
            }
            return
        }

        switch (event.key) {
        case Qt.Key_Down: move(1); break
        case Qt.Key_Up: move(-1); break
        case Qt.Key_PageDown: move(8); break
        case Qt.Key_PageUp: move(-8); break
        case Qt.Key_Return:
        case Qt.Key_Enter: activateCurrent(!cmd); break
        case Qt.Key_Tab: filterBar.next(1); break
        case Qt.Key_Backtab: filterBar.next(-1); break
        case Qt.Key_Escape:
            if (search.text.length > 0) search.text = ""
            else controller.hideWindow()
            break
        default:
            if (!cmd) return
            if (event.key === Qt.Key_P) togglePinCurrent()
            else if (event.key === Qt.Key_Backspace) removeCurrent()
            else if (event.key === Qt.Key_O) openCurrent()
            else if (event.key === Qt.Key_Comma) settingsOpen = true
            else if (event.key >= Qt.Key_1 && event.key <= Qt.Key_9) {
                const i = event.key - Qt.Key_1
                if (i >= list.count) return
                list.currentIndex = i
                activateCurrent(true)
            } else return
        }
        event.accepted = true
    }

    // ------------------------------------------------------------ visuals

    ParallelAnimation {
        id: appear
        NumberAnimation { target: card; property: "opacity"; from: 0; to: 1; duration: 130; easing.type: Easing.OutCubic }
        NumberAnimation { target: card; property: "scale"; from: Theme.glass ? 1 : 0.975; to: 1; duration: 170; easing.type: Easing.OutCubic }
    }

    // In glass mode the native window shadow is used (see Platform_mac.mm).
    RectangularShadow {
        anchors.fill: card
        visible: !Theme.glass
        opacity: card.opacity
        scale: card.scale
        radius: card.radius
        blur: 38
        offset: Qt.vector2d(0, 14)
        color: Qt.rgba(0, 0, 0, Theme.dark ? 0.55 : 0.22)
    }

    Rectangle {
        id: card

        x: win.shadowMargin
        y: win.shadowMargin
        width: win.cardWidth
        height: win.cardHeight
        radius: 14
        color: Theme.windowBg
        border.width: 1
        border.color: Theme.border

        Keys.onPressed: (event) => win.handleKey(event)

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 1
            spacing: 0

            // ------------------------------------------------ search / header
            Item {
                Layout.fillWidth: true
                Layout.preferredHeight: 56

                Icon {
                    id: leadingIcon
                    anchors.left: parent.left
                    anchors.leftMargin: 20
                    anchors.verticalCenter: parent.verticalCenter
                    name: win.settingsOpen ? "back" : "search"
                    size: 19
                    color: Theme.textSecondary

                    MouseArea {
                        anchors.fill: parent
                        anchors.margins: -6
                        enabled: win.settingsOpen
                        cursorShape: enabled ? Qt.PointingHandCursor : Qt.ArrowCursor
                        onClicked: win.settingsOpen = false
                    }
                }

                TextInput {
                    id: search
                    anchors.left: leadingIcon.right
                    anchors.leftMargin: 12
                    anchors.right: headerActions.left
                    anchors.rightMargin: 12
                    anchors.verticalCenter: parent.verticalCenter
                    visible: !win.settingsOpen
                    font.pixelSize: 18
                    color: Theme.text
                    selectionColor: Theme.accent
                    selectedTextColor: "white"
                    clip: true

                    Keys.onPressed: (event) => win.handleKey(event)
                    onTextChanged: {
                        win.controller.model.searchText = text
                        list.currentIndex = 0
                        list.positionViewAtBeginning()
                    }

                    Text {
                        anchors.fill: parent
                        verticalAlignment: Text.AlignVCenter
                        visible: search.text.length === 0 && search.preeditText.length === 0
                        text: "Поиск в истории буфера…"
                        font: search.font
                        color: Theme.textTertiary
                    }
                }

                Text {
                    anchors.left: leadingIcon.right
                    anchors.leftMargin: 12
                    anchors.verticalCenter: parent.verticalCenter
                    visible: win.settingsOpen
                    text: "Настройки"
                    font.pixelSize: 18
                    font.weight: Font.DemiBold
                    color: Theme.text
                }

                Row {
                    id: headerActions
                    anchors.right: parent.right
                    anchors.rightMargin: 16
                    anchors.verticalCenter: parent.verticalCenter
                    spacing: 8

                    Rectangle {
                        visible: win.controller.paused
                        anchors.verticalCenter: parent.verticalCenter
                        height: 22
                        width: pausedRow.implicitWidth + 16
                        radius: 11
                        color: Theme.dangerSoft

                        Row {
                            id: pausedRow
                            anchors.centerIn: parent
                            spacing: 5
                            Icon { name: "pause"; size: 11; color: Theme.danger; anchors.verticalCenter: parent.verticalCenter }
                            Text { text: "Пауза"; font.pixelSize: 11; font.weight: Font.DemiBold; color: Theme.danger }
                        }
                    }

                    Rectangle {
                        width: 30
                        height: 30
                        radius: 8
                        color: settingsMouse.containsMouse || win.settingsOpen ? Theme.hover : "transparent"

                        Icon {
                            anchors.centerIn: parent
                            name: "settings"
                            size: 17
                            color: win.settingsOpen ? Theme.accent : Theme.textSecondary
                        }

                        MouseArea {
                            id: settingsMouse
                            anchors.fill: parent
                            hoverEnabled: true
                            cursorShape: Qt.PointingHandCursor
                            onClicked: win.settingsOpen = !win.settingsOpen
                        }
                    }
                }
            }

            FilterBar {
                id: filterBar
                Layout.leftMargin: 14
                Layout.bottomMargin: 10
                visible: !win.settingsOpen
                current: win.controller.model.filter
                onSelected: (index) => win.setFilter(index)
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 1
                color: Theme.divider
            }

            // ------------------------------------------------------- body
            Item {
                Layout.fillWidth: true
                Layout.fillHeight: true

                RowLayout {
                    anchors.fill: parent
                    spacing: 0
                    visible: !win.settingsOpen && list.count > 0

                    ListView {
                        id: list
                        Layout.preferredWidth: 330
                        Layout.fillHeight: true
                        model: win.controller.model
                        clip: true
                        topMargin: 4
                        bottomMargin: 8
                        boundsBehavior: Flickable.StopAtBounds
                        highlightFollowsCurrentItem: false
                        reuseItems: true

                        section.property: "sectionName"
                        section.criteria: ViewSection.FullString
                        section.delegate: Item {
                            required property string section
                            width: list.width
                            height: 30
                            Text {
                                x: 18
                                anchors.bottom: parent.bottom
                                anchors.bottomMargin: 6
                                text: parent.section
                                font.pixelSize: 11
                                font.weight: Font.DemiBold
                                color: Theme.textTertiary
                            }
                        }

                        delegate: ClipDelegate {
                            width: list.width
                            selected: ListView.isCurrentItem
                            onClicked: list.currentIndex = index
                            onDoubleClicked: {
                                list.currentIndex = index
                                win.activateCurrent(true)
                            }
                        }

                        ScrollBar.vertical: ScrollBar {
                            id: listScroll
                            policy: ScrollBar.AsNeeded
                            contentItem: Rectangle {
                                implicitWidth: 5
                                radius: 3
                                color: Theme.textTertiary
                                opacity: listScroll.active ? 1 : 0
                                Behavior on opacity { NumberAnimation { duration: 200 } }
                            }
                        }
                    }

                    Rectangle {
                        Layout.preferredWidth: 1
                        Layout.fillHeight: true
                        color: Theme.divider
                    }

                    PreviewPane {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        item: list.currentItem
                        controller: win.controller
                    }
                }

                EmptyState {
                    anchors.centerIn: parent
                    visible: !win.settingsOpen && list.count === 0
                    searching: search.text.length > 0 || win.controller.model.filter !== 0
                    hotkey: win.controller.hotkeyText
                }

                SettingsPage {
                    anchors.fill: parent
                    visible: win.settingsOpen
                    controller: win.controller
                }
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 1
                color: Theme.divider
            }

            // ----------------------------------------------------- footer
            Item {
                Layout.fillWidth: true
                Layout.preferredHeight: 42

                Row {
                    anchors.left: parent.left
                    anchors.leftMargin: 16
                    anchors.verticalCenter: parent.verticalCenter
                    spacing: 8

                    Rectangle {
                        width: 20
                        height: 20
                        radius: 6
                        anchors.verticalCenter: parent.verticalCenter
                        gradient: Gradient {
                            orientation: Gradient.Vertical
                            GradientStop { position: 0; color: "#9a80ff" }
                            GradientStop { position: 1; color: "#6a45f5" }
                        }
                        Icon { anchors.centerIn: parent; name: "clipboard"; size: 13; color: "white"; strokeWidth: 2.2 }
                    }

                    Text {
                        anchors.verticalCenter: parent.verticalCenter
                        text: "ClipStash"
                        font.pixelSize: 12
                        font.weight: Font.DemiBold
                        color: Theme.text
                    }

                    Text {
                        anchors.verticalCenter: parent.verticalCenter
                        text: {
                            const n = win.controller.totalCount
                            return n + " " + Theme.plural(n, "запись", "записи", "записей")
                        }
                        font.pixelSize: 12
                        color: Theme.textTertiary
                    }
                }

                Row {
                    anchors.right: parent.right
                    anchors.rightMargin: 14
                    anchors.verticalCenter: parent.verticalCenter
                    spacing: 14

                    Repeater {
                        model: win.settingsOpen
                               ? [{ label: "Назад", key: "esc" }]
                               : [{ label: win.controller.pasteOnSelect ? "Вставить" : "Копировать", key: "↵" },
                                  { label: "Закрепить", key: "⌘P" },
                                  { label: "Удалить", key: "⌘⌫" },
                                  { label: "Настройки", key: "⌘," }]

                        delegate: Row {
                            id: hint
                            required property var modelData
                            spacing: 6

                            Text {
                                anchors.verticalCenter: parent.verticalCenter
                                text: hint.modelData.label
                                font.pixelSize: 12
                                color: Theme.textSecondary
                            }
                            KeyCap {
                                anchors.verticalCenter: parent.verticalCenter
                                text: hint.modelData.key
                            }
                        }
                    }
                }
            }
        }

        // -------------------------------------------------------------- toast
        Rectangle {
            id: toast

            function show(message) {
                toastText.text = message
                opacity = 1
                toastTimer.restart()
            }

            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 54
            width: toastText.implicitWidth + 28
            height: 32
            radius: 9
            color: Theme.toastBg
            opacity: 0
            visible: opacity > 0

            Behavior on opacity { NumberAnimation { duration: 160 } }

            Text {
                id: toastText
                anchors.centerIn: parent
                font.pixelSize: 12
                font.weight: Font.Medium
                color: "white"
            }

            Timer {
                id: toastTimer
                interval: 1600
                onTriggered: toast.opacity = 0
            }
        }
    }
}
