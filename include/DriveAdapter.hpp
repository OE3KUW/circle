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
    bool operator== (direction_t&) const;
    direction_t operator+ (direction_t&);
    direction_t getUnit() const;
    String getLog();
};


void isr_puls_l();
void isr_puls_r();

class DriveAdapter
{
private:
    int _speed;
    int _diff{};
    static uint8_t pinL;
    static uint8_t pinR;
    static uint8_t pinDirL;
    static uint8_t pinDirR;
    direction_t speed;
    direction_t direction;
    static direction_t pulseCnt;
    hw_timer_t *timer{};
    static Mode mode;

public:
    DriveAdapter();
    explicit DriveAdapter(Mode);
    ~DriveAdapter();

    void setup(Mode);
    void setSpeed(int);
    void setSpeed(int, int);
    void setDiff(int);
    void update();
    String getLog();

    friend void isr_puls_l();
    friend void isr_puls_r();

};

#endif