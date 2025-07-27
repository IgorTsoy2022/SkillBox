import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Window {
    width: 330
    height: 240
    visible: true
    title: qsTr("Player")

    maximumWidth: 480
    maximumHeight: 340
    minimumWidth: 180
    minimumHeight: 130

    ColumnLayout {
        spacing: 10
        width: parent.width
        height: parent.height

        ListView {
            id: listView
            height: 180

            Layout.topMargin: 10
            Layout.leftMargin: 10
            Layout.rightMargin: 10

            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.minimumWidth: 0
            Layout.minimumHeight: 30
            Layout.maximumWidth: 458
            Layout.maximumHeight: 180

        }

        Slider {
            id: horizontalSlider

            Layout.leftMargin: 10
            Layout.rightMargin: 10

            Layout.fillWidth: true
            Layout.minimumWidth: 0
            Layout.minimumHeight: 30
            Layout.maximumWidth: 458
            Layout.maximumHeight: 30
        }

        RowLayout {
            spacing: 2

            Layout.leftMargin: 10
            Layout.rightMargin: 10
            Layout.bottomMargin: 10

            Button {
                id: pushButton_Rewind
                text: String.fromCodePoint(0x000023EA)

                Layout.fillWidth: true
                Layout.minimumWidth: 30
                Layout.minimumHeight: 30
                Layout.maximumWidth: 90
                Layout.maximumHeight: 30
            }

            Button {
                id: pushButton_Play
                text: String.fromCodePoint(0x000023F5)

                Layout.fillWidth: true
                Layout.minimumWidth: 30
                Layout.minimumHeight: 30
                Layout.maximumWidth: 90
                Layout.maximumHeight: 30
            }

            Button {
                id: pushButton_Pause
                text: String.fromCodePoint(0x000023F8)

                Layout.fillWidth: true
                Layout.minimumWidth: 30
                Layout.minimumHeight: 30
                Layout.maximumWidth: 90
                Layout.maximumHeight: 30
            }

            Button {
                id: pushButton_Stop
                text: String.fromCodePoint(0x000023F9)

                Layout.fillWidth: true
                Layout.minimumWidth: 30
                Layout.minimumHeight: 30
                Layout.maximumWidth: 90
                Layout.maximumHeight: 30
            }

            Button {
                id: pushButton_Forward
                text: String.fromCodePoint(0x000023E9)

                Layout.fillWidth: true
                Layout.minimumWidth: 30
                Layout.minimumHeight: 30
                Layout.maximumWidth: 90
                Layout.maximumHeight: 30
            }

        }
    }
}
