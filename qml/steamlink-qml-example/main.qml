/*
 * steamlink-qml-example
 * Copyright (c) 2016 Mackenzie Straight. See LICENSE for license details.
 */
import QtQuick 2.0
import org.e7m.steamlink 1.0

Rectangle {
    id: page
    width: 320; height: 480
    color: "white"

    Text {
        id: helloText
        text: "Hello world!"
        y: 30
        anchors.horizontalCenter: page.horizontalCenter
        font.pointSize: 24; font.bold: true
    }

    Text {
        id: ltText
        text: "0"
        y: 30
        anchors.left: page.left
        font.pointSize: 24; font.bold: true
    }

    Text {
        id: rtText
        text: "0"
        y: 30
        anchors.right: page.right
        font.pointSize: 24; font.bold: true
    }

    ListView {
        id: sidebar
        anchors.top: helloText.bottom
        anchors.topMargin: 30
        anchors.bottom: page.bottom
        anchors.left: page.left
        anchors.leftMargin: 10
        width: page.width / 5 - 20

        model: ListModel {
            ListElement {
                name: "Menu 1"
                source: "Menu1.qml"
            }

            ListElement {
                name: "Menu 2"
                source: "Menu2.qml"
            }

            ListElement {
                name: "Menu 3"
                source: "Menu3.qml"
            }
        }
        delegate: Row {
            Text { text: name; font.pointSize: 18 }
        }
    }

    Grid {
        id: colorPicker
        x: 4;
        anchors.top: helloText.bottom
        anchors.topMargin: 30
        anchors.rightMargin: 4
        anchors.bottom: page.bottom
        anchors.bottomMargin: 4
        anchors.right: page.right
        anchors.left: page.left
        anchors.leftMargin: page.width / 5
        rows: 2; columns: 3;

        Cell { id: cellR; cellColor: "red"; focus: true }
        Cell { id: cellG; cellColor: "green" }
        Cell { id: cellB; cellColor: "blue" }
        Cell { id: cellY; cellColor: "yellow" }
        Cell { id: cellSB; cellColor: "steelblue" }
        Cell { id: cell0; cellColor: "black" }

        Controller.onPressed: {
            console.log("Generic handler", event.type)
            switch (event.type) {
            case ControllerEventType.ButtonA:
                cellG.focus = true; event.accepted = true;
                break;
            case ControllerEventType.ButtonB:
                cellR.focus = true; event.accepted = true;
                break;
            case ControllerEventType.ButtonX:
                cellB.focus = true; event.accepted = true;
                break;
            case ControllerEventType.ButtonY:
                cellY.focus = true; event.accepted = true;
                break;
            }
        }

        Controller.onReleased: {
            console.log("Button released")
        }

        Controller.onButtonGuidePressed: Qt.quit()

        Controller.onLeftTrigger: {
            ltText.text = event.distance.toString()
        }

        Controller.onRightTrigger: {
            rtText.text = event.distance.toString()
        }

        Controller.onLeftStick: {
            helloText.text = "A: " + event.angle + " D:" + event.distance
        }

        Controller.onDpadLeftPressed: {
            helloText.text = "Left"
        }

        Controller.onDpadUpPressed: {
            helloText.text = "Up"
        }

        Controller.onDpadRightPressed: {
            helloText.text = "Right"
        }

        Controller.onDpadDownPressed: {
            helloText.text = "Down"
        }
    }
}
