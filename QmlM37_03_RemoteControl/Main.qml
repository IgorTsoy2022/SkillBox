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

    property bool switched_on : false
    property int channel : 0
    property int volume : 0

    function show_channel() {
        if (channel < 10) {
            return "00" + channel;
        }
        else if (channel < 100) {
            return "0" + channel;
        }
        return "" + channel;
    }

    function increment_channel() {
        if (channel > 998) {
            channel = 0;
        }
        else {
            ++channel;
        }
        return show_channel();
    }

    function decrement_channel() {
        if (channel < 1) {
            channel = 999;
        }
        else {
            --channel;
        }
        return show_channel();
    }

    function change_channel(num) {
        if (roundButton_N.checked) {
            if (channel < 100) {
                channel = channel * 10 + num
            }
        }
        else {
            channel = num;
        }
        return show_channel();
    }

    function increment_volume() {
        if (volume > 90) {
            return volume;
        }

        volume += 10;

        return volume;
    }

    function decrement_volume() {
        if (volume < 10) {
            return volume;
        }

        volume -= 10;

        return volume;
    }

    ColumnLayout {
        id: columnLayout_0
        spacing: 0
        width: parent.width
        height: parent.height

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
                    mainWindow.switched_on = !mainWindow.switched_on;
                    if (mainWindow.switched_on) {
                        labelChannel.opacity = 1;
                        labelVolume.opacity = 1;
                    }
                    else {
                        labelChannel.opacity = 0;
                        labelVolume.opacity = 0;
                    }
                }
            }
        }

        RowLayout {
            id: rowLayout_1

            height: 120

            Layout.leftMargin: 10
            Layout.rightMargin: 10
            Layout.maximumHeight: 120

            Rectangle {
                id:rectangle_0

                border.color: "grey"
                border.width: 2

                Layout.fillWidth: true
                Layout.fillHeight: true

                Label {
                    id: labelChannel
                    opacity: 0.0
                    width: parent.width
                    height: 80

                    text: "CH#000"
                    font.pixelSize: 16
                    font.bold: true
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                Label {
                    id: labelVolume
                    opacity:0.0

                    anchors.top: labelChannel.bottom
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom

                    text: "Volume: 0 %"
                    font.pixelSize: 12
                    font.bold: true
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
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
                    if (mainWindow.switched_on) {
                        labelChannel.text = "CH#" + change_channel(1)
                    }
                }
            }
            RoundButton {
                id:roundButton_2
                Layout.minimumWidth: 50
                text: "2"

                onClicked: {
                    if (mainWindow.switched_on) {
                        labelChannel.text = "CH#" + change_channel(2)
                    }
                }
            }
            RoundButton {
                id:roundButton_3
                Layout.minimumWidth: 50
                text: "3"

                onClicked: {
                    if (mainWindow.switched_on) {
                        labelChannel.text = "CH#" + change_channel(3)
                    }
                }
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

                onClicked: {
                    if (mainWindow.switched_on) {
                        labelChannel.text = "CH#" + change_channel(4)
                    }
                }
            }
            RoundButton {
                id:roundButton_5
                Layout.minimumWidth: 50
                text: "5"

                onClicked: {
                    if (mainWindow.switched_on) {
                        labelChannel.text = "CH#" + change_channel(5)
                    }
                }
            }
            RoundButton {
                id:roundButton_6
                Layout.minimumWidth: 50
                text: "6"

                onClicked: {
                    if (mainWindow.switched_on) {
                        labelChannel.text = "CH#" + change_channel(6)
                    }
                }
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

                onClicked: {
                    if (mainWindow.switched_on) {
                        labelChannel.text = "CH#" + change_channel(7)
                    }
                }
            }
            RoundButton {
                id:roundButton_8
                Layout.minimumWidth: 50
                text: "8"

                onClicked: {
                    if (mainWindow.switched_on) {
                        labelChannel.text = "CH#" + change_channel(8)
                    }
                }
            }
            RoundButton {
                id:roundButton_9
                Layout.minimumWidth: 50
                text: "9"

                onClicked: {
                    if (mainWindow.switched_on) {
                        labelChannel.text = "CH#" + change_channel(9)
                    }
                }
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
                id: roundButton_C
                Layout.minimumWidth: 50
                text: "C"

                onClicked: {
                    if (mainWindow.switched_on) {
                        mainWindow.channel = 0
                        labelChannel.text = "CH#000"
                    }
                }
            }

            RoundButton {
                id: roundButton_0
                Layout.minimumWidth: 50
                text: "0"

                onClicked: {
                    if (mainWindow.switched_on) {
                        labelChannel.text = "CH#" + change_channel(0)
                    }
                }
            }

            RoundButton {
                id: roundButton_N
                Layout.minimumWidth: 50
                text: ">9"
                checkable: true

                onClicked: {
                    checked: !checked
                }

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

                onClicked: {
                    if (mainWindow.switched_on) {
                        labelVolume.text = "Volume: " + increment_volume() + "%"
                    }
                }
            }

            RoundButton {
                id: channel_Up

                Layout.minimumWidth: 40
                Layout.minimumHeight: 60

                text: "+"

                onClicked: {
                    if (mainWindow.switched_on) {
                        labelChannel.text = "CH#" + increment_channel()
                    }
                }
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

                onClicked: {
                    if (mainWindow.switched_on) {
                        labelVolume.text = "Volume: " + decrement_volume() + "%"
                    }
                }
            }

            RoundButton {
                id: channel_Down

                Layout.minimumWidth: 40
                Layout.minimumHeight: 60
                text: "-"

                onClicked: {
                    if (mainWindow.switched_on) {
                        labelChannel.text = "CH#" + decrement_channel()
                    }
                }
            }

        }

        RowLayout {
            id: rowLayout_9

            Layout.topMargin: 10
            Layout.leftMargin: 10
            Layout.rightMargin: 10
            Layout.bottomMargin: 30
            Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter

            Label {
                text: "Brandname"
                font.pixelSize: 20;
            }
        }
    }

}
