import QtQuick

Column {
    id: root

    property bool searching: false
    property string hotkey: ""

    spacing: 10

    Rectangle {
        anchors.horizontalCenter: parent.horizontalCenter
        width: 60
        height: 60
        radius: 18
        color: Theme.accentSoft

        Icon {
            anchors.centerIn: parent
            name: root.searching ? "search" : "clipboard"
            size: 28
            color: Theme.accent
        }
    }

    Text {
        anchors.horizontalCenter: parent.horizontalCenter
        topPadding: 4
        text: root.searching ? "Ничего не найдено" : "История пока пуста"
        font.pixelSize: 15
        font.weight: Font.DemiBold
        color: Theme.text
    }

    Text {
        anchors.horizontalCenter: parent.horizontalCenter
        horizontalAlignment: Text.AlignHCenter
        text: root.searching
              ? "Попробуй другой запрос или фильтр"
              : "Скопируй что-нибудь — запись появится здесь.\nОткрывай историю из любого приложения по " + root.hotkey
        lineHeight: 1.3
        font.pixelSize: 12
        color: Theme.textSecondary
    }
}
