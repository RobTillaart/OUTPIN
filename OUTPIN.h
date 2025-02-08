#pragma once
//
//    FILE: OUTPIN.h
//  AUTHOR: Rob Tillaart
// VERSION: 0.1.0
// PURPOSE: Arduino library for (AVR) faster and extended output pin.
//    DATE: 2016-12-27
//     URL: https://github.com/RobTillaart/OUTPIN


#include "Arduino.h"

#define OUTPIN_LIB_VERSION         (F("0.1.0"))


class OUTPIN
{
public:
  OUTPIN(uint8_t pin, uint8_t state)
  {
    _pin = pin;
    pinMode(_pin, OUTPUT);
#if defined(ARDUINO_ARCH_AVR) || defined(ARDUINO_ARCH_MEGAAVR)
    _port    = digitalPinToPort(_pin);
    _out     = portOutputRegister(_port);
    _bitmask = digitalPinToBitMask(_pin);
    set(state);
#else
    digitalWrite(_pin, state);
#endif
  };

  void set(uint8_t state)
  {
#if defined(ARDUINO_ARCH_AVR) || defined(ARDUINO_ARCH_MEGAAVR)
    uint8_t oldSREG = SREG;
    noInterrupts();
    if (state == LOW) *_out &= ~_bitmask;
    else              *_out |= _bitmask;
    SREG = oldSREG;
#else
    digitalWrite(_pin, state);
#endif
  };

  void high()
  {
#if defined(ARDUINO_ARCH_AVR) || defined(ARDUINO_ARCH_MEGAAVR)
    uint8_t oldSREG = SREG;
    noInterrupts();
    *_out |= _bitmask;
    SREG = oldSREG;
#else
    digitalWrite(_pin, state);
#endif
  };

  void low()
  {
#if defined(ARDUINO_ARCH_AVR) || defined(ARDUINO_ARCH_MEGAAVR)
    uint8_t oldSREG = SREG;
    noInterrupts();
    *_out &= ~_bitmask;
    SREG = oldSREG;
#else
    digitalWrite(_pin, state);
#endif
  };

  void toggle()
  {
#if defined(ARDUINO_ARCH_AVR) || defined(ARDUINO_ARCH_MEGAAVR)
    uint8_t oldSREG = SREG;
    noInterrupts();
    *_out ^= _bitmask;
    SREG = oldSREG;
#else
    digitalWrite(_pin, !digitalRead(pin));
#endif
  };

  //
  //  PULSEHIGH
  //
  void pulseHigh()  //  no NOPs
  {
#if defined(ARDUINO_ARCH_AVR) || defined(ARDUINO_ARCH_MEGAAVR)
    uint8_t oldSREG = SREG;
    noInterrupts();
    *_out |= _bitmask;
    *_out &= ~_bitmask;
    SREG = oldSREG;
#else
    digitalWrite(_pin, HIGH);
    digitalWrite(_pin, LOW);
#endif
  };

  void pulseHigh(uint8_t nops)
  {
#if defined(ARDUINO_ARCH_AVR) || defined(ARDUINO_ARCH_MEGAAVR)
    uint8_t oldSREG = SREG;
    noInterrupts();
    *_out |= _bitmask;
    while(nops--) asm("NOP");
    *_out &= ~_bitmask;
    SREG = oldSREG;
#else
    digitalWrite(_pin, HIGH);
    while(nops--) asm("NOP");
    digitalWrite(_pin, LOW);
#endif
  };

  void pulseLow()  //  no NOPs
  {
#if defined(ARDUINO_ARCH_AVR) || defined(ARDUINO_ARCH_MEGAAVR)
    uint8_t oldSREG = SREG;
    noInterrupts();
    *_out &= ~_bitmask;
    *_out |= _bitmask;
    SREG = oldSREG;
#else
    digitalWrite(_pin, LOW);
    digitalWrite(_pin, HIGH);
#endif
  };

  void pulseLow(uint8_t nops)
  {
#if defined(ARDUINO_ARCH_AVR) || defined(ARDUINO_ARCH_MEGAAVR)
    uint8_t oldSREG = SREG;
    noInterrupts();
    *_out &= ~_bitmask;
    while(nops--) asm("NOP");
    *_out |= _bitmask;
    SREG = oldSREG;
#else
    digitalWrite(_pin, LOW);
    while(nops--) asm("NOP");
    digitalWrite(_pin, HIGH);
#endif
  };

  void pulseOut(uint8_t state, uint16_t microseconds)
  {
#if defined(ARDUINO_ARCH_AVR) || defined(ARDUINO_ARCH_MEGAAVR)
    set(state);
    //  TUNED for overhead
    if (microseconds > 10) delayMicroseconds(microseconds - 10);
    set(!state);
#else
    digitalWrite(_pin, state);
    //  TUNE OTHER BOARDS HERE.
    if (microseconds > 1) delayMicroseconds(microseconds - 1);
    digitalWrite(_pin, !state);
#endif
  };


private:
  uint8_t _pin;
#if defined(ARDUINO_ARCH_AVR) || defined(ARDUINO_ARCH_MEGAAVR)
  uint8_t _port;
  volatile uint8_t * _out;
  uint8_t _bitmask;
  #endif
};


//  -- END OF FILE --

