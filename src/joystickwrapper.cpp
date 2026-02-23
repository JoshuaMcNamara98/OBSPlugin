#define SDL_CONTROLLER_AXIS_LEFTX 0
#define SDL_CONTROLLER_AXIS_LEFTY 1
#define SDL_CONTROLLER_AXIS_RIGHTX 2
#define SDL_CONTROLLER_AXIS_RIGHTY 3
#define SDL_CONTROLLER_AXIS_LEFTTRIGGER 4
#define SDL_CONTROLLER_AXIS_RIGHTTRIGGER 5
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#include "joystickwrapper.h"
#include <QDebug>
#include <QStandardPaths>
#include "constants.h"


#define STICK_THRESHOULD 0.2
JoystickWrapper::JoystickWrapper(QObject *parent) : QObject(parent)
{
    settings = new QSettings(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + DEFAULT_SETTINGS_PATH , QSettings::IniFormat);
    // QJoysticks::getInstance()->setVirtualJoystickRange(0.8);
    QJoysticks::getInstance()->setVirtualJoystickEnabled(false);
    connect(QJoysticks::getInstance() , SIGNAL(countChanged()) , this , SLOT(joystickCountChanged()));
    connect(QJoysticks::getInstance() , SIGNAL(povChanged(int,int,int)) , this , SLOT(joystickPOVChanged(int,int,int)));
    connect(QJoysticks::getInstance() , SIGNAL(axisChanged(int,int,qreal)) , this , SLOT(joystickAxisChanged(int,int,qreal)));
    connect(QJoysticks::getInstance() , SIGNAL(buttonChanged(int,int,bool)) , this , SLOT(joystickButtonChanged(int,int,bool)));
    xPressed = false;
    yPressed = false;
    aPressed = false;

}
JoystickWrapper::~JoystickWrapper()
{
    delete settings;
}
QStringList JoystickWrapper::getJoystickNames()
{
    return QJoysticks::getInstance()->deviceNames();
}
void JoystickWrapper::joystickCountChanged()
{
    xPressed = false;
    yPressed = false;
    aPressed = false;

}
void JoystickWrapper::joystickPOVChanged(const int js , const int pov , const int angle)
{
    qDebug()<<angle;
    if(validEvent(js))
    {
        switch(angle)
        {
        case 0:
            emit moveTopBtnTriggered();
            break;
        case 45:
            emit moveTopRightBtnTriggered();
            break;
        case 90:
            emit moveRightBtnTriggered();
            break;
        case 135:
            emit moveBottomRightBtnTriggered();
            break;
        case 180:
            emit moveBottomBtnTriggered();
            break;
        case 225:
            emit moveBottomLeftBtnTriggered();
            break;
        case 270:
            emit moveLeftBtnTriggered();
            break;
        case 315:
            emit moveTopLeftBtnTriggered();
            break;
        case -1:
            emit moveStop();
            break;
        default:
            break;
        }
    }
}
void JoystickWrapper::joystickAxisChanged(const int js , const int axis , const double value)
{
    qDebug()<<axis;
    qDebug()<<value;
    int axesNum = QJoysticks::getInstance()->getNumAxes(js);
//    qDebug()<<"Number of axes"<<axesNum;
//    qDebug()<<(QJoysticks::getInstance()->getInputDevice(js)->axes);


    int axesNum = QJoysticks::getInstance()->getNumAxes(js);
    if(axesNum < 2) // Need at least 2 axes for XY movement
        return;

    // Use SDL2 symbolic axis names for clarity
    if (axis == SDL_CONTROLLER_AXIS_LEFTX || axis == SDL_CONTROLLER_AXIS_LEFTY) {
        double x = QJoysticks::getInstance()->getAxis(js, SDL_CONTROLLER_AXIS_LEFTX);
        double y = QJoysticks::getInstance()->getAxis(js, SDL_CONTROLLER_AXIS_LEFTY);

        // Deadzone filtering (circular)
        double threshold = STICK_THRESHOULD;
        double magnitude = std::sqrt(x * x + y * y);
        if (magnitude < threshold) {
            emit moveStop();
            return;
        }

        // Calculate angle in degrees (atan2 uses y, x)
        double angle = std::atan2(-y, x) * 180.0 / M_PI; // -y to match typical joystick convention (up is negative)
        if (angle < 0) angle += 360.0;

        // 8-way pie slice detection (each slice is 45 degrees)
        if ((angle >= 337.5 && angle < 360.0) || (angle >= 0.0 && angle < 22.5)) {
            emit moveRightBtnTriggered();
        } else if (angle >= 22.5 && angle < 67.5) {
            emit moveTopRightBtnTriggered();
        } else if (angle >= 67.5 && angle < 112.5) {
            emit moveTopBtnTriggered();
        } else if (angle >= 112.5 && angle < 157.5) {
            emit moveTopLeftBtnTriggered();
        } else if (angle >= 157.5 && angle < 202.5) {
            emit moveLeftBtnTriggered();
        } else if (angle >= 202.5 && angle < 247.5) {
            emit moveBottomLeftBtnTriggered();
        } else if (angle >= 247.5 && angle < 292.5) {
            emit moveBottomBtnTriggered();
        } else if (angle >= 292.5 && angle < 337.5) {
            emit moveBottomRightBtnTriggered();
        }
    }

    // Right stick Y axis for zoom control
    if (axis == SDL_CONTROLLER_AXIS_RIGHTY) {
        double threshold = STICK_THRESHOULD;
        if (std::abs(value) < threshold) {
            emit zoomStop();
            return;
        }

        // Scale the joystick value to the zoom speed (value in [-1, 1])
        int scaledSpeed = std::round(std::abs(value) * zoomSpeed);
        if (scaledSpeed == 0) scaledSpeed = 1; // Ensure minimum speed if outside threshold

        if (value < 0) {
            // Zoom in
            emit zoomInBtnTriggered();
        } else {
            // Zoom out
            emit zoomOutBtnTriggered();
        }
        // Note: If you want to pass the speed, update the signal/slot to accept a parameter
        return;
    }
}

