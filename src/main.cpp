//*****************************************************************************
// my new 7.7.2022
//                               c i r c e l
//
//                                                              қuran july 2022
//*****************************************************************************
#include <Arduino.h>
#include "BluetoothSerial.h"

int led = 5;
int onOff;

// motor:

#define WHEEL_L            2 
#define WHEEL_R           A4
#define WHEEL_L_DIRECTION A5 
#define WHEEL_R_DIRECTION 15 
#define H               HIGH
#define L                LOW

volatile int speed;
volatile int diff;
volatile int setL, setR, oldSetL, oldSetR;
volatile int meterL, meterR;    // zurückgelegte Wegstrecke, wie sie errechnet wird
volatile int actualL, actualR;  // tatsächliche Wegstrecke gemessen
volatile int directionL, directionR;        // die Richtung in die gefahren werden soll
volatile int vL, vR;            // Vergleichswert für die pwm Motorsteuerung

volatile int count;

// Blue Tooth:
#define LEN 100
#define CR  13
#define LF  10
BluetoothSerial SerialBT;
char text[LEN];

// SPEEDOMETER:
const uint8_t impulsL = 14;
const uint8_t impulsR = 27;
volatile uint8_t impulsRcount;   // wheels movement control
volatile uint8_t impulsLcount;

//  TIMER INTERRUPT:
hw_timer_t * timer = NULL;
volatile uint16_t tick;
volatile uint8_t tenMsec;
volatile boolean flag; 

// Prototypes
void IRAM_ATTR onTimer(void);
void impuls_R_isr(void);
void impuls_L_isr(void);

// structs and types
typedef struct message MESSAGE;
struct message
{
  int x;
};
static MESSAGE m, n;

// RTOS not used in this version:
/*
// TaskFunctions:
static void taskFuncA(void * arg)
{
  MESSAGE * myMesPointer = (MESSAGE*) arg;
  printf("\ntask function A! message: %d\n", myMesPointer->x);
  for(;;)
  {
    printf("a");
    delay(500);
  }
}

static void taskFuncB(void * arg)
{
  MESSAGE * myMesPointer = (MESSAGE*) arg;
  printf("\ntask function B! message: %d\n", myMesPointer->x);
  for(;;)
  {
    printf("b");
    delay(1000);
  }
}
*/

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
void setup() 
{
//  int app_cpu = 0;  for RTOS
    m.x = 7;     // for RTOS
    n.x = 100;   // for RTOS

    delay(500); // pause für serial setup...
    printf("start!\n- - - - - - - - - - - - - - - - - - - - - - - - - - -  -\n");
  

    pinMode(led, OUTPUT);
    digitalWrite(led, HIGH); // invers!  LOW = Led on!

  // RTOS:
  // app_cpu = xPortGetCoreID();
  // printf("app_cpu is %d\n", app_cpu);
  // xTaskCreatePinnedToCore (taskFuncA, "task Function A", 2048, &m, 1, NULL, app_cpu );  for RTOS .. ö 
  // xTaskCreatePinnedToCore (taskFuncB, "task Function B", 2048, &n, 1, NULL, app_cpu );

  // BlueTooth:

    SerialBT.begin("LoLin32_220714");
    printf("bluetooth startet\n");
    
    // motor:
    speed = diff = 0;
    oldSetL = oldSetR = setL = setR = 0;
    meterL = meterR = 0;    
    actualL = actualR = 0;
    vL = vR = 0;

    pinMode(WHEEL_L, OUTPUT);
    pinMode(WHEEL_R, OUTPUT);
    pinMode(WHEEL_L_DIRECTION, OUTPUT);
    pinMode(WHEEL_R_DIRECTION, OUTPUT);

    digitalWrite(WHEEL_L, L);   // stop !
    digitalWrite(WHEEL_R, L);  // stop !
    digitalWrite(WHEEL_L_DIRECTION, L); directionL = +1;
    digitalWrite(WHEEL_R_DIRECTION, H); directionR = +1;

  // SPEEDOMETER:

    count = 0;

    impulsLcount = impulsRcount = 0;  // can später entfernt werden

    attachInterrupt(digitalPinToInterrupt(impulsR), impuls_R_isr, FALLING);
    attachInterrupt(digitalPinToInterrupt(impulsL), impuls_L_isr, FALLING);
  
  // TIMER INTERRUPT:

    timer = timerBegin(0, 80, true);
    timerAttachInterrupt(timer, &onTimer, true);
    timerAlarmWrite(timer, 10, true); // 10 µsec
    timerAlarmEnable(timer);
}

