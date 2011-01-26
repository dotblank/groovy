import Qt 4.7

Rectangle {
    id: searchBar
    x: 0
    y: 0
    width: 800
    height: 60
    color: "#000000"


    Rectangle {
        id: searchEntryPrettyBorder
        border.color: "#ffffff"
        color: "#000000"
        x: 5
        y: 5
        width: 790
        height: 50

        TextEdit {
            id: searchEntry
            anchors.fill: parent
            anchors.margins: 5
            text: ""
            property string lastSearch
            color: "#ffffff"

            Keys.onReleased: {
                console.log("starting search timer")
                searchTimer.restart()
            }

            Timer {
                id: searchTimer
                interval: 1000
                onTriggered: {
                    console.log("doing search")
                    searchEntry.doSearch();
                }
            }

            function doSearch() {
                // don't allow duplicate searches
                if (searchEntry.lastSearch == searchEntry.text) {
                    console.log("Ignoring duplicate search")
                    return
                }

                searchModel.searchBySong(searchEntry.text)
                searchEntry.lastSearch = searchEntry.text
            }
        }
    }
}
