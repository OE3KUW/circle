//*****************************************************************************
// 11. Aug. 2022
//                               c i r c l e
//
//                                                               қuran wolfgang
//*****************************************************************************
// add Libraries NeoPixelBus Version 2.7
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_I2CDevice.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <FastLED.h>
#include <BluetoothSerial.h>
#include <AppAdapter.hpp>
#include <DriveAdapter.hpp>

// defines:
#define H HIGH
#define L LOW
#define NUM_LEDS 4
#define DATA_PIN 23
#define CLOCK_PIN 18

#define NUM_LEDS 4
#define DATA_PIN 23
#define CLOCK_PIN 18

#define WHEEL_L 2
#define WHEEL_R A4
#define WHEEL_L_DIRECTION A5
#define WHEEL_R_DIRECTION 15

#define OLED_RESET -1
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// Blue Tooth:
#define LEN 100
#define CR 13
#define LF 10

// global variables:

int led = 5; // on board led
int onOff;   // on board led on / off

volatile int count;

CRGB leds[NUM_LEDS];

Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire);

AppAdapter appAdapter;

DriveAdapter driveAdapter;

char text[LEN];

// SPEEDOMETER:
const uint8_t impulsL = 14;
const uint8_t impulsR = 27;

//  TIMER INTERRUPT:
hw_timer_t *timer = NULL;
volatile uint16_t tick;
volatile uint8_t tenMsec;
volatile boolean flag;


void IRAM_ATTR onTimer(void);
void impuls_R_isr(void);
void impuls_L_isr(void);

// RTOS not used in this version:

/*
// structs and types
typedef struct message MESSAGE;
struct message
{
  int x;
};
static MESSAGE m, n;


// TaskFunctions:
static void taskFuncA(void * arg)
{
  MESSAGE * myMesPointer = (MESSAGE*) arg;
  printf("\ntask function A!!! message: %d\n", myMesPointer->x);
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
  //    m.x = 7;     // for RTOS
  //    n.x = 100;   // for RTOS

  printf("start!\n- - - - - - - - - - - - - - - - - - - - - - - - - - -  -\n");



  pinMode(led, OUTPUT);
  digitalWrite(led, HIGH); // invers!  LOW = Led on!

  FastLED.addLeds<SK9822, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);

  leds[0] = CRGB{255, 255, 0};
  leds[1] = CRGB{255, 255, 0};
  leds[2] = CRGB{0, 0, 255};
  leds[3] = CRGB{0, 0, 255};

  FastLED.show();

  oled.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  // oled.display();
  oled.clearDisplay();
  oled.setTextSize(2);
  oled.setTextColor(WHITE);
  oled.setCursor(0, 0);
  oled.print("Yieppy!");
  oled.display();

  // RTOS:
  // app_cpu = xPortGetCoreID();
  // printf("app_cpu is %d\n", app_cpu);
  // xTaskCreatePinnedToCore (taskFuncA, "task Function A", 2048, &m, 1, NULL, app_cpu );  for RTOS .. ö
  // xTaskCreatePinnedToCore (taskFuncB, "task Function B", 2048, &n, 1, NULL, app_cpu );

  // BlueTooth:

  appAdapter.connect();

  // Motors:

  driveAdapter.setup(WHEEL_L, WHEEL_R, WHEEL_L_DIRECTION, WHEEL_R_DIRECTION, impulsL, impulsR);
  driveAdapter.setSpeed(60);
  count = 0;

}

void loop()
{
  char c;
  String text = ""; // mit jedem Loop Durchlauf wird der text hier zurückgesetzt.
  char respond[LEN];
  char report[LEN];

  driveAdapter.update();

  /*    if (text.startsWith("+")) */

  // Timer

  if (flag == 1)
  {
    onOff = (onOff == 0) ? 1 : 0;
    digitalWrite(led, onOff);
    flag = 0;

    // motor:  integrativer Anteil


    // Tests:
    //        sprintf(report, "s %02d d %02d setL %02d setR %02d mL %04d mR %04d aL %04d aR %04d vL %02d vR %02d",
    //                         speed, diff, setL, setR, meterL, meterR, actualL, actualR, vL, vR );
    //        printf("%s\n", report);
    //   ---
    //sprintf(respond, "vL %03d vR %03d mR %d aR %d c %d", vL, vR, meterR, actualR, count);
    count = 0;
    // ---- SerialBT.println(respond);
    // --- August 2022
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

  if (tenMsec == 100) // 100 entspricht 1 sec
  {
    // one Second :

    tenMsec = 0;
    flag = 1;
  }
}