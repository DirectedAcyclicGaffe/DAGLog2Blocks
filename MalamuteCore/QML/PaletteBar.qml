import QtQuick 2.12
import QtQuick.Controls 2.12

ToolBar
{
    id: palleteBar
    height: 46
    width: 4 + (palleteListView.count * 44);
    background: Rectangle
    {
        anchors.fill: parent
        color: "#333333";
        border.color: "black"
        border.width: 2
    }

    ListView
    {
        id: palleteListView
        height: parent.height
        interactive: false
        clip: true
        anchors.left: parent.left
        anchors.leftMargin: 4
        anchors.top: parent.top
        anchors.topMargin: 4
        anchors.right: parent.right
        orientation: ListView.Horizontal
        model: malamuteHandler.ideaList
        spacing: 4
        delegate: PaletteItem{}
    }
}
