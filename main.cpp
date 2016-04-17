/*
 * steamlink-qml-example
 * Copyright (c) 2016 Mackenzie Straight. See LICENSE for license details.
 */
#include <QtGui/QGuiApplication>
#include <QFont>

#include "controllerattached.h"
#include "controllermanager.h"
#include "qtquick2applicationviewer.h"

static FILE *logFile;

static void logMessageHandler(
    QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QByteArray localMsg = msg.toLocal8Bit();

    if (!logFile) {
        return;
    }

    switch (type) {
    case QtDebugMsg:
        fprintf(logFile, "D/[%s@%s:%u] %s\n",
            context.function, context.file, context.line, localMsg.constData());
        break;
    case QtWarningMsg:
        fprintf(logFile, "W/[%s@%s:%u] %s\n",
            context.function, context.file, context.line, localMsg.constData());
        break;
    case QtCriticalMsg:
        fprintf(logFile, "C/[%s@%s:%u] %s\n",
            context.function, context.file, context.line, localMsg.constData());
        break;
    case QtFatalMsg:
        fprintf(logFile, "F/[%s@%s:%u] %s\n",
            context.function, context.file, context.line, localMsg.constData());
        break;
    }

    fflush(logFile);
}

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QtQuick2ApplicationViewer viewer;

    // Start logging
    logFile = fopen("/tmp/steamlink-qml-example.log", "w");
    qInstallMessageHandler(logMessageHandler);
    qWarning() << "Starting steamlink-qml-example.";

    // Move the mouse cursor off the screen so that it doesn't show on top
    // of the GUI.
    QCursor().setPos(0xFFFF, 0xFFFF);

    // Steam Link only has the Noto font family. Set it as the default font.
    app.setFont(QFont("Noto Sans"));

    // Setup controller input
    CControllerManager controllerManager(&app);
    app.installEventFilter(&controllerManager);
    ControllerAttached::initialize();

    viewer.setMainQmlFile(QStringLiteral("qml/steamlink-qml-example/main.qml"));
    viewer.showExpanded();

    return app.exec();
}
