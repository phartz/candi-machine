#include <Servo.h>

/*
 * Zero: 
 *   - LED Green  
 *   - LED Red
 *   - Servo Left
 *   - Servo Right
 *   - Servo Arrow
 *   - Taster Left
 *   - Taster Reft
 *   - Taster Coin
 *   
 * Positiv: 
 *   - Servo Left
 *   - Servo Right
 *   - Servo Coin
 */

// Servo declaration
Servo servoRight;
Servo servoLeft;
Servo servoArrow;

// Buttons
const int btnLeft = 5;
const int btnRight = 6;
const int btnCoin = 7;

// LEDs
const int ledGreen = 11;
const int ledRed = 12;

// Positioning
int maxPosRight = 105;
int minPosLeft = 125; 
int maxPosArrow = 160;
int moveDelay = 10;

// Credit 
int creditCount = 0;

// Inititialise the candi machine
void setup() {
  servoLeft.attach(2);
  servoRight.attach(3); 
  servoArrow.attach(4);
  pinMode(btnLeft, INPUT_PULLUP);
  pinMode(btnRight, INPUT_PULLUP);
  pinMode(btnCoin, INPUT_PULLUP);
  pinMode(ledGreen, OUTPUT);
  pinMode(ledRed, OUTPUT);
}

// Move right servo
void moveServoRight() {
  for (int i = 0; i < maxPosRight; i++) {
    servoRight.write(i);
    delay(moveDelay);
  }
  for (int i = maxPosRight; i >= 0; i--) {
    servoRight.write(i);
    delay(moveDelay);
  }  
}

// Move servo left
void moveServoLeft() {
  for (int i = minPosLeft; i >= 0; i--) {
    servoLeft.write(i);
    delay(moveDelay);
  }  
  for (int i = 0; i < minPosLeft; i++) {
    servoLeft.write(i);
    delay(moveDelay);
  }
}

// Point arrow to right
void moveArrowToRight() {
  for (int i = maxPosArrow / 2; i <= maxPosArrow; i++){
    servoArrow.write(i);
    delay(moveDelay);    
  }
}

// Move arrow from right to center
void moveArrowFromRight(){
  for (int i = maxPosArrow; i >= maxPosArrow / 2; i--){
    servoArrow.write(i);
    delay(moveDelay);    
  }
  
}

// Move arrow to left
void moveArrowToLeft() {
  for (int i = maxPosArrow / 2; i > 0; i--){
    servoArrow.write(i);
    delay(moveDelay);    
  }
}

// Move arrow from left to center
void moveArrowFromLeft() {
  for (int i = 0; i <= maxPosArrow / 2; i++){
    servoArrow.write(i);
    delay(moveDelay);    
  }
}

// blink LEDs
void blinkLEDs(){
  for (int i = 0; i < 5; i++){
    digitalWrite(ledGreen, HIGH);
    digitalWrite(ledRed, LOW);
    delay(200);
    digitalWrite(ledGreen, LOW);
    digitalWrite(ledRed, HIGH);
    delay(200);
  }
}

// Check if user has credit
bool checkCredit() {
  return creditCount > 0;  
}

// Increment the credit count
void incCredit() {
  creditCount++;
  delay(200);
  digitalWrite(ledGreen, HIGH); 
  delay(100);
  digitalWrite(ledRed, LOW); 
}

// Flash red LED as many credit is left
void setLEDState(){  
  if (creditCount > 0) {
    digitalWrite(ledGreen, HIGH);
    digitalWrite(ledRed, LOW);
  } else{
    digitalWrite(ledGreen, LOW);
    digitalWrite(ledRed, HIGH);
  }
}

// Flash red LED as many credit is left
void showCreditCount(){  
  setLEDState();
  for (int i = 0; i < creditCount; i++){
    delay(500);
    digitalWrite(ledRed, HIGH);    
    delay(500);
    digitalWrite(ledRed, LOW);    
  }  
}

// Decrement credit count
void decCredit() {  
  if (creditCount > 0) {
    creditCount--;
  }
}

// Deilver the left column
void deliverLeftColumn(){
  delay(200);
  blinkLEDs();
  moveArrowToLeft();  
  moveServoLeft();
  moveArrowFromLeft();
  delay(200);
  digitalWrite(ledGreen, HIGH);
  digitalWrite(ledRed, LOW);
  delay(200);
  digitalWrite(ledGreen, LOW);  
  decCredit();
  showCreditCount();  
}

// Deilver the right column
void deliverRightColumn(){
  delay(200);     
  blinkLEDs();
  moveArrowToRight();
  moveServoRight();  
  moveArrowFromRight();
  delay(200);
  digitalWrite(ledGreen, HIGH);
  digitalWrite(ledRed, LOW);
  delay(200);
  digitalWrite(ledGreen, LOW);      
  decCredit();
  showCreditCount();  
}

// Move each servo in base position
void moveServoToBasePos(){
  servoLeft.write(minPosLeft);
  servoRight.write(0);
  servoArrow.write(maxPosArrow / 2);    
}

// Coin was inserted
void coinWasInserted(){
  incCredit();
  showCreditCount();  
}

// The main loop
void loop() {
  moveServoToBasePos();
  setLEDState();

  // Check wether one button was pushed
  int btnRightRead = digitalRead(btnRight);
  int btnLeftRead = digitalRead(btnLeft);
  int btnCoinRead = digitalRead(btnCoin);

  // If coin was inserted, turn LEDs smothly on
  if (btnCoinRead == LOW){
    coinWasInserted();
  }

  // Left button was pushed
  if (checkCredit() && btnLeftRead == LOW) {
    deliverLeftColumn();
  }

  // Right button was pushed
  if (checkCredit() && btnRightRead == LOW) {
    deliverRightColumn();
  }
}
