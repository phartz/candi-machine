#include <Servo.h>

// Servo declaration
Servo servoRight;
Servo servoLeft;
Servo servoArrow;

// Buttons
const int BTNLEFT = 5;
const int BTNRIGHT = 6;
const int BTNCOIN = 7;

// LEDs
const int LEDGREEN = 11;
const int LEDRED = 12;
const int LEDLIGHTNING = 13;

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

// Inititialise the candi machine
void setup() {
  servoLeft.attach(2);
  servoRight.attach(3); 
  servoArrow.attach(4);
  pinMode(BTNLEFT, INPUT_PULLUP);
  pinMode(BTNRIGHT, INPUT_PULLUP);
  pinMode(BTNCOIN, INPUT_PULLUP);
  pinMode(LEDGREEN, OUTPUT);
  pinMode(LEDRED, OUTPUT);
  pinMode(LEDLIGHTNING, OUTPUT);
  digitalWrite(LEDLIGHTNING, HIGH);
}

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
bool checkCredit() {
  return creditCount > 0;  
}

// Increment the credit count
void incCredit() {
  creditCount++;
}

// Flash red LED as many credit is left
void setLEDState(){  
  if (creditCount > 0) {
    digitalWrite(LEDGREEN, HIGH);
    digitalWrite(LEDRED, LOW);
  } else{
    digitalWrite(LEDGREEN, LOW);
    digitalWrite(LEDRED, HIGH);
  }
}

// Flash red LED as many credit is left
void showCreditCount(){  
  setLEDState();
  for (int i = 0; i < creditCount; i++){
    delay(creditCountFlashDelay);
    digitalWrite(LEDRED, HIGH);    
    delay(creditCountFlashDelay);
    digitalWrite(LEDRED, LOW);    
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

// Check if coin was inserted
bool checkBtnCoin(){
  return digitalRead(BTNCOIN) == LOW;
}

// The main loop
void loop() {
  moveServoToBasePos();
  setLEDState();

  // Check wether one button was pushed
  int btnRightRead = digitalRead(BTNRIGHT);
  int btnLeftRead = digitalRead(BTNLEFT);

  // Check Button Coin
  if (checkBtnCoin()){
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
