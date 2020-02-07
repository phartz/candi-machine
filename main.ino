#include <Servo.h>

// Column 1
const int Col1LedGreen = 2;
const int Col1LedRed = 3;
const int Col1Button = 4;

// Colum 2
const int Col2LedGreen = 5;
const int Col2LedRed = 6;
const int Col2Button = 7;

// Coin Button
const int CoinButton = 8;

// Servo 1
Servo Col1Servo;
int Col1ServoEndPos = 90; // End position

// Servo 2
Servo Col2Servo;
int Col2ServoEndPos = 90; // End position

int ServoMoveDelay = 50; // ms between each step of the servo
int ServoWaitDelay = 2000; // Wait time before servo moves back in position 0

// Credit variable
bool HasCredit = false;

// Setup function, just executed once
void setup() {
  // Initialize all LEDs
  pinMode(Col1LedGreen, OUTPUT);
  pinMode(Col1LedRed, OUTPUT);
  pinMode(Col2LedGreen, OUTPUT);
  pinMode(Col2LedRed, OUTPUT);

  // Initialize all Button
  pinMode(Col1Button, INPUT_PULLUP);
  pinMode(Col2Button, INPUT_PULLUP);
  pinMode(CoinButton, INPUT_PULLUP);

  // Initialize Servos
  Col1Servo.attach(11);
  Col2Servo.attach(12);

  // Move servo to position 0
  Col1Servo.write(0);
  Col2Servo.write(0);  
}

// Function to move the servos
void moveServo(Servo servo, int endPos) {
  // Move forward
  for (int i = 0; i < endPos; i++) {
    servo.write(i);
    delay(ServoMoveDelay);
  }

  // Wait before moving back
  delay(ServoWaitDelay);

  // Move back
  for (int i = endPos; i >= 0; i--) {
    servo.write(i);
    delay(ServoMoveDelay);
  }
}

// The loop
void loop() {
  // Read all Buttons
  int creditRead = digitalRead(CoinButton);
  int col1ButtonRead = digitalRead(Col1Button);
  int col2ButtonRead = digitalRead(Col2Button);

  // If coin was inserted set credit to true and switch on green LEDs
  if (creditRead == LOW) {
    HasCredit = true;
    digitalWrite(Col1LedGreen, HIGH);
    digitalWrite(Col2LedGreen, HIGH);
  }

  // If has credit and button 1 was pushed, move servo 1
  if (HasCredit && col1ButtonRead == LOW) {
    digitalWrite(Col2LedGreen, LOW);
    digitalWrite(Col1LedRed, HIGH);
    moveServo(Col1Servo, Col1ServoEndPos);
    HasCredit = false;
    digitalWrite(Col1LedGreen, LOW);
    digitalWrite(Col1LedRed, LOW);
  }

  // If has credit and button 1 was pushed, move servo 1
  if (HasCredit && col2ButtonRead == LOW) {
    digitalWrite(Col1LedGreen, LOW);
    digitalWrite(Col2LedRed, HIGH);
    moveServo(Col2Servo, Col2ServoEndPos);
    HasCredit = false;
    digitalWrite(Col2LedGreen, LOW);
    digitalWrite(Col2LedRed, LOW);
  }
}
