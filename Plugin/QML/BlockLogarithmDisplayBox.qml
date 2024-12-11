import QtQuick 2.0

Rectangle
{
    id: root
    property int borderWidth: 0

    width: 20
    height: 20

    color: idea.inPlugLabelColors[0]
    border.color: Qt.darker(idea.inPlugLabelColors[0]);
    border.width: borderWidth;

    function setPartiallyFaded()
    {
        root.color = idea.outPlugLabelColors[0]
        root.width = root.width * 0.66;
        root.height = root.height * 0.66;
        root.x = root.x += root.width * 0.34;
    }
}
