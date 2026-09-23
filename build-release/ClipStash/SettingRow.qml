import QtQuick

// Title + description on the left, any control on the right.
Item {
    id: root

    property string title: ""
    property string subtitle: ""
    default property alias control: slot.data

    implicitHeight: Math.max(48, texts.implicitHeight + 18)

    Column {
        id: texts
        anchors.left: parent.left
        anchors.right: slot.left
        anchors.rightMargin: 20
        anchors.verticalCenter: parent.verticalCenter
        spacing: 3

        Text {
            width: parent.width
            text: root.title
            elide: Text.ElideRight
            font.pixelSize: 13
            font.weight: Font.Medium
            color: Theme.text
        }

        Text {
            width: parent.width
            visible: text !== ""
            text: root.subtitle
            wrapMode: Text.WordWrap
            font.pixelSize: 11
            color: Theme.textSecondary
        }
    }

    Item {
        id: slot
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        width: childrenRect.width
        height: childrenRect.height
    }

    Rectangle {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        height: 1
        color: Theme.divider
    }
}
