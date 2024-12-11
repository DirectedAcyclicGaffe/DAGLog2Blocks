import QtQuick 2.12

FocusScope
{
    width: Math.max(12,textInput.implicitWidth + 18)
    height: textInput.implicitHeight + 8

    Rectangle
    {
        width: parent.width - 4
        height: parent.height
        radius: 6

        color: "#1a1a1a"
        border.width: 2
        border.color: "#333333"

        TextInput
        {
            id: textInput
            anchors.centerIn: parent
            font.bold: true
            font.pointSize: 24
            font.family: "verdana"
            verticalAlignment: TextInput.AlignVCenter
            horizontalAlignment: TextInput.AlignHCenter
            color: idea.inPlugLabelColors[0]
            focus: true;
            selectByMouse: true;

            readOnly: true
            text: ""

            Connections
            {
                target: idea
                function onDisplayNumber(value)
                {
                    textInput.text = parseFloat(value);
                }
                function onClearDisplay()
                {
                    textInput.text = "";
                }
            }

            Keys.onPressed:
            {
                event.accepted = true;
                if(event.key === Qt.Key_C && event.modifiers & Qt.ControlModifier)
                {
                    idea.sendTextToClipboard(textInput.text);
                }
                if(event.key === Qt.Key_X && event.modifiers & Qt.ControlModifier)
                {
                    idea.sendTextToClipboard(textInput.text);
                }
            }

            onActiveFocusChanged:
            {
                if(activeFocus)
                    textInput.selectAll();
                else
                    textInput.deselect();
            }
        }
    }
}


