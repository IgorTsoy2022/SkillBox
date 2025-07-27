import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Window {
    width: 240
    height: 680
    visible: true
    title: qsTr("Remote Control")

    maximumWidth: 360
    maximumHeight: 1000
    minimumWidth: 240
    minimumHeight: 680

    ColumnLayout {
        spacing: 10
        width: parent.width
        height: parent.height

        RowLayout {
            id: rowLayout1
            spacing: 2

            Layout.bottomMargin: 10
            Layout.leftMargin: 10
            Layout.rightMargin: 10

            RoundButton {
                id: roundButton_On
                text: String.fromCodePoint(0x000023FC)

                palette.button: "#FF0000"
            }
        }

        ListView {
            id: listView
            height: 180

            Layout.leftMargin: 10
            Layout.rightMargin: 10

            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.minimumWidth: 0
            Layout.minimumHeight: 30
            Layout.maximumWidth: 360
            Layout.maximumHeight: 180


        }

        RowLayout {
            id: rowLayout2
            spacing: 2
        }
    }
}
