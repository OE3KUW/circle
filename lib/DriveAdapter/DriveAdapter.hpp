#ifndef DRIVE_ADAPTER
#define DRIVE_ADAPTER
#include <Arduino.h>

class direction_t { // to class?
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
    static char pinL;
    static char pinR;
    static char pinDirL;
    static char pinDirR;
    static direction_t vDist, Dist;
    direction_t speed, speed_old;
    static direction_t amount, amount_old;
    static direction_t direction, direction_old;
    static direction_t diff, comp;
    hw_timer_t *timer;
    static int tick;

private:
    static void onTimer();
    static void onUpdate();

public:
    DriveAdapter();
    DriveAdapter(char, char, char, char, char, char);
    ~DriveAdapter();

    void setup(char, char, char, char, char, char);
    void setSpeed(int);
    void setDiff(int);
    int getSpeed();
    int getDiff();
    void update();

    friend void _isr_puls_l();
    friend void _isr_puls_r();

};

#endif