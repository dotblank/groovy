import Qt 4.7

Rectangle {
    id: searchBar
    width: parent.width
    height: 50
    color: "#000000"

    Row {
        spacing: 5

        Rectangle {
            id: searchEntryPrettyBorder
            border.color: "#ffffff"
            color: "#000000"
            anchors.margins: 5
            width: searchBar.width - (searchButton.width + 20)
            height: searchBar.height

            TextEdit {
                id: searchEntry
                anchors.fill: parent
                anchors.margins: 5
                color: "#ffffff"
                text: ""
            }
        }

        GrooveButton {
            id: searchButton
            width: 120
            text: "Search"

            onClicked: {
                searchModel.searchBySong(searchEntry.text)
            }
        }
    }
}
