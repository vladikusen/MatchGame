import QtQuick 2.15
import QtQuick.Window 2.15

Rectangle {
    width: 1000
    height: 1000
    visible: true

    property int outerCounter: 0
    property int outerMovesCounter: 15
    color: "#b6aef4"

    NewGame {
        id: newGame

        width: parent.width / 4
        height: parent.height / 10

        color: "#752caf"

        anchors.horizontalCenter: parent.horizontalCenter

        MouseArea {
            anchors.fill: parent

            onClicked: {
                gameBoard.model.newGame()
                outerCounter = gameBoard.model.pointsCounter
                outerMovesCounter = gameBoard.model.movesCounter
            }
        }
    }


    PointsCounter {
        id: pointsCounter

        width: parent.width / 4
        height: parent.height / 10

        color: parent.color
    }

    MovesCounter {
        id: movesCounter

        anchors.right: parent.right

        width: parent.width / 4
        height: parent.height / 10

        color: parent.color
    }

    GameBoard {
        id: gameBoard

        anchors.bottom: parent.bottom

        width: parent.width
        height: parent.height - pointsCounter.height - 20
    }

    DefeatMessage {
        id: defeatMessage

        visible: false
        width: 300
        height: 100

        anchors.centerIn: parent
    }
}


