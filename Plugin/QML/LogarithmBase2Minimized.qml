import QtQuick

Rectangle
{
    height: 54
    width: Math.max(Math.max(input.width, output.width) + 10, 50);
    radius: 4

    property alias inputText: input.text;
    property alias outputText: output.text;

    color: "#1a1a1a"
    border.width: 2
    border.color: "#333333"

    Connections
    {
        target: idea
        function onDisplay(inputNumber, outputNumber)
        {
            inputText = "Log<sub>2</sub>(<font color=\"" + idea.inPlugLabelColors[0] + "\">" +
                    parseFloat(inputNumber) + "</font>" + ")";
            outputText = "= <font color=\"" + idea.outPlugLabelColors[0] + "\">"
                    + parseFloat(outputNumber.toFixed(4)) + "</font>";
        }
        function onClear()
        {
            inputText = "Log<sub>2</sub>()";
            outputText = "= ";
        }
    }

    Text
    {
        id: input;
        anchors.right: parent.right
        anchors.rightMargin: 5
        textFormat: Text.RichText

        font.bold: true
        font.family: "verdana"
        font.pointSize: 16
        color: "#a6b0cb"

        text: ""
    }
    Rectangle
    {
        color: "#a6b0cb"
        anchors.right: input.right
        anchors.left: input.left
        height: 1.0
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: 3
    }
    Text
    {
        id: output

        anchors.right: parent.right
        anchors.rightMargin: 5
        anchors.bottom: parent.bottom
        textFormat: Text.RichText

        font.bold: true
        font.family: "verdana"
        font.pointSize: 16
        color: "#a6b0cb"

        text: "= "
    }
}
