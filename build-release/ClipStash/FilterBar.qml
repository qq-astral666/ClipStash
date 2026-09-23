import QtQuick

// Content-type filter chips. Tab / Shift+Tab cycle through them.
Row {
    id: root

    property int current: 0
    signal selected(int index)

    readonly property var labels: ["Все", "Текст", "Ссылки", "Картинки", "Файлы", "Закреплённые"]

    function next(step) {
        root.selected((root.current + step + labels.length) % labels.length)
    }

    spacing: 4

    Repeater {
        model: root.labels

        delegate: Rectangle {
            id: chip

            required property int index
            required property string modelData
            readonly property bool active: index === root.current

            width: chipLabel.implicitWidth + 20
            height: 26
            radius: 7
            color: active ? Theme.accentSoft : (chipMouse.containsMouse ? Theme.hover : "transparent")

            Behavior on color { ColorAnimation { duration: 110 } }

            Text {
                id: chipLabel
                anchors.centerIn: parent
                text: chip.modelData
                font.pixelSize: 12
                font.weight: chip.active ? Font.DemiBold : Font.Normal
                color: chip.active ? Theme.accentText : Theme.textSecondary
            }

            MouseArea {
                id: chipMouse
                anchors.fill: parent
                hoverEnabled: true
                cursorShape: Qt.PointingHandCursor
                onClicked: root.selected(chip.index)
            }
        }
    }
}
