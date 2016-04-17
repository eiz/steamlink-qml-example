/*
 * steamlink-qml-example
 * Copyright (c) 2016 Mackenzie Straight. See LICENSE for license details.
 */
import QtQuick 2.0

Item {
    id: container
    property alias cellColor: rectangle.color

    width: parent.width / parent.columns
    height: parent.height / parent.rows

    Rectangle {
        id: rectangle
        border.color: focus ? "blue" : "white"
        border.width: 4
        anchors.fill: parent

        Behavior on border.color {
            ColorAnimation { duration: 200 }
        }
    }

    states: [
        State {
            name: "SELECTED"
            when: (container.focus)
            PropertyChanges { target: rectangle; border.color: "blue" }
        },

        State {
            name: "NORMAL"
            when: (!container.focus)
            PropertyChanges { target: rectangle; border.color: "white" }
        }
    ]
}
