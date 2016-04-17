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
        border.color: "white"
        anchors.fill: parent
    }
}
