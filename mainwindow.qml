import QtQuick 1.0
import "groovy-ui"

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

                delegate: GrooveSongListDelegate {

                }
            }
        }

        GrooveSearchBar {

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
