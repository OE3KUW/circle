#include "DriveAdapter.h"

const int chL = 0;
const int chR = 1;
const int chDirL = 2;
const int chDirR = 3;

uint8_t DriveAdapter::pinL = WHEEL_PIN_L;
uint8_t DriveAdapter::pinR = WHEEL_PIN_R;
uint8_t DriveAdapter::pinDirL = WHEEL_DIR_PIN_L;
uint8_t DriveAdapter::pinDirR = WHEEL_DIR_PIN_R;
Mode DriveAdapter::mode = Integrated_ic;
direction_t DriveAdapter::pulseCnt = direction_t(0, 0);
// integrated_ic == default mode


DriveAdapter::DriveAdapter() {
    _speed = 0;
    _diff = 0;
}

DriveAdapter::DriveAdapter(Mode m) {
    _speed = 0;
    _diff = 0;
    speed = direction_t(0, 0);
    direction = direction_t(0, 0);
    setup(m);
}

void DriveAdapter::setup(Mode m) {
    mode = m;
    /*if(mode == External_ic) {
        DriveAdapter::pinL = MOTOR_LEFT_FORWARD;
        DriveAdapter::pinR = MOTOR_RIGHT_FORWARD;
        DriveAdapter::pinDirL = MOTOR_LEFT_BACKWARD;
        DriveAdapter::pinDirR = MOTOR_RIGHT_BACKWARD;
    }*/

    pinMode(pinL, OUTPUT);
    pinMode(pinR, OUTPUT);
    pinMode(pinDirL, OUTPUT);
    pinMode(pinDirR, OUTPUT);
    pinMode(ImpulsePinL, INPUT);
    pinMode(ImpulsePinR, OUTPUT);

    attachInterrupt(digitalPinToInterrupt(ImpulsePinL), isr_puls_l, FALLING);
    attachInterrupt(digitalPinToInterrupt(ImpulsePinR), isr_puls_r, FALLING);

    ledcSetup(chL, 16000, 10);
    ledcAttachPin(pinL, chL);
    ledcSetup(chR, 16000, 10);
    ledcAttachPin(pinR, chR);
    if (mode == External_ic) {
        ledcSetup(chDirL, 16000, 10);
        ledcAttachPin(pinDirL, chDirL);
        ledcSetup(chDirR, 16000, 10);
        ledcAttachPin(pinDirR, chDirR);
    }
}

DriveAdapter::~DriveAdapter() {
}

void DriveAdapter::setSpeed(int v) {
    _speed = v;
    speed.l = _speed - _diff;
    speed.r = _speed - _diff;
    update();
}

void DriveAdapter::setSpeed(direction_t _speed) {
    speed = _speed;
    update();
}


void DriveAdapter::setDiff(int v) {
    _diff = v;
    speed.l = _speed - _diff;
    speed.r = _speed + _diff;
    update();
}


void DriveAdapter::update() {
    direction = speed.getUnit();
    if (mode == Integrated_ic) {
        switch (direction.l) {
            case 1:
                digitalWrite(pinDirL, HIGH);
                break;
            case -1:
                digitalWrite(pinDirL, LOW);
                break;
        }
        switch (direction.r) {
            case 1:
                digitalWrite(pinDirR, HIGH);
                break;
            case -1:
                digitalWrite(pinDirR, LOW);
                break;
        }
        ledcWrite(chL, speed.l);
        ledcWrite(chR, speed.r);
    } else if(mode == External_ic) {
        switch (direction.l) {
            case 1:
                ledcWrite(chDirR, speed.l);
                ledcWrite( chL, 0);
                break;
            case -1:
                ledcWrite( chL, speed.l * -1);
                ledcWrite(chDirR, 0);
                break;
        }
        switch (direction.r) {
            case 1:
                ledcWrite(chR, speed.r);
                ledcWrite( chDirL, 0);
                break;
            case -1:
                ledcWrite( chDirL, speed.r * -1);
                ledcWrite(chR, 0);
                break;
        }
    }

}

String DriveAdapter::getLog() {
    return "DriveAdapter: " + speed.toString() + direction.toString() + pulseCnt.toString();
}

void isr_puls_l() {
    DriveAdapter::pulseCnt.l++;
}

void isr_puls_r() {
    DriveAdapter::pulseCnt.r++;
}

