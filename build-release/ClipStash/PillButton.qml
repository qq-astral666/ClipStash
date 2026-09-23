import QtQuick

Rectangle {
    id: root

    property alias text: label.text
    property string hint: ""
    property string icon: ""
    property bool danger: false

    signal clicked()

    implicitWidth: content.implicitWidth + 22
    implicitHeight: 28
    radius: 7
    color: danger ? (mouse.containsMouse ? Theme.dangerHover : Theme.dangerSoft)
                  : (mouse.containsMouse ? Theme.selection : Theme.surface)
    border.width: 1
    border.color: danger ? "transparent" : Theme.border

    Behavior on color { ColorAnimation { duration: 100 } }

    Row {
        id: content
        anchors.centerIn: parent
        spacing: 7

        Icon {
            visible: root.icon !== ""
            anchors.verticalCenter: parent.verticalCenter
            name: root.icon
            size: 14
            color: label.color
        }

        Text {
            id: label
            anchors.verticalCenter: parent.verticalCenter
            font.pixelSize: 12
            font.weight: Font.Medium
            color: root.danger ? Theme.danger : Theme.text
        }

        KeyCap {
            visible: root.hint !== ""
            anchors.verticalCenter: parent.verticalCenter
            text: root.hint
        }
    }

    MouseArea {
        id: mouse
        anchors.fill: parent
        hoverEnabled: true
        cursorShape: Qt.PointingHandCursor
        onClicked: root.clicked()
    }
}
