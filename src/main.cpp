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

// Blue Tooth:
#define LEN 20
#define CR  13
#define LF  10
BluetoothSerial SerialBT;
char text[LEN];

//  TIMER INTERRUPT:
hw_timer_t * timer = NULL;
volatile uint16_t tick;
volatile uint8_t tenMsec;
volatile boolean flag; 

// Prototypes
void IRAM_ATTR onTimer(void);

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
  m.x = 7;
  n.x = 100;

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

  SerialBT.begin("LoLin32July22");
  printf("bluetooth startet\n");
  
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

    // BlueTooth:

    while (SerialBT.available())
    {
       c = SerialBT.read();
       text += c;
    }
    printf("%s",text.c_str());

    if (text.startsWith("ok"))
    {
      SerialBT.println("yes!");
    }


   // Timer
   if (flag == 1)
   {
        onOff = (onOff == 0) ? 1 : 0;
        digitalWrite(led, onOff);
        flag = 0;
   }


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
}