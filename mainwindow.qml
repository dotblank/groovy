import QtQuick 1.0
import "groovy-ui"

Rectangle {
    id: root
    width: 800
    height: 480

    ViewSwitcher {
        id: viewSwitcher
        currentView: searchPage
     //   root: base
    }


    SearchPage {
        id: searchPage
        width: parent.width
        anchors.top: parent.top; anchors.bottom: parent.bottom
    }

    SongViewPage {
        id: songViewPage
        opacity: 0
        width: parent.width
        anchors.top: parent.top; anchors.bottom: parent.bottom
    }
}

