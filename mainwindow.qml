import Qt 4.7
import "groovy-ui"

Rectangle {
    id: root
    width:800
    height:480

    Rectangle {
        width: parent.width
        height: parent.height - y
        clip: true

        Image {
            source:"groovy-ui/img/background.png"
            id:background
//            x:0 ; y:-1
//            width:800
//            height:480
        }

        PageStack {
            id: pageStack
        }

        //ViewSwitcher {
        //    id: viewSwitcher
        //    currentView: searchPage
        // //   root: base
        //}


        SearchPage {
            id: searchPage
            width: parent.width
            anchors.top: parent.top; anchors.bottom: parent.bottom
            visible: false

        }

        SongViewPage {
            id: songViewPage
            width: parent.width
            anchors.top: parent.top; anchors.bottom: parent.bottom
            visible: false
        }

        NowPlayingPage {
            id: nowPlayingPage
            width: parent.width
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            visible: false
        }
    }


    Image {
        x:0 ; y:430
        width:800
        height:50
        source:"groovy-ui/img/panel.png"
        opacity: 0.9

        MouseArea {
            anchors.fill: parent
            onClicked: {
                console.log("eating event on panel")
            }
        }
    }

    Image {
        source:"groovy-ui/img/windowswitcher.png"
        id:windowswitcher
        x:2 ; y:5
        width:808
        height:487
    }

    Image {
        source:"groovy-ui/img/backbutton.png"
        id:backbutton
        opacity: 0.56862745098
        x:27 ; y:0
        width:800
        height:480
    }

    Image {
        source:"groovy-ui/img/favourites.png"
        id:favourites
        x:143 ; y:434
        width:40
        height:40
    }

    Image {
        source:"groovy-ui/img/search.png"
        id:search
        x:232 ; y:435
        width:40
        height:40

        MouseArea {
            anchors.fill: parent
            onClicked: {
                pageStack.replace(searchPage)
            }
        }
    }

    Image {
        source:"groovy-ui/img/nowplaying.png"
        id:nowplaying
        x:315 ; y:435
        width:40
        height:40

        MouseArea {
            anchors.fill: parent
            onClicked: {
                pageStack.replace(nowPlayingPage)
            }
        }
    }

    Component.onCompleted: {
        pageStack.push(searchPage)
    }
}
