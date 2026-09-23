import QtQuick
import QtQuick.Layouts

// One row of the history list. Exposes all roles so PreviewPane can read
// them from ListView.currentItem.
Item {
    id: root

    required property int index
    required property var clipId
    required property int clipType
    required property string clipText
    required property string preview
    required property string sourceApp
    required property string sourceBundle
    required property var createdAt
    required property bool pinned
    required property int useCount
    required property int imageWidth
    required property int imageHeight
    required property int charCount
    required property int lineCount

    property bool selected: false

    signal clicked()
    signal doubleClicked()

    implicitHeight: 40

    Rectangle {
        anchors.fill: parent
        anchors.leftMargin: 8
        anchors.rightMargin: 8
        radius: 8
        color: root.selected ? Theme.selection : (rowMouse.containsMouse ? Theme.hover : "transparent")
        Behavior on color { ColorAnimation { duration: 80 } }
    }

    RowLayout {
        anchors.fill: parent
        anchors.leftMargin: 16
        anchors.rightMargin: 16
        spacing: 10

        TypeBadge {
            type: root.clipType
            clipId: root.clipId
            colorValue: root.clipType === 3 ? root.clipText : ""
            size: 24
        }

        Text {
            Layout.fillWidth: true
            text: root.preview
            elide: Text.ElideRight
            maximumLineCount: 1
            font.pixelSize: 13
            font.family: root.clipType === 3 ? Theme.monoFont : Qt.application.font.family
            color: Theme.text
        }

        Icon {
            visible: root.pinned
            name: "pin"
            size: 13
            color: Theme.accent
        }

        Text {
            text: Theme.shortTime(root.createdAt)
            font.pixelSize: 11
            color: Theme.textTertiary
        }
    }

    MouseArea {
        id: rowMouse
        anchors.fill: parent
        hoverEnabled: true
        onClicked: root.clicked()
        onDoubleClicked: root.doubleClicked()
    }
}
