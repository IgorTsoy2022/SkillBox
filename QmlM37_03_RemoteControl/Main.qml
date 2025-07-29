import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Window {
    id: mainWindow
    visible: true
    width: 240
    height: 680
    title: qsTr("TV")

    maximumWidth: 240
    maximumHeight: 800
    minimumWidth: 240
    minimumHeight: 680



    ColumnLayout {
        id: columnLayout_0
        spacing: 0
        width: parent.width
        height: parent.height

        property bool switched_on : false
        property int channel : 0
        property int volume : 0

        RowLayout {
            id: rowLayout_0

            width: parent.width

            Layout.topMargin: 10
            Layout.leftMargin: 10

            RoundButton {
                id: roundButton_On
                text: String.fromCodePoint(0x000023FC)

                palette.button: "#FF0000"

                onClicked: {
                    columnLayout_0.switched_on = !columnLayout_0.switched_on;
                    console.log(columnLayout_0.switched_on);
                    rectangle_0.opacity = columnLayout_0.switched_on ? 1 : 0
                }
            }
        }

        RowLayout {
            id: rowLayout_1

            Layout.leftMargin: 10
            Layout.rightMargin: 10
            Layout.maximumHeight: 60

            Rectangle {
                id:rectangle_0

//                antialiasing: true
                border.color: "grey"
                border.width: 2

                Layout.fillWidth: true
                Layout.fillHeight: true
                opacity: 0.0

                Label {
                    id: labelChannel
                    text: ""
                }

/*
                ListView {
                    id: listView_0
                    width: 240
                    height: 60

                    model: ListModel {
                        id: screenModel
                        ListElement {
                            chanel: ""
                            volume: 0
                        }
                    }

                    delegate: Column {
                        Text { text: "<b>Channel:</b> " + chanel }
                        Text { text: "<b>Volume:</b> " + volume + "%" }
                    }
                }
*/

            }
        }

        RowLayout {
            id: rowLayout_2
            spacing: 6

            Layout.topMargin: 10
            Layout.leftMargin: 10
            Layout.rightMargin: 10
            Layout.alignment: Qt.AlignHCenter

            RoundButton {
                id:roundButton_1
                Layout.minimumWidth: 50
                text: "1"

                onClicked: {
                    if (columnLayout_0.switched_on) {
                        labelChannel.text = "CH1"
//                        screenModel.append({ "chanel" : "1" })
                    }
                }
            }
            RoundButton {
                id:roundButton_2
                Layout.minimumWidth: 50
                text: "2"

                onClicked: {
                }

            }
            RoundButton {
                id:roundButton_3
                Layout.minimumWidth: 50
                text: "3"
            }
        }

        RowLayout {
            id: rowLayout_3
            spacing: 6

            Layout.topMargin: 6
            Layout.leftMargin: 10
            Layout.rightMargin: 10
            Layout.alignment: Qt.AlignHCenter

            RoundButton {
                id:roundButton_4
                Layout.minimumWidth: 50
                text: "4"
            }
            RoundButton {
                id:roundButton_5
                Layout.minimumWidth: 50
                text: "5"
            }
            RoundButton {
                id:roundButton_6
                Layout.minimumWidth: 50
                text: "6"
            }
        }

        RowLayout {
            id: rowLayout_4
            spacing: 6

            Layout.topMargin: 6
            Layout.leftMargin: 10
            Layout.rightMargin: 10
            Layout.alignment: Qt.AlignHCenter

            RoundButton {
                id:roundButton_7
                Layout.minimumWidth: 50
                text: "7"
            }
            RoundButton {
                id:roundButton_8
                Layout.minimumWidth: 50
                text: "8"
            }
            RoundButton {
                id:roundButton_9
                Layout.minimumWidth: 50
                text: "9"
            }
        }

        RowLayout {
            id: rowLayout_5
            spacing: 6

            Layout.topMargin: 6
            Layout.leftMargin: 10
            Layout.rightMargin: 10
            Layout.bottomMargin: 10
            Layout.alignment: Qt.AlignHCenter

            RoundButton {
                id: roundButton_0
                Layout.minimumWidth: 50
                text: "0"
            }
        }

        RowLayout {
            id: rowLayout_6
            spacing: 30

            Layout.topMargin: 10
            Layout.leftMargin: 10
            Layout.rightMargin: 10
            Layout.alignment: Qt.AlignHCenter

            RoundButton {
                id: volum_Up

                Layout.minimumWidth: 40
                Layout.minimumHeight: 60
                text: "+"
            }

            RoundButton {
                id: channel_Up

                Layout.minimumWidth: 40
                Layout.minimumHeight: 60

                text: "+"
            }
        }

        RowLayout {
            id: rowLayout_7
            spacing: 30

            Layout.leftMargin: 10
            Layout.rightMargin: 10
            Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter

            Text {
                Layout.leftMargin: 20
                Layout.minimumWidth: 40
                text: "VOL"
            }

            Text {
                Layout.minimumWidth: 40
                text: "CH"
            }
        }

        RowLayout {
            id: rowLayout_8
            spacing: 30

            Layout.leftMargin: 10
            Layout.rightMargin: 10
            Layout.alignment: Qt.AlignHCenter

            RoundButton {
                id: volum_Down

                Layout.minimumWidth: 40
                Layout.minimumHeight: 60
                text: "-"
            }

            RoundButton {
                id: channel_Down

                Layout.minimumWidth: 40
                Layout.minimumHeight: 60
                text: "-"
            }

        }

        RowLayout {
            id: rowLayout_9

            Layout.topMargin: 10
            Layout.leftMargin: 10
            Layout.rightMargin: 10
            Layout.bottomMargin: 30
            Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter

            Text {
                text: "Brandname"
                font.pixelSize: 20;
            }
        }
    }

}
