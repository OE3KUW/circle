//*****************************************************************************
// 11. Aug. 2022
//                               c i r c l e
//
//                                                               қuran wolfgang
//*****************************************************************************
// add Libraries NeoPixelBus Version 2.7 <-- Todo oder woffuer?
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_I2CDevice.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <FastLED.h>
#include <BluetoothSerial.h>
#include <DriveAdapter.hpp>
#include <AppAdapter.hpp>

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

volatile int speed;
volatile int diff;
volatile int setL, setR, oldSetL, oldSetR;
volatile int amountL, amountR;
volatile int oAmountL, oAmountR;
volatile int meterL, meterR;                       // zurückgelegte Wegstrecke, wie sie errechnet wird
volatile int actualL, actualR;                     // tatsächliche Wegstrecke, gemessen
volatile int directionL, directionR, oDirL, oDirR; // die Richtung in die gefahren werden soll
volatile int diffL, diffR;
volatile int vL, vR; // Vergleichswert für die pwm Motorsteuerung

volatile int count;

CRGB leds[NUM_LEDS];

Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire);

BluetoothSerial SerialBT;

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

// Prototypes
void driveControl(void);
void IRAM_ATTR onTimer(void);
void impuls_R_isr(void);
void impuls_L_isr(void);

void setup()
{
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

  SerialBT.begin("LoLin32_220808");
  printf("bluetooth startet\n");

  // motor:
  speed = diff = 0;
  oldSetL = oldSetR = setL = setR = 0;
  amountL = amountR = 0;
  oAmountL = oAmountR = 0;
  meterL = meterR = 0;
  actualL = actualR = 0;
  diffL = diffR = 0;
  vL = vR = 0;

  driveAdapter.setup(Integrated_ic);
  driveAdapter.setSpeed(30);
  driveAdapter.setDiff(-30);
  count = 0;

  pinMode(impulsL, INPUT);
  pinMode(impulsR, INPUT);

  attachInterrupt(digitalPinToInterrupt(impulsR), impuls_R_isr, FALLING);
  attachInterrupt(digitalPinToInterrupt(impulsL), impuls_L_isr, FALLING);

  // TIMER INTERRUPT:

  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, 10, true);
  timerAlarmEnable(timer);
}

void loop()
{
  if (flag == 1)
  {
    onOff = (onOff == 0) ? 1 : 0;
    digitalWrite(led, onOff);
  }
}
