import Qt 4.7

Component {
    id: songDelegate

    Rectangle {
        ListView.onAdd: {
            console.log("Item added!")
        }

        ListView.onRemove: {
            console.log("Item removed!")
        }

        id: songListDelegateItem
        width: ListView.view.width
        height: 100
        color: "#000000"

        MouseArea {
            anchors.fill: parent
            onClicked: {
//                searchPage.selectedRow = index
                searchList.currentIndex = index
                viewSwitcher.switchView(songViewPage, false)
            }

            Image {
                source: coverArtUrl
                width: 90
                height: 90
                x: 10

//                                onStatusChanged: {
//                                    console.log(status)
//                                }
//
//                                onProgressChanged: {
//                                    console.log(progress)
//                                }
            }
            Text {
                x: 110
                color: "#ffffff"
                text: artist + " - " + title
            }
        }
    }
}