void JoystickWrapper::joystickButtonChanged(const int js , const int button , const bool pressed)
{
    qDebug()<<button;
    if(validEvent(js))
    {
        switch(button)
        {
        case BTN_START:
            if(!pressed)
                emit enterBtnTriggered();
            break;
        case BTN_BACK:
            if(!pressed)
                emit backBtnTriggered();
            break;
        case BTN_X:
            if(!pressed)
                emit callPreset(2);
            break;
        case BTN_Y:
            if(!pressed)
                emit callPreset(3);
            break;
        case BTN_A:
            if(!pressed)
                emit callPreset(1);
            break;
        case BTN_B:
            if(!pressed)
                emit focusStateChange();
            break;
        case BTN_TL:
            if(!pressed)
                emit osdMenuBtnTriggered();
            break;
        case BTN_TR:
            if(!pressed)
                emit moveHome();
            break;
        case BTN_LB:
            if(pressed)
                emit zoomInBtnTriggered();
            else
                emit zoomStop();
            break;
        case BTN_RB:
            if(pressed)
                emit zoomOutBtnTriggered();
            else
                emit zoomStop();
            break;
        case BTN_LT:
            if(pressed)
                emit focusInBtnTriggered();
            else
                emit focusStop();
            break;
        case BTN_RT:
            if(pressed)
                emit focusOutBtnTriggered();
            else
                emit focusStop();
            break;
        case DPAD_UP:
            if(pressed)
                emit moveTopBtnTriggered();
            else
                emit moveStop();
            break;
        case DPAD_DOWN:
            if(pressed)
                emit moveBottomBtnTriggered();
            else
                emit moveStop();
            break;
        case DPAD_LEFT:
            if(pressed)
                emit moveLeftBtnTriggered();
            else
                emit moveStop();
            break;
        case DPAD_RIGHT:
            if(pressed)
                emit moveRightBtnTriggered();
            else
                emit moveStop();
            break;
        default:
            break;
        }
    }
}
bool JoystickWrapper::validEvent(int js)
{
    if(!settings->value(SETTING_JOYSTICK_AVAILABLE , false).toBool())
        return false;
    if(js != settings->value(SETTING_SELECTED_JOYSTICK))
        return false;
    return true;
}
