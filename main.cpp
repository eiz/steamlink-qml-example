#include <QtGui/QGuiApplication>
#include "qtquick2applicationviewer.h"
#include <QFont>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QtQuick2ApplicationViewer viewer;

    // Move the mouse cursor off the screen so that it doesn't show on top
    // of the GUI.
    QCursor().setPos(0xFFFF, 0xFFFF);

    // Steam Link only has the Noto font family. Set it as the default font.
    app.setFont(QFont("Noto Sans"));

    viewer.setMainQmlFile(QStringLiteral("qml/steamlink-qml-example/main.qml"));
    viewer.showExpanded();

    return app.exec();
}
