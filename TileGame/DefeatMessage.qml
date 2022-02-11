import QtQuick 2.0
import QtQuick.Controls 2.12

Popup {
    id: root
    padding: 0

    Rectangle {
        anchors.fill: parent
        color: "#752caf"
        border.width: 2
        Text {
         anchors.centerIn: parent
         text: "You're out of moves!"
         font.bold: true
         color: "white"
         font.pointSize: Math.min(root.width, root.height) / 7
        }
    }

    modal: true
    focus: true
    closePolicy: Popup.CloseOnEscape
}
