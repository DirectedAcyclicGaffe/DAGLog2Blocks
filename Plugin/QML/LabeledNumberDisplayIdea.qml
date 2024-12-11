import QtQuick 2.12

FocusScope
{
    id: root
    height: textBG.height + numBG.height
    width: textBG.width > numBG.width ? textBG.width + 12 : numBG.width + 12;

    Rectangle
    {
        id: textBG
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.horizontalCenterOffset: -1
        width: label.width + 10
        height: label.height + 4
        radius: 6
        color: "#1a1a1a"
        border.width: 2
        border.color: "#333333"

        TextEdit
        {
            id: label
            anchors.horizontalCenter: parent.horizontalCenter
            font.bold: true
            font.pointSize: 20
            font.family: "verdana"
            verticalAlignment: TextInput.AlignVCenter
            horizontalAlignment: TextInput.AlignHCenter
            color: idea.inPlugLabelColors[0]
            selectByMouse: true
            text: idea.text;
            focus: true

            onActiveFocusChanged:
            {
                if(label.activeFocus && label.text === "Label")
                {
                    label.text = "";
                }
                if(!label.activeFocus && label.text === "")
                {
                    label.text = "Label"
                }
            }
            onEditingFinished:
            {
                idea.setText(label.text)
            }

            KeyNavigation.tab: numDisplay;
            KeyNavigation.down: numDisplay;

            Connections
            {
                target: idea
                function onTextChanged()
                {
                    label.text = idea.text;
                }
            }
            MouseArea
            {
                anchors.fill: parent
                cursorShape: Qt.IBeamCursor;
                acceptedButtons: Qt.NoButton
            }
        }

        MouseArea
        {
            cursorShape: Qt.IBeamCursor;
            acceptedButtons: Qt.NoButton
        }
    }



    Rectangle
    {
        id: numBG
        width: numDisplay.width + 12
        height: numDisplay.height + 2
        anchors.bottom: parent.bottom
        anchors.bottomMargin: -4
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.horizontalCenterOffset: 1
        color: "#1a1a1a"
        border.width: 2
        border.color: "#333333"
        radius: 4
        TextInput
        {
            id: numDisplay
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.horizontalCenterOffset: -1
            font.bold: true
            font.pointSize: 24
            font.family: "verdana"
            verticalAlignment: TextInput.AlignVCenter
            horizontalAlignment: TextInput.AlignHCenter
            color: idea.inPlugLabelColors[0]
            selectByMouse: true
            readOnly: true
            visible: idea.valid

            text: "0.0"

            Connections
            {
                target: idea
                function onDisplayNumber(value)
                {
                    numDisplay.text = parseFloat(value);
                }
            }

            Keys.onPressed:
            {
                event.accepted = true;
                if(event.key === Qt.Key_C && event.modifiers & Qt.ControlModifier)
                {
                    idea.sendTextToClipboard(textInput.text);
                }
            }

            onActiveFocusChanged:
            {
                if(activeFocus)
                    numDisplay.selectAll();
                else
                    numDisplay.deselect();
            }
        }
    }
}
