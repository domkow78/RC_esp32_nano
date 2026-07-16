#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>
#include <FastLED.h>
#include "printf.h"
#include <avr/wdt.h>

// ===================== KONFIG =====================
#define LED_PIN_FRONT 7
#define LED_PIN_LEFT  8
#define LED_PIN_RIGHT 9
#define NUM_LEDS 5
#define LED_BRIGHTNESS 120
const int steerCenter = 127;  // centralna pozycja joysticka / serwa

// ===================== LED ========================
CRGB ledsFront[NUM_LEDS];
CRGB ledsLeft[NUM_LEDS];
CRGB ledsRight[NUM_LEDS];

// ===================== RADIO ======================
RF24 radio(3, 2);
const byte address[6] = "00001";
unsigned long lastReceiveTime = 0;
unsigned long currentTime = 0;

// ===================== SERVO / ESC =================
Servo esc;
Servo servoSteer;
const int servoMin = 40;
const int servoMax = 140;
const int servoTrim = -8;

// ===================== ESC ========================
const int escStop = 1500;
const int escMinForward = 1600;
const int escMaxForward = 1640;
const int escMinBackward = 1390;
const int escMaxBackward = 1370;
const int escDeadzone = 5;
const int rampStep = 5;

// ===================== DATA =======================
struct Data_Package {
  byte j1PotX, j1PotY, j1Button;
  byte j2PotX, j2PotY, j2Button;
  byte pot1, pot2, tSwitch1, tSwitch2;
  byte button1, button2, button3, button4;
};
Data_Package data;

// ===================== SETUP ======================
void setup() {
  Serial.begin(9600);

  FastLED.addLeds<WS2812, LED_PIN_FRONT, GRB>(ledsFront, NUM_LEDS);
  FastLED.addLeds<WS2812, LED_PIN_LEFT,  GRB>(ledsLeft, NUM_LEDS);
  FastLED.addLeds<WS2812, LED_PIN_RIGHT, GRB>(ledsRight, NUM_LEDS);
  FastLED.setBrightness(LED_BRIGHTNESS);
  FastLED.clear();
  FastLED.show();

  if (!radio.begin()) while (1);
  printf_begin();
  radio.openReadingPipe(0, address);
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_LOW);
  radio.startListening();

  resetData();

  esc.attach(10);
  servoSteer.attach(4);
  servoSteer.write((servoMin + servoMax)/2 + servoTrim);

  wdt_enable(WDTO_1S);  // watchdog 1s
}

// ===================== LOOP =======================
void loop() {
  wdt_reset();
  currentTime = millis();

  if (currentTime - lastReceiveTime > 1000) resetData();
  if (radio.available()) {
    radio.read(&data, sizeof(Data_Package));
    lastReceiveTime = millis();
  }

  // -------- SERVO STEERING --------
  static int lastServoValue = (servoMin + servoMax)/2 + servoTrim;
  int steer = data.j2PotX;
  int servoValue;

  if (abs(steer - steerCenter) <= 8) servoValue = (servoMin + servoMax)/2 + servoTrim;
  else servoValue = map(steer, 0, 255, servoMax, servoMin) + servoTrim;

  servoValue = constrain(servoValue, servoMin, servoMax);
  if (servoValue != lastServoValue) {
    servoSteer.write(servoValue);
    lastServoValue = servoValue;
  }

  // -------- ESC CONTROL --------
  static int lastEscValue = escStop;
  int delta = data.j1PotY - 127;
  int targetEsc;

  static bool crawlMode = false;
  static bool speedMode = true;
  const int pot2Center = 127;
  const int pot2Deadzone = 10;

  if (data.pot2 < pot2Center - pot2Deadzone) crawlMode = true;
  else if (data.pot2 > pot2Center + pot2Deadzone) crawlMode = false;
  speedMode = !crawlMode;

  if (crawlMode) {
    const int cycleTime = 300;
    const int pulseTime = 100;
    static unsigned long lastPulse = 0;
    static bool pulseOn = false;

    if (abs(delta) > escDeadzone) {
      if (millis() - lastPulse >= cycleTime) {
        lastPulse = millis();
        pulseOn = true;
      }
      if (pulseOn) {
        if (millis() - lastPulse < pulseTime) {
          if (delta > 0) esc.writeMicroseconds(escMinForward);
          else esc.writeMicroseconds(escMinBackward);
        } else {
          esc.writeMicroseconds(escStop);
          pulseOn = false;
        }
      }
    } else {
      esc.writeMicroseconds(escStop);
      pulseOn = false;
    }

  } else {  // speed mode
    if (abs(delta) <= escDeadzone) targetEsc = escStop;
    else if (delta < 0) targetEsc = map(delta, -1, -127, escMinBackward, escMaxBackward);
    else targetEsc = map(delta, 1, 128, escMinForward, escMaxForward);

    if (lastEscValue < targetEsc) lastEscValue += rampStep;
    else if (lastEscValue > targetEsc) lastEscValue -= rampStep;
    esc.writeMicroseconds(lastEscValue);
  }

  // -------- LEDS --------
  static unsigned long lastBlink = 0;
  static bool blinkState = false;
  if (millis() - lastBlink > 400) {
    lastBlink = millis();
    blinkState = !blinkState;
  }

  if (data.tSwitch2 == 0) FastLED.clear();
  else {
    if (crawlMode) {
      fill_solid(ledsFront, NUM_LEDS, CRGB::White);
      fill_solid(ledsLeft, NUM_LEDS, CRGB::Green);
      fill_solid(ledsRight, NUM_LEDS, CRGB::Green);
    } else {
      fill_solid(ledsFront, NUM_LEDS, CRGB::White);
      fill_solid(ledsLeft, NUM_LEDS, CRGB::Red);
      fill_solid(ledsRight, NUM_LEDS, CRGB::Red);
    }

    if (steer < steerCenter - 15 && blinkState) fill_solid(ledsLeft, NUM_LEDS, CRGB::Orange);
    if (steer > steerCenter + 15 && blinkState) fill_solid(ledsRight, NUM_LEDS, CRGB::Orange);
  }
  FastLED.show();
}

// ===================== RESET ======================
void resetData() {
  data.j1PotX = 127; data.j1PotY = 127;
  data.j2PotX = 127; data.j2PotY = 127;
  data.j1Button = 1; data.j2Button = 1;
  data.pot1 = 1; data.pot2 = 1;
  data.tSwitch1 = 1; data.tSwitch2 = 1;
  data.button1 = 1; data.button2 = 1;
  data.button3 = 1; data.button4 = 1;
}
