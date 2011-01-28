import Qt 4.7

Image {
    id: searchBar
    width: parent.width
    height: 50
    state: "inactive"

    source:"img/panel.png"

    MouseArea {
        anchors.fill: parent
        onClicked: {
            console.log("eating event on top panel")
        }
    }

    Rectangle {
        id: currentProgress
        anchors.margins: 4
        anchors.fill: parent
        color: "#ffffff"
        border.color: "#222222"
        border.width: 3
        radius: 10

        TextEdit {
            id: searchEntry
            anchors.fill: parent
            anchors.margins: 2
            anchors.leftMargin: 10
            color: "#000000"
            font.pixelSize: 35
            text: ""

            Keys.onReleased: {
                // set the state here, not in onSearchStarted, as the search triggers on a timer
                // this gives some visual feedback that we *WILL* search really soon now™
                searchBar.state = "searching"
                searchTimer.restart()
            }

            Timer {
                id: searchTimer
                interval: 500

                onTriggered: {
                    searchModel.searchBySong(searchEntry.text)
                }
            }
        }

        AnimatedImage {
            id: nowSearchingImage
            source: "img/loading.gif"
            opacity: 0
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.margins: 5
        }
    }


    Connections {
        target: searchModel

        onSearchCompleted: {
            searchBar.state = "inactive"
            // TODO: how do we access how many songs were retrieved in QML?
        }
    }

    states: [
        State {
            name: "inactive"
            PropertyChanges {
                target: nowSearchingImage
                opacity: 0
            }
        },

        State {
            name: "searching"
            PropertyChanges {
                target: nowSearchingImage
                opacity: 1
            }
        }
    ]

    transitions: [
        Transition {
            to: "searching"

            NumberAnimation {
                properties: "opacity"
                duration: 1000
            }
        },

        Transition {
            to: "inactive"

            NumberAnimation {
                properties: "opacity"
                duration: 300
            }
        }
    ]
}
