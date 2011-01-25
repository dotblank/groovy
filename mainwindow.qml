import QtQuick 1.0

Rectangle {
    id: base
    width: 800
    height: 480

    Rectangle {
        id: searchPage
        width: 800
        height: 480
        color: "#000000"
        property int selectedRow: -1

        Rectangle {
            id: searchListContainer
            x: 0
            y: 65
            width: 800
            height: 420
            color: "#000000"

            ListView {
                id: searchList
                model: searchModel
                anchors.fill: parent

                delegate: Component {
                    id: songDelegate
                    Rectangle {
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
            }
        }

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
    }

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
                        searchPage.selectedRow = -1
                    }
                }
            }
        }
    }

    onStateChanged: {
        if (state == "search") {
            searchPage.visible = true
        } else if (state == "songview") {
            songViewPage.visible = true
        }

        // timer to hide the old page(s) when the transition is done
        hptimer.restart()
    }

    Timer {
        id: hptimer
        interval: 3000
        onTriggered: {
            if (state == "search") {
                songViewPage.visible = false
            } else if (state == "songview") {
                searchPage.visible = false
            }

            console.log("hidden inappropriate page")
        }
    }



    states: [
        State {
            name: "search"
            when: searchPage.selectedRow == -1
            PropertyChanges {
                target: searchPage
                x: 0
            }
            PropertyChanges {
                target: songViewPage
                x: songViewPage.width
            }
        },

        State {
            name: "songview"
            when: searchPage.selecwtedRow != -1
            PropertyChanges {
                target: searchPage
                x: -searchPage.width
            }
            PropertyChanges {
                target: songViewPage
                x: 0
            }
        }
    ]

    transitions: Transition {
        NumberAnimation { properties: "x,y"; easing.type: Easing.InOutQuad }
    }
}
