import QtQuick 2.0

Rectangle {
    id: root

    property int innerMovesCounter: outerMovesCounter

    Text {
        anchors.centerIn: parent
        text: innerMovesCounter
        color: "white"
        font.bold: true
        font.pointSize: parent.width / 14
    }

}
