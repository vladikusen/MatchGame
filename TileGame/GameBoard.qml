import QtQuick 2.0
import GameBoard 1.0

GridView {
    id: root
    interactive: false
    property int sourceIndex: -1
    property int destinationIndex: -1

    property bool wasMoved: false

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

            width: parent.width - 10
            height: parent.height - 10

            color: model.color


            MouseArea {
                id: mouseArea

                anchors.fill: parent

                onClicked: {
                    if(sourceIndex == -1){
                        sourceIndex = index
                        wasMoved = false
                    }
                    else{
                        if(sourceIndex !== index){
                            destinationIndex = index
                            if(!boardModel.checkIfThereIsMatches(sourceIndex, destinationIndex)){
                                tileWrapper.z = 1300
                                root.itemAtIndex(sourceIndex).z = 0

                                if(sourceIndex + 1 == destinationIndex){
                                   tileAnimationToLeft.start()
                                }
                                if(sourceIndex - 1 == destinationIndex){
                                    tileAnimationToRight.start()
                                }
                                if(sourceIndex + boardModel.columns == destinationIndex){
                                    tileAnimationToBottom.start()
                                }
                                if(sourceIndex - boardModel.columns == destinationIndex){
                                    tileAnimationToTop.start()
                                }
                            }
                            else{
                                wasMoved = true
                                boardModel.move(sourceIndex, destinationIndex)
                            }

                            sourceIndex = -1
                        }
                        outerCounter = boardModel.pointsCounter
                        outerMovesCounter = boardModel.movesCounter

                    }
                }
            }

            SequentialAnimation {
                id: tileAnimationToLeft

                NumberAnimation {
                    target: tile
                    property: "x"
                    to: tile.x - tile.width
                    duration: 400
                    easing.type: Easing.OutQuad
                }
                NumberAnimation {
                    target: tile
                    property: "x"
                    to: tile.x
                    duration: 400
                    easing.type: Easing.OutQuad
                }


            }

            SequentialAnimation {
                id: tileAnimationToTop

                NumberAnimation {
                    target: tile
                    property: "y"
                    to: tile.y + tile.height
                    duration: 400
                    easing.type: Easing.OutQuad
                }
                NumberAnimation {
                    target: tile
                    property: "y"
                    to: tile.y
                    duration: 400
                    easing.type: Easing.OutQuad
                }


            }

            SequentialAnimation {
                id: tileAnimationToBottom

                NumberAnimation {
                    target: tile
                    property: "y"
                    to: tile.y - tile.height
                    duration: 400
                    easing.type: Easing.OutQuad
                }
                NumberAnimation {
                    target: tile
                    property: "y"
                    to: tile.y
                    duration: 400
                    easing.type: Easing.OutQuad
                }

            }


            SequentialAnimation {
                id: tileAnimationToRight

                NumberAnimation {
                    target: tile
                    property: "x"
                    to: tile.x + tile.width
                    duration: 400
                    easing.type: Easing.OutQuad
                }
                NumberAnimation {
                    target: tile
                    property: "x"
                    to: tile.x
                    duration: 400
                    easing.type: Easing.OutQuad
                }

            }

        }
    }


    function checkMoves(){
        if(!boardModel.checkIfThereIsValidMoves()){
            defeatMessage.open()
        }
    }



    move: Transition {
       onRunningChanged: {
           if(running == false){
               if(wasMoved){
                   boardModel.checkBoard()
                   wasMoved = false
               }
           }
       }

       SequentialAnimation {
           PauseAnimation {
               duration: 350
           }
           ParallelAnimation {
               NumberAnimation { properties: "x,y"; duration: 700; easing.type: Easing.OutQuad }
           }
       }
    }

    add: Transition {

        SequentialAnimation {

            PauseAnimation {
                duration: 350
            }
            ParallelAnimation {
                NumberAnimation { property: "opacity"; from: 0; to: 1.0; duration: 150 }
                NumberAnimation { property: "scale"; from: 0; to: 1.0; duration: 150 }
                NumberAnimation {
                property: "y"; from: -40; duration:800
            }
        }

        }
        onRunningChanged: {
            if(running === false){
                checkMoves()

                boardModel.checkBoard()
                outerCounter = boardModel.pointsCounter
                outerMovesCounter = boardModel.movesCounter

            }
        }
    }
}
