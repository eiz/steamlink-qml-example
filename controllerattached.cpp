/*
 * steamlink-qml-example
 * Copyright (c) 2016 Mackenzie Straight. See LICENSE for license details.
 */

#include <QtGui>
#include "controllerattached.h"

ControllerAttached::ControllerAttached(QObject *parent) :
    QObject(parent)
{
    _lastDpadDirection = QControllerEvent::DIRECTION_CENTER;
    _simulatedDpad = QControllerEvent::NONE;
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
    qmlRegisterUncreatableType<QuickControllerEventDirection>(
        "org.e7m.steamlink", 1, 0, "ControllerEventDirection",
        "Only for enum values");
    qmlRegisterUncreatableType<QuickControllerEventType>(
        "org.e7m.steamlink", 1, 0, "ControllerEventType",
        "Only for enum values");
}

struct ControllerSignalName
{
    const char *name;
    int type;
};

static const ControllerSignalName pressedSignals[] =
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

static const ControllerSignalName releasedSignals[] =
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

static const ControllerSignalName triggerSignals[] =
{
    { "leftTrigger", QControllerEvent::TRIGGER_LEFT },
    { "rightTrigger", QControllerEvent::TRIGGER_RIGHT },
    { 0, 0 },
};

static const ControllerSignalName stickSignals[] =
{
    { "leftStick", QControllerEvent::THUMBSTICK_LEFT },
    { "rightStick", QControllerEvent::THUMBSTICK_RIGHT },
    { 0, 0 },
};

static const ControllerSignalName dpadPressedSignals[] =
{
    { "dpadCenterPressed", QControllerEvent::DIRECTION_CENTER },
    { "dpadUpPressed", QControllerEvent::DIRECTION_N },
    { "dpadUpRightPressed", QControllerEvent::DIRECTION_NE },
    { "dpadRightPressed", QControllerEvent::DIRECTION_E },
    { "dpadDownRightPressed", QControllerEvent::DIRECTION_SE },
    { "dpadDownPressed", QControllerEvent::DIRECTION_S },
    { "dpadDownLeftPressed", QControllerEvent::DIRECTION_SW },
    { "dpadLeftPressed", QControllerEvent::DIRECTION_W },
    { "dpadUpLeftPressed", QControllerEvent::DIRECTION_NW },
    { 0, 0 },
};

static const char *signalNameForEventType(
    const ControllerSignalName *names, int type)
{
    for (int i = 0; names[i].name; ++i) {
        if (names[i].type == type) {
            return names[i].name;
        }
    }

    return NULL;
}

void ControllerAttached::sendSpecificEvent(
    const ControllerSignalName *signalTable,
    QuickControllerEvent& qcevt,
    int tableValue)
{
    const char *sigName = signalNameForEventType(
        signalTable, tableValue);

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
}

#define HANDLE_CONTROLLER_EVENT(genericEvent, table, value)  \
    do {                                                     \
        sendSpecificEvent(table, qcevt, qcevt.value());      \
                                                             \
        if (!qcevt.isAccepted()) {                           \
            emit genericEvent(&qcevt);                       \
        }                                                    \
                                                             \
        evt->setAccepted(qcevt.isAccepted());                \
                                                             \
        if (qcevt.isAccepted()) {                            \
            return true;                                     \
        }                                                    \
    } while(0)

bool ControllerAttached::eventFilter(QObject *obj, QEvent *evt)
{
    if (evt->type() == QEvent::Controller) {
        QControllerEvent *cevt = reinterpret_cast<QControllerEvent *>(evt);
        QuickControllerEvent qcevt(*cevt);

        if (cevt->isButtonPress()) {
            HANDLE_CONTROLLER_EVENT(pressed, pressedSignals, type);
        }

        if (cevt->isButtonRelease()) {
            HANDLE_CONTROLLER_EVENT(released, releasedSignals, type);
        }

        if (cevt->isTrigger()) {
            HANDLE_CONTROLLER_EVENT(trigger, triggerSignals, type);
        }

        if (cevt->isThumbstick()) {
            HANDLE_CONTROLLER_EVENT(stick, stickSignals, type);

            if (_simulatedDpad == cevt->type()) {
                QControllerEvent::Direction currentDirection =
                    QControllerEvent::DIRECTION_CENTER;
                float degrees = qRadiansToDegrees(cevt->angle());

                if (degrees <= 45 || degrees > 315) {
                    currentDirection = QControllerEvent::DIRECTION_N;
                } else if (degrees > 45 && degrees <= 135) {
                    currentDirection = QControllerEvent::DIRECTION_E;
                } else if (degrees > 135 && degrees <= 225) {
                    currentDirection = QControllerEvent::DIRECTION_S;
                } else if (degrees > 225 && degrees < 315) {
                    currentDirection = QControllerEvent::DIRECTION_W;
                }

                if (cevt->distance() == 0) {
                    currentDirection = QControllerEvent::DIRECTION_CENTER;
                }

                if (_lastDpadDirection != currentDirection) {
                    _lastDpadDirection = currentDirection;
                    qcevt.setType(QControllerEvent::DPAD);
                    qcevt.setDirection(currentDirection);
                    HANDLE_CONTROLLER_EVENT(
                        dpad, dpadPressedSignals, direction);
                }
            }
        }

        if (cevt->isDPad()) {
            HANDLE_CONTROLLER_EVENT(dpad, dpadPressedSignals, direction);
        }
    }

    return QObject::eventFilter(obj, evt);
}
