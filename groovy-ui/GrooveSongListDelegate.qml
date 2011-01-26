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
                searchPage.selectedRow = index
                searchList.currentIndex = index
            }

            Image {
                source: coverArtUrl
                width: 90
                height: 90
                x: 5

//                                onStatusChanged: {
//                                    console.log(status)
//                                }
//
//                                onProgressChanged: {
//                                    console.log(progress)
//                                }
            }
            Text {
                x: 100
                color: "#ffffff"
                text: artist + " - " + title
            }
        }
    }
}
