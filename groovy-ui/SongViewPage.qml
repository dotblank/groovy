import QtQuick 1.0

Rectangle {
    id: songViewPage
    width: 800
    height: 480
    color: "#000000"
    x: songViewPage.width // offscreen

    Column {
        Rectangle {
            width: songViewPage.width
            height: songViewPage.height / 2
            color: "#000000"

            Text {
                anchors.fill: parent
                text: "Play"
                color: "#ffffff"
                font.pixelSize: 40
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    playerBackend.queueSong(searchList.currentIndex)
                }
            }
        }

        Rectangle {
            width: songViewPage.width
            height: songViewPage.height / 2
            color: "#000000"

            Text {
                anchors.fill: parent
                text: "Go back"
                color: "#ffffff"
                font.pixelSize: 40
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    viewSwitcher.switchView(searchPage, false)
                }
            }
        }
    }
}
