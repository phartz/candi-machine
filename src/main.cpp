#include <main.h>

// Move the servo
void moveServo(Servo &servo, uint8_t start, uint8_t end, int ticks){
  // Caluclate the direction
  uint8_t step = start < end ? 1 : -1;
  uint8_t pos = start;

  while (pos != end)
  {
    pos += step;
    servo.write(pos);
    delay(ticks);
  }
}

// Move right servo
void moveServoRight() {
  moveServo(servoRight, 0, maxPosRight, moveDelayRight);
  // Move back fast
  servoRight.write(0);
}

// Move servo left
void moveServoLeft() {
  moveServo(servoLeft, minPosLeft, 0, moveDelayLeft);
  // Move back fast
  servoLeft.write(minPosLeft);
}

// blink LEDs
void blinkLEDs(){
  for (uint8_t i = 0; i < BLINKCOUNT; i++){
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
}

// Decrement credit count
void decCredit() {  
  creditCount = hasCredit() ? creditCount - 1 : 0;
}

// Deilver the left column
void deliverLeftColumn(){
  delay(100);
  blinkLEDs();
  moveServo(servoArrow, maxPosArrow / 2, 0, moveDelayArrow);
  moveServoLeft();
  moveServo(servoArrow, 0, maxPosArrow / 2, moveDelayArrow);
  decCredit();
  showCreditCount();  
}

// Deilver the right column
void deliverRightColumn(){
  delay(100);     
  blinkLEDs();
  moveServo(servoArrow, maxPosArrow / 2, maxPosArrow, moveDelayArrow);
  moveServoRight();  
  moveServo(servoArrow, maxPosArrow, maxPosArrow / 2, moveDelayArrow); 
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
    if (allowMultipleTimes-- == 0) {
      secretCredit = 0; 
    }
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
  servoLeft.attach(pinServoLeft);
  servoRight.attach(pinServoRight); 
  servoArrow.attach(pinServoArrow);
  pinMode(BTNCOIN, INPUT_PULLUP);
  pinMode(LEDGREEN, OUTPUT);
  pinMode(LEDRED, OUTPUT);
  pinMode(LEDLIGHTING, OUTPUT);
  digitalWrite(LEDLIGHTING, HIGH);

  // attach buttons
  btnLeft.attachClick(btnLeftClick);
  btnRight.attachClick(btnRightClick);
  
  servoCounter.Init();
}

// The main loop
void loop() {
  moveServosToBasePos();
  setLEDState();

  // Check wether one button was pushed
  btnLeft.tick();
  btnRight.tick();

  // Check Button Coin
  if (digitalRead(BTNCOIN) == LOW){
    coinWasInserted();
  }
}
