import com.malamute.core 1.0
import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Effects

MCasing
{
    id: root
    width: casingSize.width + 1.4 * 15 + 10;
    height: casingSize.height + 0.4 * 15;

    Rectangle
    {
        id: frameRect
        x: 0.7 * 15;
        y: 0.2 * 15;
        z: 0
        width: casingSize.width;
        height: casingSize.height
        radius: 20;
        gradient: Gradient
        {
            GradientStop{position: 0.05; color: "#181818"}
            GradientStop{position: 0.95; color: "#5b5b5b"}
        }
        border.width: selected ? 4 : 0
        border.color: "#287f6c"

        Rectangle
        {
            id: mainSurface
            width: parent.width - 8
            height: parent.height - 8
            x: 4
            y: 4
            radius: parent.radius
            gradient: Gradient
            {
                GradientStop{position: 0.05; color: "#5b5b5b"}
                GradientStop{position: 0.95; color: "#333333"}
            }
        }

        //Name at the top.
        Rectangle
        {
            id: nameBackground
            anchors.top: frameRect.top;
            anchors.horizontalCenter: frameRect.horizontalCenter;
            width: frameRect.width
            height: nameSize.height + 10
            visible: ideaName !== "";

            radius: CasingStyle.frameRadius();

            gradient: Gradient
            {
                GradientStop{position: 0.05; color: "#4d4d4d"}
                GradientStop{position: 0.95; color: "#000000"}
            }
            border.width: selected ? 4 : 0
            border.color: "#287f6c"
            Rectangle
            {
                anchors.top: parent.top;
                anchors.topMargin: 3
                anchors.horizontalCenter: parent.horizontalCenter;
                width: parent.width - 6
                height: parent.height - 6
                visible: ideaName !== "";

                radius: CasingStyle.frameRadius();

                gradient: Gradient
                {
                    GradientStop{position: 0.05; color: "#272727"}
                    GradientStop{position: 0.95; color: "#4d4d4d"}
                }
                border.width: selected ? 4 : 0
                border.color: "#287f6c"
                Rectangle
                {
                    width: parent.width - 6
                    height: parent.height - 6
                    anchors.centerIn: parent

                    radius: CasingStyle.frameRadius();
                    color: "#1a1a1a"
                }
                Text
                {
                    id: name
                    anchors.centerIn: parent
                    anchors.verticalCenterOffset: -2
                    color: iColor
                    font: CasingStyle.nameFont()
                    text: ideaName

                }
            }
        }
        Column
        {
            id: inPlugsColumn
            y: inPlugBoxTop
            x: -0.5*15 + frameRect.border.width / 2;
            z: 99
            width: 15;

            spacing:  CasingStyle.plugSpacing();

            Repeater
            {
                model: inPlugLabels.length
                Rectangle
                {
                    radius: CasingStyle.plugSize();
                    width: CasingStyle.plugSize();
                    height: CasingStyle.plugSize();
                    border.color: "black"
                    border.width: CasingStyle.plugBorderWidth();
                    color: inPlugLabelColors[index]
                }
            }
        }
        Column
        {
            id: inPlugLabelsColumn
            x: inPlugsColumn.x + inPlugsColumn.width/2 - 2;
            y: inPlugsColumn.y - 2;
            z: 97
            width: inLabelWidth

            spacing: CasingStyle.plugSpacing() - 4;

            Repeater
            {
                model: inPlugLabels

                Rectangle
                {
                    visible: modelData !== "";
                    width: text.width + text.x + 3
                    height: CasingStyle.plugSize() + 4;
                    color: "#1a1a1a"
                    border.width: 2
                    border.color: "black"
                    radius: 6
                    z: -1
                    Text
                    {
                        id: text
                        x: inPlugsColumn.width/2 + 2
                        anchors.verticalCenter: parent.verticalCenter;
                        anchors.verticalCenterOffset: -1
                        font: CasingStyle.plugLabelsFont();
                        color: inPlugLabelColors[index]
                        text: modelData
                    }
                }
            }
        }
        Column
        {
            id: outPlugsColumn
            y: outPlugBoxTop
            x: frameRect.width - 0.5*15 - frameRect.border.width / 2;
            z: 99
            width: 15;
            spacing: CasingStyle.plugSpacing();

            Repeater
            {
                model: outPlugLabels.length
                Rectangle
                {
                    id: outPlugRect
                    radius: CasingStyle.plugSize();
                    width: CasingStyle.plugSize();
                    height: CasingStyle.plugSize();
                    border.color: "black"
                    border.width: CasingStyle.plugBorderWidth();
                    color: outPlugLabelColors[index]
                }
            }
        }
        Column
        {
            id: outPlugLabelsColumn
            y: outPlugBoxTop - 2
            x: outPlugsColumn.x - outLabelWidth + 8;
            width: outLabelWidth
            spacing: CasingStyle.plugSpacing() - 4;

            Repeater
            {
                model: outPlugLabels
                Rectangle
                {
                    visible: modelData !== "";
                    width: textout.width + 5 + CasingStyle.plugSize()/2
                    x: outLabelWidth - width
                    height: CasingStyle.plugSize() + 4;
                    color: "#1a1a1a"
                    border.width: 2
                    border.color: "black"
                    radius: 6
                    z: -1
                    Text
                    {
                        id: textout
                        x: 3
                        anchors.verticalCenter: parent.verticalCenter;
                        anchors.verticalCenterOffset: -1
                        font: CasingStyle.plugLabelsFont();
                        color: outPlugLabelColors[index]
                        text: modelData
                    }
                }
            }
        }

        //Feedback box.
        Rectangle
        {
            gradient: Gradient
            {
                GradientStop{position: 0.05; color: "#181818"}
                GradientStop{position: 0.95; color: "#5b5b5b"}
            }
            radius: frameRect.radius;

            y: feedbackBoxTop - 2
            anchors.left: frameRect.left;
            width: frameRect.width
            height: feedbackBoxSize.height === 0 ? 0 : feedbackBoxSize.height + 2*5 + 2*border.width + 4

            Rectangle
            {
                width: parent.width - 8
                height: parent.height - 8
                x: 4
                y: 4
                radius: parent.radius
                gradient: Gradient
                {
                    GradientStop{position: 0.05; color: "#5b5b5b"}
                    GradientStop{position: 0.95; color: "#333333"}
                }

                Rectangle
                {
                    width: parent.width - 8
                    height: parent.height - 8
                    anchors.centerIn: parent

                    radius: parent.radius
                    gradient: Gradient
                    {
                        GradientStop{position: 0.05; color: Qt.darker(feedbackColor, 1.15)}
                        GradientStop{position: 0.5; color: feedbackColor}
                        GradientStop{position: 0.95; color: Qt.darker(feedbackColor, 1.15)}
                    }
                    Text
                    {
                        id: validatationMessageDisplay
                        color: CasingStyle.feedbackTextColor();
                        font: CasingStyle.feedbackTextFont();
                        horizontalAlignment: Text.AlignHCenter
                        anchors.centerIn: parent
                        anchors.verticalCenterOffset: -1
                        text: feedbackMessage
                    }
                }
            }
        }
    }

    Rectangle
    {
        id: resizer;
        visible: resizeable
        x: frameRect.x + frameRect.width - width;
        y: frameRect.y + frameRect.height - height;
        z: frameRect.z + 1;
        width: CasingStyle.resizerSize()
        height: CasingStyle.resizerSize()
        radius: CasingStyle.resizerSize()
        border.width: 2;
        color: CasingStyle.resizerColor()
        border.color: CasingStyle.resizerBorderColor()
    }
}
