import QtQuick 2.12
import QtQuick.Controls 2.12

Rectangle
{
    id: root
    width: Math.max(title.width + 20, newNameText.width + 20);
    height: 128
    radius: 20;
    gradient: Gradient
    {
        GradientStop{position: 0.05; color: "#181818"}
        GradientStop{position: 0.95; color: "#5b5b5b"}
    }

    Rectangle
    {
        width: parent.width - 8
        height: parent.height - 8
        x: 4
        y: 4
        radius: 20

        gradient: Gradient
        {
            GradientStop{position: 0.05; color: "#5b5b5b"}
            GradientStop{position: 0.95; color: "#333333"}
        }


        Column
        {
            anchors.fill: parent
            anchors.margins: 10
            Text
            {
                id: title
                anchors.horizontalCenter: parent.horizontalCenter
                text: "Enter New Name"
                color: "#2fbde0"
                font.bold: true;
                font.pointSize: 16
                font.family: "verdana"
            }


            TextEdit
            {
                id: newNameText;
                anchors.horizontalCenter: parent.horizontalCenter
                color: "#4df64d"
                font.bold: true;
                font.pointSize: 16
                font.family: "verdana"
                text: "NewName";
                font.underline: true;
                selectByMouse: true;
                onEditingFinished:
                {
                    corkboard.changeNameFromCasingMenu(text);
                    root.visible = false;
                    text = "New"
                }
            }
        }
    }
}


