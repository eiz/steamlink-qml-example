/*
 * steamlink-qml-example
 * Copyright (c) 2016 Mackenzie Straight. See LICENSE for license details.
 */

#include <QtGui>
#include <syslog.h>
#include "controllerattached.h"

ControllerAttached::ControllerAttached(QObject *parent) :
    QObject(parent)
{
    parent->installEventFilter(this);
}

ControllerAttached *ControllerAttached::qmlAttachedProperties(QObject *obj)
{
    return new ControllerAttached(obj);
}

void ControllerAttached::initialize()
{
    qmlRegisterUncreatableType<ControllerAttached>(
        "org.e7m.steamlink", 1, 0, "Controller",
        "Can only be created via attached properties");
    qmlRegisterType<QuickControllerEvent>(
        "org.e7m.steamlink", 1, 0, "ControllerEvent");
}

struct SignalName
{
    const char *name;
    int type;
};

static const SignalName pressedSignals[] =
{
    { "buttonAPressed", QControllerEvent::BUTTON_A },
    { "buttonBPressed", QControllerEvent::BUTTON_B },
    { "buttonXPressed", QControllerEvent::BUTTON_X },
    { "buttonYPressed", QControllerEvent::BUTTON_Y },
    { "buttonBackPressed", QControllerEvent::BUTTON_BACK },
    { "buttonGuidePressed", QControllerEvent::BUTTON_GUIDE },
    { "buttonStartPressed", QControllerEvent::BUTTON_START },
    { "buttonLeftStickPressed", QControllerEvent::BUTTON_LEFTSTICK },
    { "buttonRightStickPressed", QControllerEvent::BUTTON_RIGHTSTICK },
    { "buttonLeftShoulderPressed", QControllerEvent::BUTTON_LEFTSHOULDER },
    { "buttonRightShoulderPressed", QControllerEvent::BUTTON_RIGHTSHOULDER },
    { 0, 0 }
};

static const SignalName releasedSignals[] =
{
    { "buttonAReleased", QControllerEvent::BUTTON_A },
    { "buttonBReleased", QControllerEvent::BUTTON_B },
    { "buttonXReleased", QControllerEvent::BUTTON_X },
    { "buttonYReleased", QControllerEvent::BUTTON_Y },
    { "buttonBackReleased", QControllerEvent::BUTTON_BACK },
    { "buttonGuideReleased", QControllerEvent::BUTTON_GUIDE },
    { "buttonStartReleased", QControllerEvent::BUTTON_START },
    { "buttonLeftStickReleased", QControllerEvent::BUTTON_LEFTSTICK },
    { "buttonRightStickReleased", QControllerEvent::BUTTON_RIGHTSTICK },
    { "buttonLeftShoulderReleased", QControllerEvent::BUTTON_LEFTSHOULDER },
    { "buttonRightShoulderReleased", QControllerEvent::BUTTON_RIGHTSHOULDER },
    { 0, 0 }
};

static const char *signalNameForEventType(const SignalName *names, int type)
{
    for (int i = 0; names[i].name; ++i) {
        if (names[i].type == type) {
            return names[i].name;
        }
    }

    return NULL;
}

bool ControllerAttached::eventFilter(QObject *obj, QEvent *evt)
{
    if (evt->type() == QEvent::Controller) {
        // Bubble the event unless explicitly handled.
        evt->ignore();

        QControllerEvent *cevt = reinterpret_cast<QControllerEvent *>(evt);
        QuickControllerEvent qcevt(*cevt);

        if (cevt->isButtonPress()) {
            const char *sigName = signalNameForEventType(
                        pressedSignals, cevt->type());

            if (sigName) {
                QString sigNameWithType = sigName;

                sigNameWithType.append("(QuickControllerEvent*)");
                int signalIndex = metaObject()->indexOfSignal(
                    sigNameWithType.toLocal8Bit().constData());
                QMetaMethod signalMethod = metaObject()->method(signalIndex);

                if (isSignalConnected(signalMethod)) {
                    qcevt.setAccepted(true);
                    signalMethod.invoke(
                        this,
                        Qt::DirectConnection,
                        Q_ARG(QuickControllerEvent*, &qcevt));
                }
            }

            if (!qcevt.isAccepted()) {
                emit pressed(&qcevt);
            }

            evt->setAccepted(qcevt.isAccepted());
        }

        if (cevt->isButtonRelease()) {
            const char *sigName = signalNameForEventType(
                        releasedSignals, cevt->type());

            if (sigName) {
                QString sigNameWithType = sigName;

                sigNameWithType.append("(QuickControllerEvent*)");
                int signalIndex = metaObject()->indexOfSignal(
                    sigNameWithType.toLocal8Bit().constData());
                QMetaMethod signalMethod = metaObject()->method(signalIndex);

                if (isSignalConnected(signalMethod)) {
                    qcevt.setAccepted(true);
                    signalMethod.invoke(
                        this,
                        Qt::DirectConnection,
                        Q_ARG(QuickControllerEvent*, &qcevt));
                }
            }

            if (!qcevt.isAccepted()) {
                emit released(&qcevt);
            }

            evt->setAccepted(qcevt.isAccepted());
        }

        return true;
    }

    return QObject::eventFilter(obj, evt);
}
