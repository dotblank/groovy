import QtQuick 1.0

Rectangle {
    id: searchPage
    width: 800
    height: 480
    color: "#000000"

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
