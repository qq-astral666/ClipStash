import QtQuick

// Leading visual of a history row: tinted type icon, colour swatch or
// image thumbnail.
Rectangle {
    id: root

    property int type: 0
    property var clipId: -1
    property string colorValue: ""
    property real size: 24

    width: size
    height: size
    radius: 6
    color: type === 3 ? colorValue : (type === 2 ? Theme.surface : Theme.typeTint(type))
    border.width: type === 3 || type === 2 ? 1 : 0
    border.color: Theme.border

    Icon {
        anchors.centerIn: parent
        visible: root.type !== 3 && !(root.type === 2 && thumb.status === Image.Ready)
        name: Theme.typeIcon(root.type)
        size: root.size * 0.62
        color: Theme.typeColor(root.type)
    }

    Image {
        id: thumb
        anchors.fill: parent
        anchors.margins: 1
        visible: root.type === 2
        source: root.type === 2 ? "image://clipstash/clip/" + root.clipId : ""
        sourceSize.width: root.size * 2
        sourceSize.height: root.size * 2
        fillMode: Image.PreserveAspectCrop
        asynchronous: true
        cache: true
    }
}
