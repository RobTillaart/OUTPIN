
[![Arduino CI](https://github.com/RobTillaart/OUTPIN/workflows/Arduino%20CI/badge.svg)](https://github.com/marketplace/actions/arduino_ci)
[![Arduino-lint](https://github.com/RobTillaart/OUTPIN/actions/workflows/arduino-lint.yml/badge.svg)](https://github.com/RobTillaart/OUTPIN/actions/workflows/arduino-lint.yml)
[![JSON check](https://github.com/RobTillaart/OUTPIN/actions/workflows/jsoncheck.yml/badge.svg)](https://github.com/RobTillaart/OUTPIN/actions/workflows/jsoncheck.yml)
[![GitHub issues](https://img.shields.io/github/issues/RobTillaart/OUTPIN.svg)](https://github.com/RobTillaart/OUTPIN/issues)

[![License: MIT](https://img.shields.io/badge/license-MIT-green.svg)](https://github.com/RobTillaart/OUTPIN/blob/master/LICENSE)
[![GitHub release](https://img.shields.io/github/release/RobTillaart/OUTPIN.svg?maxAge=3600)](https://github.com/RobTillaart/OUTPIN/releases)
[![PlatformIO Registry](https://badges.registry.platformio.org/packages/robtillaart/library/OUTPIN.svg)](https://registry.platformio.org/libraries/robtillaart/OUTPIN)


# OUTPIN

Arduino library for (AVR) faster and extended output pin.


## Description

**Experimental**

Arduino library for a faster output pin (AVR only) functions.
It can be used to replace **digitalWrite(pin, state)** when more performance
(AVR only) is needed. The trick is well known caching low level access. 

The interface of OUTPIN is straightforward. Besides setting a variable state it has 
optimized functions for **high()**, **low()** and **toggle()** which are all faster 
than equivalent code using digitalWrite().

The main reason to write this library was to optimize pulse lengths for a test setup.
This resulted in additional functions **pulseHigh()** and **pulseLow()**.
These have 255 steps between roughly 2 and 82 microseconds (step ~0.314 us).
Finally these is a **pulseOut(state, duration)** which is used for pulses above 80 us.

The library should work on other platforms, however the code is not optimized for them.
If you have (optimized) additions for other boards, please open a PR.

Feedback as always is welcome.


### Related

- https://github.com/RobTillaart/OUTPIN
- https://github.com/RobTillaart/PulsePattern
- digitalWriteFast()  TODO link


## Performance

See OUTPIN_performance.ino sketch, which is a mixed test for performance
and accuracy indication. The sketch has no correction for loop overhead 1000 calls.

Indicative time is per 1000 function calls, so digitalWrite() takes ~3.6 microseconds.

|  function       |  time us  |  factor   |  notes  |
|:----------------|:---------:|:---------:|:--------|
|  digitalWrite   |    3592   |   1.0 x   |  reference
|  set            |    2016   |   1.7 x   |
|  high           |    1136   |   3.1 x   |
|  low            |    1196   |   3.0 x   |
|  toggle         |    1136   |   3.1 x   |
|                 |           |           |
|  DW             |    6792   |   1.0 x   |  "pulseHigh" with digitalWrite() on UNO
|  pulseHigh()    |    1892   |   3.5 x   |  Actual pulse length is shorter (overhead).
|  pulseHigh(0)   |    2648   |   2.5 x   |
|  pulseHigh(20)  |    8928   |           |
|  step size      |  0.3140   |           |  tune pulseHigh() in these steps
|  pulseHigh(13)  |    6732   |   1.0 x   |   

Accuracy **pulseOut()**

|  range         |  accuracy         |  notes  |
|---------------:|:-----------------:|:-------:|
|    10 -   100  |  low              |  micros has step size 4 us.
|   100 -  1000  |  ~5.0% or better  |
|  1000 - 10000  |  ~0.5% or better  |



## Interface

```cpp
#include "OUTPIN.h"
```

### Constructor

- **OUTPIN(uint8_t pin, uint8_t state)** Constructor with initial state.

### Write functions

- **void set(uint8_t state)** sets pin state HIGH or LOW.
- **void high()** sets pin state HIGH.
- **void low()** sets pin state LOW.
- **void toggle()** reverses pin state.


### Pulse HIGH

- **void pulseHigh()** gives shortest possible HIGH pulse. 
Note this is faster than **pulseHigh(0)**
- **void pulseHigh(uint8_t nops)** extend HIGH pulse with NOPs.

The indicative formula for the pulse duration based upon the duration 
of the function calls **OUTPIN_pulseHighNops.ino** is (my UNO R3).
```
time = 2.172856 + 0.314356 * NOPs;
```
To calculate the number of **NOPs = round((time - 2.172856)/ 0.314356)**.
Note the actual pulse is slightly smaller as there is function call 
and some interrupt blocking overhead. (TODO better formula).

There is a "gap" between **pulseHigh()** and **pulseHigh(0)** for
which one could add functions with hard coded number of NOPs.
The library does not provide these as it would add about 11 extra functions.
This needs extra thoughts how to handle.


### Pulse LOW

- **void pulseLow()** gives shortest possible LOW pulse.
Note this is faster than **pulseLow(0)**
- **void pulseLow(uint8_t nops)** extend LOW pulse with NOPs.


### PulseOut

- **void pulseOut(uint8_t state, uint16_t microseconds)** for longer pulses.
The parameter microseconds should be > 75 for AVR UNO R3, to have reasonable accuracy.
Below 75 us one can better use **pulseHigh()** or **pulseLow()**

Other boards might need tweak in the .cpp file.

See **OUTPIN_performance.ino** sketch for figures.


## Future

#### Must

- update documentation
- get performance figures
- add examples

#### Should

- make timing measurements with an oscilloscope to see actual pulse lengths.
- add unit tests?.

#### Could

- example e.g. DHT22 simulator.
- example pulse generator - potmeter to set pulse length?

#### Wont



## Support

If you appreciate my libraries, you can support the development and maintenance.
Improve the quality of the libraries by providing issues and Pull Requests, or
donate through PayPal or GitHub sponsors.

Thank you,

