import QtQuick 2.0

Rectangle {
    id: root

    border.width: 2
    radius: 10

    Text{
        anchors.centerIn: parent
        text: "New Game"
        color: "white"
        font.bold: true
        font.pointSize: parent.width / 14
    }
}
