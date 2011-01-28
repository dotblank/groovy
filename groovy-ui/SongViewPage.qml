import Qt 4.7

Rectangle {
    id: songViewPage
    //color: "#000000"

    Rectangle {
        id: albumInfo

        anchors.left: controlButtons.right
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        color: "#000000"

        Image {
            id: coverArt

            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: progressBar.top

            source: playerBackend.albumArt
            fillMode: Image.PreserveAspectFit
            smooth: true
        }

        Rectangle {
            id: progressBar
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            height: 100
            color: "#000000"

            Rectangle {
                id: currentProgressTime
                property bool showTimeLeft: false

                anchors.left: parent.left
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                width: 130
                color: "#000000"
                Text {
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    text: parent.showTimeLeft ? playerBackend.currentTimeLeft : playerBackend.currentTime
                    color: "#ffffff"
                    font.pixelSize: 40
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: parent.showTimeLeft = !parent.showTimeLeft
                }

            }
            Rectangle {
                id: currentProgress
                anchors.left: currentProgressTime.right
                anchors.right: totalProgressTime.left
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.margins: 4
                color: "#000000"

                border.color: "#555555"
                border.width: 3
                radius: 15
                Rectangle {
                    anchors.left: parent.left
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    anchors.leftMargin: parent.border.width
                    anchors.topMargin: 20
                    anchors.bottomMargin: 20
                    color: "white"
                    width: playerBackend.currentTimeMS / playerBackend.totalTimeMS * (parent.width - parent.border.width * 2)
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        var newTime = playerBackend.totalTimeMS / width * mouseX;
                        playerBackend.seekTo(newTime);
                    }
                }
            }
            Rectangle {
                id: totalProgressTime
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                width: 130
                color: "#000000"
                Text {
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    text: playerBackend.totalTime
                    color: "#ffffff"
                    font.pixelSize: 40
                }
            }
        }

    }


    Column {
        id: controlButtons
        width: 150
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom

        GrooveButton {
            width: parent.width
            text:  "Go back"
            onClicked: {
                pageStack.pop();
            }
        }

        GrooveButton {
            width: parent.width
            text: "Queue"
            onClicked: {
                playerBackend.queueSong(searchPage.currentIndex);
            }
        }

        GrooveButton {
            width: parent.width
            text: "Stop"
            onClicked: {
                playerBackend.stopSong();
            }
        }

        GrooveButton {
            width: parent.width
            text: playerBackend.isPlaying ? "Pause" : "Play"
            onClicked: {
                playerBackend.pausePlaySong();
            }
        }

        GrooveButton {
            width: parent.width
            text: ">|"
            onClicked: {
                playerBackend.playNextSong();
            }
        }
    }
}
