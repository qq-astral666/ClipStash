import QtQuick
import QtQuick.Layouts

// "Label ........ value" line in the preview metadata block.
RowLayout {
    id: root

    property string label: ""
    property string value: ""
    property string iconSource: ""

    spacing: 8
    Layout.fillWidth: true

    Text {
        Layout.preferredWidth: 96
        text: root.label
        font.pixelSize: 12
        color: Theme.textTertiary
    }

    Image {
        visible: root.iconSource !== "" && status === Image.Ready
        source: root.iconSource
        sourceSize.width: 32
        sourceSize.height: 32
        Layout.preferredWidth: 14
        Layout.preferredHeight: 14
        asynchronous: true
    }

    Text {
        Layout.fillWidth: true
        text: root.value
        elide: Text.ElideRight
        font.pixelSize: 12
        color: Theme.textSecondary
    }
}
