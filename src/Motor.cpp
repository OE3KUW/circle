//
// Created by dagra on 02/11/2022.
//

#include "Motor.h"
#include <Arduino.h>

int Motor::lastchannel = 0;

Motor::Motor(int p1, int p2, int freq, int res)
{
    pin1 = p1;
    pin2 = p2;
    ch1 = Motor::lastchannel;
    Motor::lastchannel += 1;
    ch2 = Motor::lastchannel;
    Motor::lastchannel += 1;
    controlSpeed = 0;
    currentSpeed = 0;

    ledcSetup(ch1, freq, res);
    ledcSetup(ch2, freq, res);
    ledcAttachPin(pin1, ch1);
    ledcAttachPin(pin2, ch2);

}

void Motor::adjustCurrentSpeed()
{
    int lcurrentSpeed = currentSpeed;
    //update currentSpeed
    if(controlSpeed > currentSpeed) {
        currentSpeed += 1;
    } else if(controlSpeed < currentSpeed) {
        currentSpeed -= 1;
    }
    //set new currentSpeed
    if(currentSpeed != lcurrentSpeed) {
        if (currentSpeed == 0)
        {
            ledcWrite(ch1, currentSpeed);
            ledcWrite(ch2, currentSpeed);
        }
        else if (currentSpeed >= 0 && currentSpeed <= 256)
        {
            digitalWrite(pin2, LOW);
            ledcWrite(ch1, currentSpeed);
        }
        else if (currentSpeed < 0 && currentSpeed >= -256)
        {
            digitalWrite(pin1, LOW);
            ledcWrite(ch2, currentSpeed * -1);
        }
    }
}

void Motor::setSpeed(int s)
{
    controlSpeed = s;
}


void Motor::drive()
{
    if (controlSpeed == 0)
    {
        digitalWrite(pin1, LOW);
        digitalWrite(pin2, LOW);
    }
    else if (controlSpeed > 0 && controlSpeed <= 256)
    {
        digitalWrite(pin2, LOW);
        ledcWrite(ch1, currentSpeed);
    }
    else if (controlSpeed < 0 && controlSpeed >= -256)
    {
        digitalWrite(pin1, LOW);
        ledcWrite(ch2, controlSpeed * -1);
    }
}

int Motor::getControlSpeed() const {
    return controlSpeed;
}

void Motor::setControlSpeed(int controlSpeed) {
    Motor::controlSpeed = controlSpeed;
}
