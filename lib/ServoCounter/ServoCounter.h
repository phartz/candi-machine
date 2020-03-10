#ifndef ServoCounter_h
#define ServoCounter_h

#include <Arduino.h>
#include <Servo.h>

class ServoCounter
{
private:
  Servo _servoDigit;
  Servo _servoDeci;

  uint8_t _actPos;

  uint8_t _minDigit = 28;
  uint8_t _maxDigit = 178;

  uint8_t _minDeci = 158;
  uint8_t _maxDeci = 8;
  
  uint8_t _pinDigit = 0;
  uint8_t _pinDeci = 0;

  int _moveDelay = 15;

  void moveTo(uint8_t);

public:
  ServoCounter(uint8_t, uint8_t);

  void Init();

  void Inc();
  void Dec();

  void MoveMin();
  void Move(uint8_t);

  void Set(uint8_t);
};

#endif 