void loop() 
{
    char c;
    String text = "";   // mit jedem Loop Durchlauf wird der text hier zurückgesetzt.
    char respond[LEN];  
    char report[LEN];

    // BlueTooth:

    while (SerialBT.available())
    {
       c = SerialBT.read();
       text += c;
    }

    if (text.startsWith("ok"))
    {
        SerialBT.println("yes!");
    }

    if (text.startsWith("start"))
    {
        SerialBT.println("we start now!");
        speed = 60;
        diff = 0;
        setL = setR = 40;
        // Richtung bleibt derzeit noch weg.... 
    }

/*

    if (text.startsWith("+"))
    {
        val++;
        sprintf(respond,"val %d", val);
        SerialBT.println(respond);
    }

    if (text.startsWith("-"))
    {
        val--;
        sprintf(respond,"val %d", val);
        SerialBT.println(respond);
    }

    if (text.startsWith("d"))
    {
        sprintf(respond,"direction up");
        digitalWrite(WHEEL_RIGHT_DIRECTION, LOW);        
        SerialBT.println(respond);
    }

    if (text.startsWith("D"))
    {
        sprintf(respond,"direction down");
        digitalWrite(WHEEL_RIGHT_DIRECTION, HIGH);        
        SerialBT.println(respond);
    }

*/

   // Timer
   if (flag == 1)
   {
        onOff = (onOff == 0) ? 1 : 0;
        digitalWrite(led, onOff);
        flag = 0;

        // motor:

        meterL += setL; meterR += setR;

        if (actualL > meterL) actualL = meterL - 40;
        vL = setL + (setL - oldSetL) +  (meterL - actualL);
        if (vL > 255) vL = 255;
        if (vL < 0)  vL = -1; // sollte 0 sein - zum debuggen besser... 
        oldSetL = setL;

        if (actualR > meterR) actualR = meterR - 40;
        vR = setR + (setR - oldSetR) +  (meterR - actualR);
        if (vR > 255) vR = 255;
        if (vR < 0)  vR = -1;
        oldSetR = setR;


        sprintf(report, "s %02d d %02d setL %02d setR %02d mL %04d mR %04d aL %04d aR %04d vL %02d vR %02d",
                         speed, diff, setL, setR, meterL, meterR, actualL, actualR, vL, vR );

        printf("%s\n", report);

        sprintf(respond, "vL %03d vR %03d mL %d aL %d c %d", vL, vR, meterL, actualR, count);
        count = 0;
        SerialBT.println(respond);

   }

}

//****************************************************************
// ISR Speedometer:
//****************************************************************

void impuls_L_isr(void)
{
  impulsLcount += directionL;
  actualL += directionL;
  count++;

  //vL = meterL - actualL;
  //if (vL > 255) vL = 255;
  //if (vL < 0)  vL = 0;
}

void impuls_R_isr(void)
{
  impulsRcount += directionR;
  actualR += directionR;

  //vR = meterR - actualR;
  //if (vR > 255) vR = 255;
  //if (vR < 0)  vR = 0;

}




//****************************************************************
//****************************************************************

void IRAM_ATTR onTimer(void)
{
    tick++;

    if ((tick & 0x3ff) == 0)
    {
        tenMsec++;
    }

    if (tenMsec == 100)  // 100 entspricht 1 sec
    {
        // one Second :  
        
        tenMsec = 0; 
        flag = 1; 
        
    }


    if ((tick & 0xff) > vL) digitalWrite(WHEEL_L, L); else digitalWrite(WHEEL_L, H);
    if ((tick & 0xff) > vR) digitalWrite(WHEEL_R, L); else digitalWrite(WHEEL_R, H);

    
}