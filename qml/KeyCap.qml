import QtQuick

// Small keyboard key label, e.g. [⌘P].
Rectangle {
    id: root

    property alias text: label.text

    implicitWidth: Math.max(20, label.implicitWidth + 10)
    implicitHeight: 20
    radius: 5
    color: Theme.keycap
    border.width: 1
    border.color: Theme.divider

    Text {
        id: label
        anchors.centerIn: parent
        font.pixelSize: 11
        font.weight: Font.Medium
        color: Theme.textSecondary
    }
}
