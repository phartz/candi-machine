#ifndef main_h
#define main_h

#include <Arduino.h>
#include <Servo.h>
#include <OneButton.h>
#include <ServoCounter.h>

// Servo declaration
Servo servoRight;
Servo servoLeft;
Servo servoArrow;
uint8_t pinServoLeft = 2;
uint8_t pinServoRight = 3;
uint8_t pinServoArrow = 4;

ServoCounter servoCounter(8,9);

// Buttons
const uint8_t BTNCOIN = 7;
OneButton btnLeft(5, true);
OneButton btnRight(6, true);

// LEDs
const uint8_t LEDGREEN = 11;
const uint8_t LEDRED = 12;
const uint8_t LEDLIGHTING = 13;
const uint8_t BLINKCOUNT = 1;

// Positioning
uint8_t maxPosRight = 105;
uint8_t minPosLeft = 125; 
uint8_t maxPosArrow = 160;
int moveDelayLeft = 5;
int moveDelayRight = 5;
int moveDelayArrow = 10;
int creditCountFlashDelay = 250;

// Credit 
uint8_t creditCount = 0;
uint8_t secretCredit = 5;
uint8_t allowMultipleTimes = 3;
uint8_t secretLeftCnt = 0;
uint8_t secretRightCnt = 0;
uint8_t secretLeft = 3;
uint8_t secretRight = 3;

void moveServo(Servo &servo, uint8_t start, uint8_t end, int ticks);
// Move right servo
void moveServoRight();
// Move servo left
void moveServoLeft();
// blink LEDs
void blinkLEDs();
// Check if user has credit
bool hasCredit();
// Increment the credit count
void incCredit();
// Set LED state regarding the credit count
void setLEDState();
// Flash red LED as many credit is left
void showCreditCount();
// Decrement credit count
void decCredit();
// Deilver the left column
void deliverLeftColumn();
// Deilver the right column
void deliverRightColumn();
// Move each servo in base position
void moveServosToBasePos();
// Coin was inserted
void coinWasInserted();
// Left button was pushed
void btnLeftClick();
// Right button was pushed
void btnRightClick();
// Inititialise the candi machine
void setup();
// The main loop
void loop();

#endif