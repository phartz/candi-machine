#include <Servo.h>
#include <Arduino.h>
#include <OneButton.h>
#include <ServoCounter.h>

// Servo declaration
Servo servoRight;
Servo servoLeft;
Servo servoArrow;

ServoCounter servoCounter(8,9);

// Buttons
const int BTNCOIN = 7;
OneButton btnLeft(5, true);
OneButton btnRight(6, true);

// LEDs
const int LEDGREEN = 11;
const int LEDRED = 12;
const int LEDLIGHTING = 13;

// Positioning
int maxPosRight = 105;
int minPosLeft = 125; 
int maxPosArrow = 160;
int moveDelayLeft = 5;
int moveDelayRight = 5;
int moveDelayArrow = 10;
int creditCountFlashDelay = 250;

// Credit 
int creditCount = 0;
int secretCredit = 3;
int secretLeftCnt = 0;
int secretRightCnt = 0;
int secretLeft = 3;
int secretRight = 3;

// Move right servo
void moveServoRight() {
  for (int i = 0; i < maxPosRight; i++) {
    servoRight.write(i);
    delay(moveDelayRight);
  }
  // Move back fast
  servoRight.write(0);
}

// Move servo left
void moveServoLeft() {
  for (int i = minPosLeft; i >= 0; i--) {
    servoLeft.write(i);
    delay(moveDelayLeft);
  }  
  // Move back fast
  servoLeft.write(minPosLeft);
}

// Point arrow to right
void moveArrowToRight() {
  for (int i = maxPosArrow / 2; i <= maxPosArrow; i++){
    servoArrow.write(i);
    delay(moveDelayArrow);    
  }
}

// Move arrow from right to center
void moveArrowFromRight(){
  for (int i = maxPosArrow; i >= maxPosArrow / 2; i--){
    servoArrow.write(i);
    delay(moveDelayArrow);    
  }
  
}

// Move arrow to left
void moveArrowToLeft() {
  for (int i = maxPosArrow / 2; i > 0; i--){
    servoArrow.write(i);
    delay(moveDelayArrow);    
  }
}

// Move arrow from left to center
void moveArrowFromLeft() {
  for (int i = 0; i <= maxPosArrow / 2; i++){
    servoArrow.write(i);
    delay(moveDelayArrow);    
  }
}

// blink LEDs
void blinkLEDs(){
  for (int i = 0; i < 2; i++){
    digitalWrite(LEDGREEN, HIGH);
    digitalWrite(LEDRED, LOW);
    delay(200);
    digitalWrite(LEDGREEN, LOW);
    digitalWrite(LEDRED, HIGH);
    delay(200);
  }
}

// Check if user has credit
bool hasCredit() {
  return creditCount > 0;  
}

// Increment the credit count
void incCredit() {
  creditCount++;
}

// Set LED state regarding the credit count
void setLEDState(){  
  digitalWrite(LEDGREEN, hasCredit() ? HIGH : LOW);
  digitalWrite(LEDRED, hasCredit() ? LOW : HIGH);
}

// Flash red LED as many credit is left
void showCreditCount(){  
  servoCounter.Set(creditCount);
  setLEDState();
}

// Decrement credit count
void decCredit() {  
  creditCount = hasCredit() ? creditCount - 1 : 0;
}

// Deilver the left column
void deliverLeftColumn(){
  delay(100);
  blinkLEDs();
  moveArrowToLeft();  
  moveServoLeft();
  moveArrowFromLeft();
  decCredit();
  showCreditCount();  
}

// Deilver the right column
void deliverRightColumn(){
  delay(100);     
  blinkLEDs();
  moveArrowToRight();
  moveServoRight();  
  moveArrowFromRight();     
  decCredit();
  showCreditCount();  
}

// Move each servo in base position
void moveServosToBasePos(){
  servoLeft.write(minPosLeft);
  servoRight.write(0);
  servoArrow.write(maxPosArrow / 2);    
}

// Coin was inserted
void coinWasInserted(){
  incCredit();
  showCreditCount();  
}

// Check if coin was inserted
bool checkBtnCoin(){
  return digitalRead(BTNCOIN) == LOW;
}

// Left button was pushed
void btnLeftClick(){
  if (hasCredit()) {
    deliverLeftColumn();
    return;
  }

  if (secretRightCnt == secretRight && secretLeftCnt == secretLeft){
    for (int i = 0; i < secretCredit; i++)
    {
      incCredit();
    }
    showCreditCount();  
    secretCredit = 0; 
  }

  if (secretRightCnt > 0 || ++secretLeftCnt > secretLeft){
    secretLeftCnt = secretRightCnt = 0;
  }
}

// Right button was pushed
void btnRightClick() {
  if (hasCredit()) {
    deliverRightColumn();
    return;
  }

  if (secretLeftCnt < secretLeft || ++secretRightCnt > secretRight){
    secretLeftCnt = secretRightCnt = 0;
  }
}

// Inititialise the candi machine
void setup() {
  servoLeft.attach(2);
  servoRight.attach(3); 
  servoArrow.attach(4);
  pinMode(BTNCOIN, INPUT_PULLUP);
  pinMode(LEDGREEN, OUTPUT);
  pinMode(LEDRED, OUTPUT);
  pinMode(LEDLIGHTING, OUTPUT);
  digitalWrite(LEDLIGHTING, HIGH);

  // attach buttons
  btnLeft.attachClick(btnLeftClick);
  btnRight.attachClick(btnRightClick);
  
  servoCounter.Init();
  Serial.begin(9600);
}

// The main loop
void loop() {
  moveServosToBasePos();
  setLEDState();

  // Check wether one button was pushed
  btnLeft.tick();
  btnRight.tick();

  // Check Button Coin
  if (checkBtnCoin()){
    coinWasInserted();
  }
}
