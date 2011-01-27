import Qt 4.7

Rectangle {
    id: searchPage
    color: "#000000"
    property alias currentIndex: searchList.currentIndex

    Rectangle {
        id: searchListContainer
        anchors.fill: parent
        color: "#000000"
        anchors.margins: 5

        ListView {
            id: searchList
            model: searchModel
            anchors.fill: parent

            delegate: GrooveSongListDelegate {

            }
        }
    }
}
