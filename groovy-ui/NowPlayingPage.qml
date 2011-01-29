import Qt 4.7

Rectangle {
    id: searchPage
    color: "transparent"

    Row {
        id: controlButtons
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        height: 100

        GrooveButton {
            width: 100
            text: "Stop"
            onClicked: {
                playerBackend.stopSong();
            }
        }
/*
        GrooveButton {
            width: 100
            text: playerBackend.isPlaying ? "Pause" : "Play"
            onClicked: {
                playerBackend.pausePlaySong();
            }
        }

        GrooveButton {
            width: 100
            text: ">|"
            onClicked: {
                playerBackend.playNextSong();
            }
        }*/
    }

    PathView {
        id: coverArt
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: controlButtons.bottom
        //anchors.horizontalCenter: parent.horizontalCenter
        height: 230;


        model: playlistModel
        pathItemCount: 5

        path: Path {
            startX: 0; startY: coverArt.y
            PathCubic {
                x: coverArt.width
                y: coverArt.y

                control1X: 30
                control1Y: coverArt.y + 30

                control2X: coverArt.width - 30
                control2Y: coverArt.y + 30
            }
        }

        delegate: Component {
            Image {
                source: coverArtUrl
                width: 190
                height: 190

                MouseArea {
                    anchors.fill: parent

                    onClicked: {
                        if (index == playlistModel.currentTrackIndex())
                            playerBackend.pausePlaySong();
                    }

                    onDoubleClicked: {
                        playerBackend.skipTo(index - 1)
                    }
                }
            }
        }
    }
/*
    Image {
        id: coverArt

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: controlButtons.bottom
        height: 230;

        source: playerBackend.albumArt
        fillMode: Image.PreserveAspectFit
        smooth: true
    }
*/
    Rectangle {
        id: progressBar
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: coverArt.bottom
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
