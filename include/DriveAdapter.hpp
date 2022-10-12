#ifndef DRIVE_ADAPTER
#define DRIVE_ADAPTER
#include <Arduino.h>

#define WheelPinL 2
#define WheelPinR 32
#define WheelDirPinL 33
#define WheelDirPinR 15

#define ImpulsePinL 14
#define ImpulsePinR 27

enum Mode {
    Integrated_ic, External_ic
};

class direction_t {
public:
    int l;
    int r;

public:
    direction_t(){}
    direction_t(int, int);
    void selfcp(direction_t&);
    bool operator== (direction_t&);
    direction_t operator+ (direction_t&);
};

void _isr_puls_l();
void _isr_puls_r();

class DriveAdapter
{
private:
    int __speed;
    int __diff;
    static uint8_t pinL;
    static uint8_t pinR;
    static uint8_t pinDirL;
    static uint8_t pinDirR;
    static direction_t vDist, Dist;
    direction_t speed, speed_old;
    static direction_t amount, amount_old;
    static direction_t direction, direction_old;
    static direction_t diff, comp;
    hw_timer_t *timer;
    static int tick;
    static Mode mode;

private:
    static void onTimer();
    static void onUpdate();

public:
    DriveAdapter();
    DriveAdapter(Mode);
    ~DriveAdapter();

    void setup(Mode);
    void setSpeed(int);
    void setSpeed(int, int);
    void setDiff(int);
    void setMode(Mode);
    int getSpeed();
    int getDiff();
    void update();

    friend void _isr_puls_l();
    friend void _isr_puls_r();

};

#endif