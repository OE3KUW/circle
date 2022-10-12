#include <DriveAdapter.hpp>

const int chL = 0;
const int chR = 1;
const int chDirL = 2;
const int chDirR = 3;

uint8_t DriveAdapter::pinL = WheelPinL;
uint8_t DriveAdapter::pinR = WheelPinR;
uint8_t DriveAdapter::pinDirL = WheelDirPinL;
uint8_t DriveAdapter::pinDirR = WheelDirPinR;
Mode DriveAdapter::mode = Integrated_ic;
direction_t DriveAdapter::pulseCnt = direction_t(0, 0);
// integrated_ic == default mode

direction_t::direction_t(int _l, int _r) {
    l = _l;
    r = _r;
}

direction_t direction_t::operator+(direction_t &d) {
    return {l + d.l, r + d.r};
}

bool direction_t::operator==(direction_t &d) const {
    return (l == d.l && r == d.r);
}

void direction_t::selfcp(direction_t &source) {
    l = source.l;
    r = source.r;
}

direction_t direction_t::getUnit() const {
    direction_t cp = direction_t();
    if (l > 0) {
        cp.l = 1;
    } else if (l < 0) {
        cp.l = -1;
    } else {
        cp.l = 0;
    }
    if (r > 0) {
        cp.r = 1;
    } else if (l < 0) {
        cp.r = -1;
    } else {
        cp.r = 0;
    }
    return cp;
}

String direction_t::getLog() {
    String msg = "direction_t: ";
    msg.concat(l);
    msg.concat(", ");
    msg.concat(r);
    msg.concat(" ");
    return msg;
}


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

void DriveAdapter::setSpeed(int sL, int sR) {
    speed.l = sL;
    speed.r = sR;
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
                ledcWrite(chL, speed.l);
                ledcWrite( chDirL, 0);
            break;
            case -1:
                ledcWrite( chDirL, speed.l);
                ledcWrite(chL, 0);
            break;
        }
        switch (direction.r) {
            case 1:
                ledcWrite(chR, speed.l);
                ledcWrite( chDirR, 0);
                break;
            case -1:
                ledcWrite( chDirR, speed.l);
                ledcWrite(chR, 0);
                break;
        }
    }

}

String DriveAdapter::getLog() {
    return "DriveAdapter: " + speed.getLog() + direction.getLog() + pulseCnt.getLog();
}

void isr_puls_l() {
    DriveAdapter::pulseCnt.l++;
}

void isr_puls_r() {
    DriveAdapter::pulseCnt.r++;
}

