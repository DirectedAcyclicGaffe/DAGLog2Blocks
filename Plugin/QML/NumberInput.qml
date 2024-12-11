import QtQuick 2.12
import QtQuick.Controls 2.12

FocusScope
{
    width: Math.max(16,textInput.implicitWidth + 22)
    height: textInput.implicitHeight + 16

    Rectangle
    {
        width: parent.width - 4
        height: parent.height
        x: 4

        radius: 6

        color: "#1a1a1a"
        border.width: 2
        border.color: "#333333"

        MouseArea
        {
            anchors.fill: parent
            cursorShape: Qt.IBeamCursor;
            acceptedButtons: Qt.NoButton
        }

        TextInput
        {
            id: textInput

            anchors.centerIn: parent
            anchors.verticalCenterOffset: -1
            font.bold: true
            font.underline: true
            font.pointSize: 24
            font.family: "verdana"
            verticalAlignment: TextInput.AlignVCenter
            horizontalAlignment: TextInput.AlignHCenter
            color: idea.outPlugLabelColors[0]
            selectByMouse: true
            validator: DoubleValidator{decimals: 10}
            text: idea.number;
            focus: true

            onEditingFinished:
            {
                idea.setNumber(text)
            }
            onActiveFocusChanged:
            {
                if(textInput.activeFocus && textInput.text === "0")
                {
                    textInput.text = "";
                }
                if(!textInput.activeFocus && textInput.text === "")
                {
                    textInput.text = "0"
                    idea.setNumber(0);
                }
            }

            Connections
            {
                target: idea
                function onNumberChanged()
                {
                    textInput.text = idea.number;
                }
            }
        }
    }
}



