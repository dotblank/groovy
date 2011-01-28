import Qt 4.7

Rectangle {
    id: searchPage
    //color: "#000000"
    color: "transparent"
    property alias currentIndex: searchList.currentIndex


    ListView {
        id: searchList
        model: searchModel
        anchors.left: parent.left
        anchors.right:  parent.right
        anchors.bottom: parent.bottom
        anchors.top:  searchBar.bottom
        anchors.margins: 5

        delegate: GrooveSongListDelegate {

        }
    }

    GrooveSearchBar {
        id: searchBar
    }
}
