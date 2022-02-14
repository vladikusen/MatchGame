import QtQuick 2.0
import GameBoard 1.0

GridView {
    id: root
    interactive: false
    property int sourceIndex: -1
    property int bufferIndex: -1
    property int destinationIndex: -1

    property bool ifThereMatches: true

    model: GameBoardModel {
        id: boardModel
    }

    cellHeight: root.height / model.rows
    cellWidth: root.width / model.columns

    delegate: Item {
        id: tileWrapper

        width: cellWidth
        height: cellHeight

        Rectangle {
            id: tile

            radius: 10

            anchors.fill: parent
            anchors.margins: 5

            color: model.color

        }
    }


    function checkMoves(){
        if(boardModel.movesCounter === 0){
            defeatMessage.open()
        }
    }

    MouseArea {
        id: mouseArea

        anchors.fill: parent

        onClicked: {
            if(sourceIndex == -1){
                sourceIndex = root.indexAt(mouseX, mouseY)
            }
            else{
                if(sourceIndex !== root.indexAt(mouseX, mouseY)){
                    destinationIndex = root.indexAt(mouseX, mouseY)
                    bufferIndex = sourceIndex
                    boardModel.move(sourceIndex, destinationIndex)
                    if(!boardModel.checkIfThereIsMatches()){
                        ifThereMatches = false
                        boardModel.increaseMoveCounter()
                    }

                    sourceIndex = -1
                }
                outerCounter = boardModel.pointsCounter
                outerMovesCounter = boardModel.movesCounter

            }

            checkMoves()
        }
    }

    move: Transition {
       SequentialAnimation {
           PauseAnimation {
               duration: 150
           }
           ParallelAnimation {
               NumberAnimation { properties: "x,y"; duration: 500; easing.type: Easing.OutQuad }
           }
       }
       onRunningChanged: {
           if(running === false){
               if(!ifThereMatches){
                   boardModel.move(destinationIndex, bufferIndex)
                   ifThereMatches = true
               }

               boardModel.checkBoard()
               outerCounter = boardModel.pointsCounter
               outerMovesCounter = boardModel.movesCounter

               checkMoves()
           }
       }
    }

    add: Transition {
        NumberAnimation { property: "opacity"; from: 0; to: 1.0; duration: 300 }
        NumberAnimation { property: "scale"; from: 0; to: 1.0; duration: 300 }
        NumberAnimation {
        property: "y"; from: -40; duration: 600
        }
    }
}
