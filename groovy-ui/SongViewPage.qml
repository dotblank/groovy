import Qt 4.7

Rectangle {
    id: albumInfo
    anchors.fill: parent
    color: "transparent"

    GrooveButton {
        width: parent.width
        text: "Queue"
        onClicked: {
            playerBackend.queueSong(searchPage.currentIndex);
        }
    }
}
