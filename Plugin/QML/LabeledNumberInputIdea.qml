import QtQuick 2.12

FocusScope
{
    id: root
    height: textBG.height + numBG.height
    width: textBG.width > numBG.width ? textBG.width + 12 : numBG.width + 12;

    Rectangle
    {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.horizontalCenterOffset: 1
        id: textBG
        width: textEdit.width + 10
        height: textEdit.height + 4
        radius: 6

        color: "#1a1a1a"
        border.width: 2
        border.color: "#333333"
        TextEdit
        {
            id: textEdit
            anchors.horizontalCenter: parent.horizontalCenter
            font.bold: true
            font.pointSize: 20
            font.family: "verdana"
            verticalAlignment: TextEdit.AlignVCenter
            horizontalAlignment: TextEdit.AlignHCenter
            color: idea.outPlugLabelColors[0]
            selectByMouse: true
            text: idea.text
            focus: true;

            onEditingFinished:
            {
                idea.setText(textEdit.text)
            }

            onActiveFocusChanged:
            {
                if(textEdit.activeFocus && textEdit.text === "Label")
                {
                    textEdit.text = "";
                }
                if(!textEdit.activeFocus && textEdit.text === "")
                {
                    textEdit.text = "Label"
                }
            }

            KeyNavigation.tab: numInput;
            KeyNavigation.down: numInput;

            Connections
            {
                target: idea
                function onTextChanged()
                {
                    textEdit.text = idea.text;
                }
            }
            MouseArea
            {
                anchors.fill: parent
                cursorShape: Qt.IBeamCursor;
                acceptedButtons: Qt.NoButton
            }
        }
    }



    Rectangle
    {
        id: numBG
        width: numInput.width + 12
        height: numInput.height + 6
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
            id: numInput
            anchors.centerIn: parent
            anchors.verticalCenterOffset: -2
            font.bold: true
            font.family: "verdana"
            font.underline: true
            font.pointSize: 24
            verticalAlignment: TextInput.AlignVCenter
            horizontalAlignment: TextInput.AlignHCenter
            color: idea.outPlugLabelColors[0]
            selectByMouse: true
            validator: DoubleValidator{decimals: 10}
            text: idea.number

            onActiveFocusChanged:
            {
                if(numInput.activeFocus && numInput.text === "0")
                {
                    numInput.text = "";
                }
                if(!numInput.activeFocus && numInput.text === "")
                {
                    numInput.text = "0"
                    idea.setNumber(0);
                }
            }

            KeyNavigation.tab: textEdit;
            KeyNavigation.up: textEdit;

            onEditingFinished:
            {
                idea.setNumber(numInput.text)
            }
            Connections
            {
                target: idea
                function onNumberChanged()
                {
                    numInput.text = idea.number;
                }
            }
            MouseArea
            {
                anchors.fill: parent
                cursorShape: Qt.IBeamCursor;
                acceptedButtons: Qt.NoButton
            }
        }
    }


}


