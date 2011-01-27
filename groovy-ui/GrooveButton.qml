import Qt 4.7

Rectangle {
    id: button
    color: "#000000"
    height: 70
    property alias text: buttonText.text
    signal clicked()

    Text {
        id: buttonText
        anchors.fill: parent
        text: "->|"
        color: "#ffffff"
        font.pixelSize: 40
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            button.clicked()
        }
    }
}
