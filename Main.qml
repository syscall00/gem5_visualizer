import QtQuick
import QtQuick.Window
import QtQuick.Layouts
import QtQuick.Controls.Imagine 2.12
import gem5_visualizer;
import QtQml
import QtQuick.Dialogs

ApplicationWindow {
    id: root
    width: 1000
    height: 700
    visible: true
    title: qsTr("RISC-V Visualizer")

    Popup {
        parent: Overlay.overlay
        dim: true
        x: Math.round((parent.width - width) / 2)
        y: Math.round((parent.height - height) / 2)
        width: 70
        height: 64

        padding: 10
        visible: pipeline.loading
        contentItem:     BusyIndicator {
            running: true
        }
    }
    // Define the drop areas all over the window
    Rectangle {
        color: "transparent"
        anchors.fill: parent
        DropArea {
            anchors.fill: parent
            onEntered: (drag) => {
                           drag.accept (Qt.LinkAction);
                       }
            onDropped: (drop) => {
                           pipeline.setFilename(drop.urls[0]);
                       }
        }
    }

    FileDialog {
        visible: false
        id: fileDialog
        onAccepted: {
            pipeline.setFilename(selectedFile);
        }
    }

    menuBar: MenuBar {
        Menu {
            title: qsTr("&File")
            Action {
                text: qsTr("&Open...")
                onTriggered: {fileDialog.visible = true }
                shortcut: "Ctrl+O"
            }
            MenuSeparator { }
            Action {
                text: qsTr("&Quit");
                onTriggered: Qt.quit();
                shortcut: "Ctrl+W"
            }
        }
    }
    Connections {
        target: pipeline
        function onLoadingChanged() {
            if(!pipeline.loading) {
                lv.model = null;
                lv.model = pipeline.model;
            }
        }
    }

    Pipeline {
        id: pipeline

    }
    Text {
        visible: lv.model == null || lv.model.length === 0
        font.pointSize: 16
        anchors.centerIn: parent
        text: "No file loaded yet"

    }

    RowLayout {
        anchors.fill: parent
        ListView {
            id: lv;
            Layout.preferredWidth: 350
            contentWidth: 10000
            flickableDirection: Flickable.AutoFlickDirection
            model: null;
            delegate: Component {
                id: comp
                GridLayout {
                    columns: 3

                    Text {
                        id: del
                        text: model.address + ": " + model.assembly + " " + model.stage + " " + model.cycleNumber;
                        color: "black"
                        font.pointSize: 15
                        Layout.preferredWidth : 300

                    }
                    Rectangle {
                        color: "transparent"
                        width: 40*index
                        height: 10
                    }

                    Row {

                        Layout.fillWidth: true
                        Layout.fillHeight: true


                        Repeater {
                            x: 40*index
                            model: fruitModel

                            Rectangle {
                                id: f
                                width: 40
                                height: 15
                                border.color: "Black"
                                border.width: 1

                                color: colorr
                                Text {
                                    anchors.fill:parent
                                    text: name
                                    horizontalAlignment: Text.AlignHCenter
                                    verticalAlignment: Text.AlignVCenter
                                }
                            }
                        }
                    }
                }
            }
            Layout.fillHeight: true
        }
        ListModel {
            id: fruitModel

            ListElement {
                name: "IF"
                colorr: "yellow"
            }
            ListElement {
                name: "S"
                colorr: "yellow"
            }
            ListElement {
                name: "S"
                colorr: "yellow"
            }
            ListElement {
                name: "ID"
                colorr: "lightblue"
            }
            ListElement {
                name: "EX"
                colorr: "red"
            }
            ListElement {
                name: "Mem"
                colorr: "lightgreen"
            }
            ListElement {
                name: "WAR"
                colorr: "lightgreen"
            }
            ListElement {
                name: "WB"
                colorr: "magenta"
            }
        }

    }
}



