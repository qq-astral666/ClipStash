import QtQuick

// Segmented control (macOS-style).
Rectangle {
    id: root

    property var options: []
    property int current: 0
    signal selected(int index)

    implicitWidth: row.implicitWidth + 4
    implicitHeight: 28
    radius: 8
    color: Theme.surface
    border.width: 1
    border.color: Theme.divider

    Row {
        id: row
        anchors.centerIn: parent
        spacing: 2

        Repeater {
            model: root.options

            delegate: Rectangle {
                id: segment

                required property int index
                required property string modelData
                readonly property bool active: index === root.current

                width: segmentLabel.implicitWidth + 20
                height: 24
                radius: 6
                color: active ? Theme.segmentActive : (segmentMouse.containsMouse ? Theme.hover : "transparent")
                border.width: active && !Theme.dark ? 1 : 0
                border.color: Theme.divider

                Behavior on color { ColorAnimation { duration: 110 } }

                Text {
                    id: segmentLabel
                    anchors.centerIn: parent
                    text: segment.modelData
                    font.pixelSize: 12
                    font.weight: segment.active ? Font.DemiBold : Font.Normal
                    color: segment.active ? Theme.text : Theme.textSecondary
                }

                MouseArea {
                    id: segmentMouse
                    anchors.fill: parent
                    hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor
                    onClicked: root.selected(segment.index)
                }
            }
        }
    }
}
