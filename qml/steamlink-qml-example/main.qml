/*
 * steamlink-qml-example
 * Copyright (c) 2016 Mackenzie Straight. See LICENSE for license details.
 */
import QtQuick 2.0
import org.e7m.steamlink 1.0

Rectangle {
    id: page
    width: 320; height: 480
    color: "lightgray"

    Text {
        id: helloText
        text: "Hello world!"
        y: 30
        anchors.horizontalCenter: page.horizontalCenter
        font.pointSize: 24; font.bold: true
    }

    Grid {
        id: colorPicker
        x: 4;
        anchors.top: helloText.bottom
        anchors.topMargin: 30
        anchors.rightMargin: 4
        anchors.bottom: page.bottom;
        anchors.bottomMargin: 4
        anchors.right: page.right;
        anchors.left: page.left;
        rows: 2; columns: 3;
        focus: true

        Cell { cellColor: "red"; }
        Cell { cellColor: "green"; }
        Cell { cellColor: "blue"; }
        Cell { cellColor: "yellow"; }
        Cell { cellColor: "steelblue"; }
        Cell { cellColor: "black"; }

        Controller.onPressed: {
            console.log("Generic handler", event.type)
            event.accepted = event.type === 2
        }

        Controller.onReleased: {
            console.log("Button released")
        }

        Controller.onButtonAPressed: {
            console.log("Specific handler auto-accepts like Keys")
        }
    }
}
