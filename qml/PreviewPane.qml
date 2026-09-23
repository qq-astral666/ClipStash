import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts

// Right-hand side: full content of the selected clip + metadata.
Item {
    id: root

    property Item item: null          // ClipDelegate (ListView.currentItem)
    property var controller: null

    readonly property bool hasItem: item !== null
    readonly property int type: hasItem ? item.clipType : 0

    function sizeText() {
        if (!hasItem) return ""
        switch (type) {
        case 2:
            return item.imageWidth + " × " + item.imageHeight + " px"
        case 4: {
            const n = item.lineCount
            return n + " " + Theme.plural(n, "файл", "файла", "файлов")
        }
        default: {
            const c = item.charCount, l = item.lineCount
            return c + " " + Theme.plural(c, "символ", "символа", "символов")
                 + " · " + l + " " + Theme.plural(l, "строка", "строки", "строк")
        }
        }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 18
        spacing: 14
        visible: root.hasItem

        Loader {
            Layout.fillWidth: true
            Layout.fillHeight: true
            sourceComponent: [textView, linkView, imageView, colorView, fileView][root.type] ?? textView
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 1
            color: Theme.divider
        }

        ColumnLayout {
            Layout.fillWidth: true
            spacing: 6

            MetaRow { label: "Тип"; value: Theme.typeName(root.type) }
            MetaRow {
                visible: root.hasItem && root.item.sourceApp !== ""
                label: "Источник"
                value: root.hasItem ? root.item.sourceApp : ""
                iconSource: root.hasItem && root.item.sourceBundle !== ""
                            ? "image://clipstash/app/" + root.item.sourceBundle : ""
            }
            MetaRow { label: "Скопировано"; value: root.hasItem ? Theme.fullDate(root.item.createdAt) : "" }
            MetaRow { label: "Размер"; value: root.sizeText() }
            MetaRow {
                visible: root.hasItem && root.item.useCount > 0
                label: "Использовано"
                value: root.hasItem
                       ? root.item.useCount + " " + Theme.plural(root.item.useCount, "раз", "раза", "раз")
                       : ""
            }
        }
    }

    // ---------------------------------------------------------------- views

    Component {
        id: textView

        ScrollView {
            id: scroll
            clip: true
            contentWidth: availableWidth
            ScrollBar.horizontal.policy: ScrollBar.AlwaysOff

            Text {
                width: scroll.availableWidth
                // Rendering megabytes of text in one Text item would stall
                // the UI; the full content is still what gets pasted.
                text: {
                    const t = root.item ? root.item.clipText : ""
                    return t.length > 20000 ? t.slice(0, 20000) + "\n…" : t
                }
                textFormat: Text.PlainText
                wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                font.pixelSize: 13
                lineHeight: 1.25
                color: Theme.text
            }
        }
    }

    Component {
        id: linkView

        ColumnLayout {
            spacing: 12

            Item { Layout.fillHeight: true }

            Rectangle {
                Layout.alignment: Qt.AlignHCenter
                width: 56
                height: 56
                radius: 16
                color: Theme.typeTint(1)
                Icon { anchors.centerIn: parent; name: "link"; size: 26; color: Theme.typeColor(1) }
            }

            Text {
                Layout.fillWidth: true
                horizontalAlignment: Text.AlignHCenter
                text: root.item ? Theme.domainOf(root.item.clipText) : ""
                font.pixelSize: 20
                font.weight: Font.DemiBold
                elide: Text.ElideRight
                color: Theme.text
            }

            Text {
                Layout.fillWidth: true
                horizontalAlignment: Text.AlignHCenter
                text: root.item ? root.item.clipText : ""
                wrapMode: Text.WrapAnywhere
                maximumLineCount: 4
                elide: Text.ElideRight
                font.pixelSize: 12
                color: Theme.accentText
            }

            PillButton {
                Layout.alignment: Qt.AlignHCenter
                Layout.topMargin: 4
                icon: "external"
                text: "Открыть в браузере"
                hint: "⌘O"
                onClicked: root.controller.openUrl(root.item.clipText)
            }

            Item { Layout.fillHeight: true }
        }
    }

    Component {
        id: imageView

        Rectangle {
            radius: 10
            color: Theme.surface
            border.width: 1
            border.color: Theme.divider

            Image {
                id: preview
                anchors.fill: parent
                anchors.margins: 10
                source: root.item ? "image://clipstash/clip/" + root.item.clipId : ""
                sourceSize.width: Math.max(1, width * 2)
                sourceSize.height: Math.max(1, height * 2)
                fillMode: Image.PreserveAspectFit
                asynchronous: true
                smooth: true
                mipmap: true
                opacity: status === Image.Ready ? 1 : 0
                Behavior on opacity { NumberAnimation { duration: 140 } }
            }
        }
    }

    Component {
        id: colorView

        ColumnLayout {
            id: colorBox
            spacing: 14

            readonly property color value: root.item ? root.item.clipText : "transparent"

            Item { Layout.fillHeight: true }

            Rectangle {
                Layout.alignment: Qt.AlignHCenter
                width: 132
                height: 132
                radius: 28
                color: colorBox.value
                border.width: 1
                border.color: Theme.border
            }

            Text {
                Layout.alignment: Qt.AlignHCenter
                text: root.item ? root.item.clipText.toUpperCase() : ""
                font.family: Theme.monoFont
                font.pixelSize: 18
                font.weight: Font.DemiBold
                color: Theme.text
            }

            Text {
                Layout.alignment: Qt.AlignHCenter
                text: "rgb(" + Math.round(colorBox.value.r * 255) + ", "
                      + Math.round(colorBox.value.g * 255) + ", "
                      + Math.round(colorBox.value.b * 255) + ")"
                font.family: Theme.monoFont
                font.pixelSize: 12
                color: Theme.textSecondary
            }

            Item { Layout.fillHeight: true }
        }
    }

    Component {
        id: fileView

        ListView {
            clip: true
            spacing: 4
            model: root.item ? root.item.clipText.split("\n").filter(p => p.length > 0) : []

            delegate: RowLayout {
                id: fileRow
                required property string modelData
                width: ListView.view.width
                spacing: 10

                TypeBadge { type: 4; size: 28 }

                ColumnLayout {
                    Layout.fillWidth: true
                    spacing: 1

                    Text {
                        Layout.fillWidth: true
                        text: fileRow.modelData.split("/").pop()
                        elide: Text.ElideMiddle
                        font.pixelSize: 13
                        font.weight: Font.Medium
                        color: Theme.text
                    }
                    Text {
                        Layout.fillWidth: true
                        text: fileRow.modelData.substring(0, fileRow.modelData.lastIndexOf("/"))
                        elide: Text.ElideMiddle
                        font.pixelSize: 11
                        color: Theme.textTertiary
                    }
                }
            }
        }
    }
}
