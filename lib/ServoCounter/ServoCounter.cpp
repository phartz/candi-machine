#include <ServoCounter.h>

ServoCounter::ServoCounter(uint8_t pinDeci, uint8_t pinDigit)
{
  _pinDigit = pinDigit;
  _pinDeci  = pinDeci;
}

void ServoCounter::Init(){
  _servoDigit.attach(_pinDigit);
  _servoDeci.attach(_pinDeci);

  _servoDigit.write(_minDigit);
  _servoDeci.write(_minDeci);
}

void ServoCounter::moveTo(uint8_t number){
  int actDeci = _actPos / 10;
  int actDigit = _actPos % 10;

  int nextDeci = number / 10;
  int nextDigit = number % 10;

  int stepDeci = (nextDeci - actDeci) * -1;
  int stepDigit = nextDigit - actDigit;

  int posDeci = _minDeci - (actDeci * 15);
  int posDigit = _minDigit + (actDigit * 15);

  Serial.print("posDeci: ");
  Serial.print(posDeci);
  Serial.print(" - posDigit: ");
  Serial.print(posDigit);
  Serial.println();

  for (uint8_t i = 0; i < 15; i++)
  {
    posDeci += stepDeci;
    posDigit += stepDigit;
    _servoDigit.write(posDigit);
    _servoDeci.write(posDeci);
    delay(_moveDelay);
    Serial.print("posDeci: ");
    Serial.print(posDeci);
    Serial.print(" - posDigit: ");    
    Serial.print(posDigit);
    Serial.println();
  }
  
  _actPos = number;
}


void ServoCounter::Inc(){
  if (_actPos == 99) return;
  moveTo(_actPos + 1);
}

void ServoCounter::Dec(){
  if (_actPos == 0) return;
  moveTo(_actPos - 1);
}

void ServoCounter::Set(uint8_t pos){
  moveTo(pos > 99 ? 99 : pos);
}

void ServoCounter::Move(uint8_t pos){
  _servoDeci.write(pos);
}

void ServoCounter::MoveMin(){
  _servoDeci.write(0);
  delay(2000);
  _servoDeci.write(200);
  delay(2000);
}