import QtQuick 2.12
import QtQuick.Controls
import QtMultimedia

import com.malamute.tileCalculator 1.0
import "BlockLogarithmHelperFunctions.js" as HelperFunctions

Item
{
    id: root
    width: 544
    height: 240

    readonly property real base : 2.0
    property real inputNumber: 0.0;
    property real outputNumber: 0.0;
    property real currentlyShownBoxes: 0.0;
    property real divisions: 0.0;

    function setupActionPicture(base){HelperFunctions.setupActionPicture(base);}
    Component.onCompleted:{setupActionPicture(base);}

    Connections
    {
        target: idea
        function onDisplay(input, output)
        {
            inputNumber = input;
            outputNumber = output;
            if(input > 5000 || input < 1)
            {
                outputText.text = outputNumber.toPrecision(4)

                animationUnsupportedText.visible = true;
                outputText.visible = false;
                inputText.visible = false;
                visualArea.visible = false;
            }
            else
            {
                currentlyShownBoxes = input;

                divisions = 0.0;
                outputText.text = "0";
                inputText.text = parseFloat(inputNumber.toPrecision(4));
                visualArea.setup(input, idea.inPlugLabelColors[0], idea.outPlugLabelColors[0])

                animationUnsupportedText.visible = false;
                outputText.visible = true;
                inputText.visible = true;
                visualArea.visible = true;
            }
        }
        function onClear()
        {
            animationUnsupportedText.visible = false;
            inputText.text = "";
            outputText.text = "";
        }

        function onOutPlugLabelColorsChanged()
        {
            HelperFunctions.clearActionPicture();
            HelperFunctions.setupActionPicture(base);
        }
    }

    Rectangle
    {
        id: inputArea
        anchors.left: parent.left
        height: parent.height
        width: 64

        radius: 4
        border.width: 2
        border.color: "#333333"

        gradient: Gradient
        {
            GradientStop{position: 0.0; color: "black"}
            GradientStop{position: 0.5; color: "#282828"}
            GradientStop{position: 1.0; color: "black"}
        }

        Rectangle
        {
            id: inputBox
            anchors.centerIn: parent
            width: 60
            height: 60
            color: idea.inPlugLabelColors[0]
            border.width: 6
            border.color: Qt.darker(idea.inPlugLabelColors[0]);
        }

        Text
        {
            anchors.top: inputBox.bottom
            anchors.topMargin: 4
            id: inputText
            text: "";
            color: idea.inPlugLabelColors[0]
            font.pointSize: 20
            font.family: "verdana"
            font.bold: true
            anchors.horizontalCenter: parent.horizontalCenter

            width: 60
            fontSizeMode: Text.HorizontalFit
            horizontalAlignment: Text.AlignHCenter
        }
    }

    Rectangle
    {
        id: outputArea
        anchors.right: parent.right
        height: parent.height
        width: 64

        radius: 4
        border.width: 2
        border.color: "#333333"

        gradient: Gradient
        {
            GradientStop{position: 0.0; color: "black"}
            GradientStop{position: 0.5; color: "#282828"}
            GradientStop{position: 1.0; color: "black"}
        }

        Image
        {
            id: actionPicture
            anchors.centerIn: parent
            fillMode: Image.PreserveAspectFit
            width: 60
            source: "qrc:/Images/DownArrows.png"

            Item
            {
                id: topRectanglesContainer
                width: 60
                height: 20
                y: 13
            }

            Item
            {
                id: midRectanglesContainer
                width: 60
                height: 20
                y: 53
            }

            Item
            {
                id: botRectanglesContainer
                width: 60
                height: 20
                y: 93
            }
        }

        Text
        {
            id: outputText
            property string nextText: ""

            anchors.top: actionPicture.bottom
            anchors.topMargin: 4
            anchors.horizontalCenter: parent.horizontalCenter
            y: 190
            color: idea.outPlugLabelColors[0]
            font.pointSize: 20
            font.family: "verdana"
            font.bold: true
            text: parseFloat(divisions.toPrecision(4));

            width: 60
            fontSizeMode: Text.HorizontalFit
            horizontalAlignment: Text.AlignHCenter

            Rectangle
            {
                id: highLightRect
                color: idea.outPlugLabelColors[0]
                anchors.fill: parent
                anchors.margins: -5
                radius: 10
                opacity: 0

                SequentialAnimation
                {
                    id: highLightAnim
                    PauseAnimation{duration: 300}

                    PropertyAnimation
                    {
                        target: highLightRect
                        property: "opacity"
                        to: 0.35
                        duration: 200
                    }
                    PropertyAction
                    {
                        target: outputText;
                        property: "text"
                        value: outputText.nextText;
                    }
                    PropertyAnimation
                    {
                        target: highLightRect
                        property: "opacity"
                        to: 0.0
                        duration: 200
                    }
                }
            }
        }
    }

    Rectangle
    {
        width: 408
        height: 208
        anchors.horizontalCenter: parent.horizontalCenter
        color: "#050512"
        radius: 4
        border.width: 2
        border.color: "#333333"
    }

    LogBase2VisualArea
    {
        id: visualArea
        opacity:  idea.valid ? 1.0 : 0.0
        y: 4
        width: 400
        anchors.horizontalCenter: parent.horizontalCenter
        height: 200
    }

    Text
    {
        id: animationUnsupportedText;
        text: "Animation Unsupported";
        horizontalAlignment: Text.AlignHCenter
        color: "white"
        font.bold: true;
        font.pointSize: 12
        anchors.centerIn: visualArea
        visible: false
    }

    Rectangle
    {
        id: controlsRow
        width: buttonsRow.width + 20 //Row is width: 286
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: -4
        height: 40

        radius: 8
        color: "#050512"
        border.width: 2
        border.color: "#333333"
        z: -1
        Row
        {
            id: buttonsRow
            x: 10
            spacing: 6
            visible: idea.valid

            Rectangle
            {
                id: divideButton
                height: 36
                width: divideText.width + 20
                anchors.verticalCenter: parent.verticalCenter;
                Text
                {
                    id: divideText
                    anchors.centerIn: parent
                    text: "Divide by 2"
                    font.bold: true
                    font.family: "verdana"
                    font.pointSize: 16
                }
                color: idea.outPlugLabelColors[0]
                radius: 20
                MouseArea
                {
                    anchors.fill: parent
                    enabled: idea.valid;
                    onClicked:
                    {
                        divideColorChangeAnimation.start();
                        if(currentlyShownBoxes > 1)
                        {
                            popSound.play();
                            visualArea.action();
                            currentlyShownBoxes /= base;
                            divisions += 1;
                            if(currentlyShownBoxes > 1)
                                outputText.nextText = parseFloat(divisions.toPrecision(4));
                            else
                                outputText.nextText = parseFloat(outputNumber.toPrecision(4));
                            highLightAnim.restart();

                        }
                    }
                }
                SequentialAnimation
                {
                    id: divideColorChangeAnimation
                    PropertyAnimation
                    {
                        duration: 100
                        target: divideButton
                        property: "opacity"
                        to: 0.7
                    }
                    PropertyAnimation
                    {
                        duration: 300
                        target: divideButton
                        property: "opacity"
                        to: 1.0
                    }
                }
            }
            Rectangle
            {
                id: resetButton
                height: 36
                width: resetText.width + 20
                anchors.verticalCenter: parent.verticalCenter
                radius: 20
                Text
                {
                    id: resetText
                    anchors.centerIn: parent
                    text: "Reset"
                    font.bold: true
                    font.family: "verdana"
                    font.pointSize: 16
                }
                color: "grey"

                MouseArea
                {
                    anchors.fill: parent
                    onClicked:
                    {
                        resetColorChangeAnimation.start();
                        visualArea.reset();
                        currentlyShownBoxes = inputNumber;
                        divisions = 0.0;
                        outputText.text = "0";
                    }
                }
                SequentialAnimation
                {
                    id: resetColorChangeAnimation
                    PropertyAnimation
                    {
                        duration: 100
                        target: resetButton
                        property: "opacity"
                        to: 0.7
                    }
                    PropertyAnimation
                    {
                        duration: 300
                        target: resetButton
                        property: "opacity"
                        to: 1.0
                    }
                }
            }
        }

        SoundEffect
        {
            id: popSound
            source: "qrc:/SoundEffects/shortPop.wav"
            volume: 0.3
        }
    }
}
