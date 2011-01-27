import Qt 4.7

QtObject {
    property Item currentView
    property Item previousView

    property int duration: 700
    property bool running: switchAnimation.running
    property bool direction

    function switchView(newView, leftToRight) {
        if (newView != currentView && !switchAnimation.running) {
            newView.x = leftToRight ? -root.width : root.width
            direction = leftToRight;
            previousView = currentView;
            currentView = newView;
            newView.opacity = 1;
            switchAnimation.start();
        }
    }

    property variant switchAnimation :
        ParallelAnimation {
            NumberAnimation { target: previousView; property: "x"; easing.type: Easing.InOutSine
                              to: direction ? root.width : -root.width;  }
            NumberAnimation { target: currentView; property: "x"; easing.type: Easing.InOutSine; to: 0 }

        onRunningChanged:  {
            if (!running) {
                previousView.opacity = 0;
                if (previousView.deactivationComplete != undefined) previousView.deactivationComplete();
                if (currentView.activationComplete != undefined) currentView.activationComplete();
            }
        }
    }
}
