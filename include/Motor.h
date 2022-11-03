//
// Created by dagra on 02/11/2022.
//

#ifndef CIRCLE_MOTOR_H
#define CIRCLE_MOTOR_H


class Motor {
private:
    int pin1;
    int pin2;
    int ch1;
    int ch2;
    int currentSpeed; //slow-follows the controlSpeed
    int controlSpeed;
public:
    int getControlSpeed() const;

    void setControlSpeed(int controlSpeed);

private:
    //controls the speed
    static int lastchannel;

public:

    static const int updatedelay = 250; //time in micros (* 256 = ramptime = time to go from 0 to max)

    Motor() {}
    Motor(int, int, int, int);
    void setSpeed(int);
    void drive();
    void adjustCurrentSpeed();
};


#endif //CIRCLE_MOTOR_H
