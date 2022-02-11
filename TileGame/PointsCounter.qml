import QtQuick 2.0


Rectangle {
    id: root
    property int innerCounter: outerCounter

    Text {
        anchors.centerIn: parent
        text: innerCounter
        color: "white"
        font.bold: true
        font.pointSize: parent.width / 14
    }
}
