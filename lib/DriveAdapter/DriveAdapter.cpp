#include "DriveAdapter.hpp"

char DriveAdapter::pinL = 0;
char DriveAdapter::pinR = 0;
char DriveAdapter::pinDirL = 0;
char DriveAdapter::pinDirR = 0;
direction_t DriveAdapter::vDist = direction_t();
direction_t DriveAdapter::Dist = direction_t();
direction_t DriveAdapter::amount = direction_t();
direction_t DriveAdapter::amount_old = direction_t();
direction_t DriveAdapter::direction = direction_t();
direction_t DriveAdapter::direction_old = direction_t();
direction_t DriveAdapter::diff = direction_t();
direction_t DriveAdapter::comp = direction_t();
int DriveAdapter::tick = 0;

direction_t::direction_t(int _l, int _r) {
    l = _l;
    r = _r;
}

direction_t direction_t::operator+(direction_t& d) {
  return direction_t(l+d.l, r+d.r);
}

bool direction_t::operator==(direction_t& d) {
  return (l == d.l && r == d.r);
}

void direction_t::selfcp(direction_t& source) {
  l = source.l;
  r = source.r;
}

DriveAdapter::DriveAdapter() {
    __speed = 0;
    __diff = 0;
    speed_old = direction_t(0, 0);
    amount_old = direction_t(0, 0);
    direction_old = direction_t(0, 0);
    Dist = direction_t(0, 0);
    vDist = direction_t(0, 0);
    tick = 0;
    pinL = 0;
    pinR = 0;
}

DriveAdapter::DriveAdapter(char motor_pin_l, char motor_pin_r, char motor_dir_l, char motor_dir_r, char impulse_pin_l, char impulse_pin_r) {
    speed_old = direction_t(0, 0);
    amount_old = direction_t(0, 0);
    direction_old = direction_t(0, 0);
    Dist = direction_t(0, 0);
    vDist = direction_t(0, 0);
    tick = 0;
    setup(motor_pin_l, motor_pin_r, motor_dir_l, motor_dir_r, impulse_pin_l, impulse_pin_r);
}

void DriveAdapter::setup(char motor_pin_l, char motor_pin_r, char motor_dir_l, char motor_dir_r, char impulse_pin_l, char impulse_pin_r)
{
    pinL = motor_pin_l;
    pinR = motor_pin_r;
    pinDirL = motor_dir_l;
    pinDirR = motor_dir_r;

    pinMode(pinL, OUTPUT);
    pinMode(pinR, OUTPUT);
    pinMode(pinDirL, OUTPUT);
    pinMode(pinDirR, OUTPUT);
    pinMode(impulse_pin_l, INPUT);
    pinMode(impulse_pin_r, OUTPUT);

    attachInterrupt(digitalPinToInterrupt(impulse_pin_r), _isr_puls_r, FALLING);
    attachInterrupt(digitalPinToInterrupt(impulse_pin_l), _isr_puls_l, FALLING);

    timer = timerBegin(0, 80, true);
    timerAttachInterrupt(timer, &onTimer, true);
    timerAlarmWrite(timer, 10, true);
    timerAttachInterrupt(timer, &onUpdate, true);
    timerAlarmWrite(timer, 1000000, true);
    timerAlarmEnable(timer);
}

DriveAdapter::~DriveAdapter()
{
}

void DriveAdapter::setSpeed(int v) {
    __speed = v;
}
int DriveAdapter::getSpeed() {
    return __speed;
}

void DriveAdapter::setDiff(int v) {
    __diff = v;
}

int DriveAdapter::getDiff() {
    return __diff;
}

void DriveAdapter::update() {
  speed.l = __speed - __diff;
  speed.r = __speed - __diff;

  if(speed.l >= 0) { // <-simplify?
    amount.l = speed.l;
    digitalWrite(pinDirL, HIGH);
    direction.l = 1;
  } else {
    amount.l = -speed.l;
    digitalWrite(pinDirL, LOW);
    direction.l = -1;
  }
  if(speed.r >= 0) {
    amount.r = speed.r;
    digitalWrite(pinDirR, HIGH);
    direction.r = 1;
  } else {
    amount.r = -speed.r;
    digitalWrite(pinDirR, LOW);
    direction.r = -1;
  }

  if ((direction.l != direction_old.l) || (speed.l == 0))
  {
    Dist.l = 0;
    vDist.l = 0;
  }

  if ((direction.r != direction_old.r) || (speed.r == 0))
  {
    Dist.r = 0;
    vDist.r = 0;
  }
  //  Proportionalanteil:

  comp.l = amount.l;
  comp.r = amount.r;

  // differentieller Anteil:

  diff.l = 2 * (amount.l - amount_old.l);
  diff.r = 2 * (amount.r - amount_old.r);

  comp.l += diff.l;
  comp.r = diff.r;

  amount_old = amount;
}

void DriveAdapter::onTimer() {
  tick++;

  /*if (!(tick & 0x3ff)) <- 4what use???
  {
    tenMsec++;
  }*/

  if (((tick << 2) & 0xff) > comp.l)
    digitalWrite(pinR, LOW);
  else
    digitalWrite(pinL, HIGH);
  if (((tick << 2) & 0xff) > comp.r)
    digitalWrite(pinR, LOW);
  else
    digitalWrite(pinL, HIGH);

}

void DriveAdapter::onUpdate() {
    Dist.l += amount.l;
    Dist.r += amount.r;

    if (amount.l == 0)
      comp.l = 0;
    else
    {
      diff.l >>= 1; // differentieller Anteil halbiert sich
      if (Dist.l > vDist.l)
        Dist.l = vDist.l - 40;
      comp.l = diff.l + amount.l + vDist.l - Dist.l;
    }
    if (comp.l > 255)
      comp.l = 255;
    if (comp.l < 0)
      comp.l = 0; // sollte 0 sein - zum debuggen besser...

    if (amount.r == 0)
      comp.r = 0;
    else
    {
      diff.r >>= 1; // differentieller Anteil halbiert sich
      if (Dist.r > vDist.r)
        Dist.r = vDist.r - 40;
      comp.r = diff.r + amount.r + Dist.r - vDist.r;
    }
    if (comp.r > 255)
      comp.r = 255;
    if (comp.r < 0)
      comp.r = 0;
}


void _isr_puls_l() {
    DriveAdapter::vDist.l++;
}

void _isr_puls_r() {
    DriveAdapter::vDist.r++;
}