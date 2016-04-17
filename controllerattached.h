/*
 * steamlink-qml-example
 * Copyright (c) 2016 Mackenzie Straight. See LICENSE for license details.
 */
#ifndef CONTROLLERATTACHED_H
#define CONTROLLERATTACHED_H

#include <QObject>
#include <QInputEvent>
#include <QtQml>

class QuickControllerEvent;
class ControllerSignalName;

class ControllerAttached : public QObject
{
    Q_OBJECT

public:
    explicit ControllerAttached(QObject *parent = 0);
    static void initialize();
    static ControllerAttached *qmlAttachedProperties(QObject *);

private:
    virtual bool eventFilter(QObject *obj, QEvent *evt);
    void sendSpecificEvent(
            const ControllerSignalName *signalTable,
            QuickControllerEvent& qcevt);

signals:
    void pressed(QuickControllerEvent *event);
    void released(QuickControllerEvent *event);
    void trigger(QuickControllerEvent *event);
    void stick(QuickControllerEvent *event);
    void dpad(QuickControllerEvent *event);

    void buttonAPressed(QuickControllerEvent *event);
    void buttonBPressed(QuickControllerEvent *event);
    void buttonXPressed(QuickControllerEvent *event);
    void buttonYPressed(QuickControllerEvent *event);
    void buttonBackPressed(QuickControllerEvent *event);
    void buttonGuidePressed(QuickControllerEvent *event);
    void buttonStartPressed(QuickControllerEvent *event);
    void buttonLeftStickPressed(QuickControllerEvent *event);
    void buttonRightStickPressed(QuickControllerEvent *event);
    void buttonLeftShoulderPressed(QuickControllerEvent *event);
    void buttonRightShoulderPressed(QuickControllerEvent *event);

    void buttonAReleased(QuickControllerEvent *event);
    void buttonBReleased(QuickControllerEvent *event);
    void buttonXReleased(QuickControllerEvent *event);
    void buttonYReleased(QuickControllerEvent *event);
    void buttonBackReleased(QuickControllerEvent *event);
    void buttonGuideReleased(QuickControllerEvent *event);
    void buttonStartReleased(QuickControllerEvent *event);
    void buttonLeftStickReleased(QuickControllerEvent *event);
    void buttonRightStickReleased(QuickControllerEvent *event);
    void buttonLeftShoulderReleased(QuickControllerEvent *event);
    void buttonRightShoulderReleased(QuickControllerEvent *event);

    void leftTrigger(QuickControllerEvent *event);
    void rightTrigger(QuickControllerEvent *event);

    void leftStick(QuickControllerEvent *event);
    void rightStick(QuickControllerEvent *event);

    void dpadCenterPressed(QuickControllerEvent *event);
    void dpadUpPressed(QuickControllerEvent *event);
    void dpadUpRightPressed(QuickControllerEvent *event);
    void dpadRightPressed(QuickControllerEvent *event);
    void dpadDownRightPressed(QuickControllerEvent *event);
    void dpadDownPressed(QuickControllerEvent *event);
    void dpadDownLeftPressed(QuickControllerEvent *event);
    void dpadLeftPressed(QuickControllerEvent *event);
    void dpadUpLeftPressed(QuickControllerEvent *event);
};

QML_DECLARE_TYPEINFO(ControllerAttached, QML_HAS_ATTACHED_PROPERTIES)

class QuickControllerEvent: public QObject
{
    Q_OBJECT
    Q_PROPERTY(int type READ type)
    Q_PROPERTY(int direction READ direction)
    Q_PROPERTY(qreal distance READ distance)
    Q_PROPERTY(qreal angle READ angle)
    Q_PROPERTY(bool accepted READ isAccepted WRITE setAccepted)

public:
    QuickControllerEvent(): _accepted(false) { }
    QuickControllerEvent(const QControllerEvent &event):
        _event(event), _accepted(event.isAccepted()) { }

    int type() const { return _event.type(); }
    int direction() const { return _event.direction(); }
    qreal distance() const { return _event.distance(); }
    qreal angle() const { return _event.angle(); }
    bool isAccepted() const { return _accepted; }
    void setAccepted(bool value) { _accepted = value; }

private:
    QControllerEvent _event;
    bool _accepted;
};

#endif // CONTROLLERATTACHED_H
