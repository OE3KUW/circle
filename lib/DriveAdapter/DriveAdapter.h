#ifndef DRIVE_ADAPTER
#define DRIVE_ADAPTER

#include <Arduino.h>
#include "direction_t.h"

#define WHEEL_PIN_L 2
#define WHEEL_PIN_R 32
#define WHEEL_DIR_PIN_L 33
#define WHEEL_DIR_PIN_R 15

#define MOTOR_LEFT_FORWARD 2
#define MOTOR_LEFT_BACKWARD 15
#define MOTOR_RIGHT_FORWARD 33
#define MOTOR_RIGHT_BACKWARD 32

#define ImpulsePinL 14
#define ImpulsePinR 27

enum Mode {
    Integrated_ic, External_ic
};

void isr_puls_l();
void isr_puls_r();

class DriveAdapter
{
private:
    int _speed;
    int _diff;
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
    void setSpeed(direction_t);
    void setDiff(int);
    void update();
    String getLog();

    friend void isr_puls_l();
    friend void isr_puls_r();

};

#endif