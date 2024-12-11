import QtQuick
import QtQuick.Window 2.12
import QtQuick.Controls
import QtQuick.Dialogs
import Qt.labs.settings 1.1
import QtQml.Models 2.12
import QtCore

import com.malamute.core 1.0

ApplicationWindow
{
    id: applicationWindow
    visible: true
    width: 1000
    height: 700 + palleteBar.height
    title: qsTr("Malamute")
    color: "black"

    onWidthChanged:{corkboard.setVisibleWindowParams(width,height - 107)}
    onHeightChanged:{corkboard.setVisibleWindowParams(width,height - 107)}


    header: PaletteBar{id: palleteBar}
    //Provides tool tips for the palleteBar. Because of how i did the geometry it's at the same level.
    PaletteToolTip{id: paletteToolTip}
    ToolBar{id: emptyHeader; height: 0;}

    Corkboard
    {
        id: corkboard;
        width: 4000;
        height: 4000;
        x: -1500;
        y: -1650;

        Rectangle
        {
            id: selectionRectangle
            border.color: CorkboardStyle.selectionRectangleColor()
            border.width: CorkboardStyle.selectionRectangleWidth()
            color: "#1a008080"
            visible: corkboard.selecting
            radius: CorkboardStyle.selectionRectangleRadius()
            z: 1
            Connections
            {
                target: corkboard;
                function onSelectingBoxChanged(box)
                {
                    selectionRectangle.x = box.x;
                    selectionRectangle.y = box.y;
                    selectionRectangle.width = box.width;
                    selectionRectangle.height = box.height;
                }
            }
        }

        Image
        {
            //The corkboard image itself.
            id: corkboardImage
            anchors.fill: parent
            source: CorkboardStyle.backgroundImagePath();
            fillMode: Image.Tile
            z: -9
        }
    }


    CorkboardHandler
    {
        id: malamuteHandler
        Component.onCompleted: {resetCorkboard(corkboard);}
    }

    Action
    {
        id: newAction
        text: "&New"
        icon.name: "document-new"
        shortcut: StandardKey.New
        onTriggered:
        {
            corkboard.deleteAll();
        }
    }

    Action
    {
        id: openAction
        text: "&Open"
        shortcut: StandardKey.Open
        icon.name: "document-open"
        onTriggered:
        {
            openFileDialog.open();
        }
    }

    Action
    {
        id: saveAction
        text: "&Save"
        shortcut: StandardKey.Save
        icon.name: "document-save"
        onTriggered:
        {
            if(malamuteHandler.validOpenFileUrl())
                malamuteHandler.save();
            else
                saveAsFileDialog.open();
        }
    }

    Action
    {
        id: saveAsAction
        text: "&SaveAs..."
        shortcut: StandardKey.SaveAs
        icon.name: "document-save"
        onTriggered:
        {
            saveAsFileDialog.open();
        }
    }

    Action
    {
        id: exitAction
        text: "E&xit"
        icon.name: "document-close"
        onTriggered:
        {
            Qt.quit();
        }
    }

    //End of actions for file menu, start actions edit menu
    Action
    {
        id: undoAction
        text: "Undo";
        icon.name: "edit-undo"
        onTriggered:
        {
            corkboard.undo();
        }
    }

    Action
    {
        id: redoAction
        text: "Redo";
        icon.name: "edit-redo"
        onTriggered:
        {
            corkboard.redo();
        }
    }

    Action
    {
        id: cutAction
        text: "Cut";
        icon.name: "edit-cut"
        onTriggered:
        {
            corkboard.cut();
        }
    }

    Action
    {
        id: copyAction
        text: "Copy";
        icon.name: "edit-copy"
        onTriggered:
        {
            corkboard.copy();
        }
    }

    Action
    {
        id: pasteAction
        text: "Paste";
        icon.name: "edit-paste"
        onTriggered:
        {
            corkboard.paste();
        }
    }

    Action
    {
        id: duplicateAction
        text: "Duplicate"
        icon.name: "edit-copy"
        onTriggered:
        {
            corkboard.duplicate();
        }
    }

    Action
    {
        id: deleteAction
        text: "Delete"
        icon.name: "edit-delete"
        onTriggered:
        {
            corkboard.deleteSelection();
        }
    }

    Action
    {
        id: deleteAll
        text: "Delete All"
        icon.name: "edit-delete"
        onTriggered:
        {
            corkboard.deleteAll();
        }
    }

    Action
    {
        id: selectAll
        text: "Select All"
        icon.name: "edit-selectall"
        onTriggered:
        {
            corkboard.selectAll();
        }
    }

    Action
    {
        id: deselectAll
        text: "Deselect All";
        icon.name: "edit-selectall"
        onTriggered:
        {
            corkboard.deselectAll();
        }
    }

    //View Actions.
    Action
    {
        id: defaultView
        text: "Default View"
        onTriggered:
        {
            corkboard.resetScale();
            corkboard.centerView();
        }
    }

    Action
    {
        id: centerView
        text: "Center View"
        onTriggered:
        {
            corkboard.centerView();
        }
    }

    Action
    {
        id: zoomIn
        text: "Zoom in"
        onTriggered:
        {
            corkboard.zoomIn();
        }
    }

    Action
    {
        id: zoomOut
        text: "Zoom Out"
        onTriggered:
        {
            corkboard.zoomOut();
        }
    }

    Action
    {
        id: frameLessWindow
        text: "Show Frame"

        shortcut: "Ctrl+P"
        property bool showFrame: true

        onTriggered:
        {
            if(showFrame)
            {
                applicationWindow.header = emptyHeader;
                corkboard.setHeaderHeight(0);
                showFrame = false;
                applicationWindow.flags = Qt.Window | Qt.FramelessWindowHint;
            }
            else
            {
                applicationWindow.header = palleteBar;
                corkboard.setHeaderHeight(48);
                showFrame = true;
                applicationWindow.flags = Qt.Window;
            }
        }
    }

    Action
    {
        id: casingMenuMaximize
        text: "Minimize/Maximize";
        onTriggered:
        {
            corkboard.minimizeFromCasingMenu();
        }
    }

    Action
    {
        id: casingMenuShowExtras
        text: "Extra Controls"
        onTriggered:
        {
            corkboard.showControlsFromCasingMenu();
        }
    }

    Action
    {
        id: casingMenuRename
        text: "Name"
        onTriggered:
        {
            renameDialog.visible = true;
        }
    }

    RenameDialog
    {
        id: renameDialog;
        visible: false;
    }

    Action
    {
        id: casingMenuDelete
        text: "Delete"
        onTriggered:
        {
            corkboard.deleteFromCasingMenu();
        }
    }


    FileDialog
    {
        id: saveAsFileDialog
        title: "Save File As"
        acceptLabel: "Save"
        nameFilters: ["Malamute files (*.mutt)"]
        currentFolder: StandardPaths.writableLocation(StandardPaths.DocumentsLocation)
        onAccepted:
        {
            malamuteHandler.saveAs(saveAsFileDialog.currentFile)
        }
    }
    FileDialog
    {
        id: openFileDialog
        title: "Load File"
        nameFilters: ["Malamute files (*.mutt)"]
        currentFolder: StandardPaths.writableLocation(StandardPaths.DocumentsLocation)
        onAccepted:
        {
            malamuteHandler.load(openFileDialog.currentFile);
        }
    }

    Menu
    {
        id: contextMenu

        MenuItem{action: undoAction}
        MenuItem{action: redoAction}
        MenuSeparator{}
        MenuItem{ action: cutAction}
        MenuItem{ action: copyAction}
        MenuItem{ action: pasteAction}
        MenuSeparator{}
        MenuItem{ action: duplicateAction}
        MenuItem{ action: deleteAction}

        Connections
        {
            target: corkboard
            function onContextMenuActiveChanged(active)
            {
                if(active)
                {
                    contextMenu.x = corkboard.contextMenuPt.x - 20;
                    contextMenu.y = corkboard.contextMenuPt.y - 180;
                    contextMenu.open();
                }
            }
        }
        onClosed:
        {
            corkboard.setContextMenuActive(false);
        }
    }

    Menu
    {
        id: casingMenu
        MenuItem{action: casingMenuMaximize}
        MenuItem{action: casingMenuShowExtras}
        MenuItem{action: casingMenuRename}

        MenuSeparator{}

        MenuItem{action: casingMenuDelete}
        Connections
        {
            target: corkboard
            function onCasingMenuActiveChanged(active)
            {
                if(active)
                {
                    casingMenu.x = corkboard.casingMenuPt.x - 20
                    casingMenu.y = corkboard.casingMenuPt.y - 180;
                    casingMenu.open();
                }
            }
        }
        onClosed:
        {
            corkboard.setCasingMenuActive(false);
        }
    }
}
