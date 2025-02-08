//
//    FILE: OUTPIN_pulseHighNops_measure.ino
//  AUTHOR: Rob Tillaart
// PURPOSE: measure range of function
//     URL: https://github.com/RobTillaart/OUTPIN
//
//  sketch to measure the timing on a scope


#include "OUTPIN.h"

//  adjust pin if needed
OUTPIN op(13, LOW);

uint32_t start, stop;

void setup()
{
  Serial.begin(115200);
  while (!Serial);
  Serial.println(__FILE__);
  Serial.print("OUTPIN_LIB_VERSION: ");
  Serial.println(OUTPIN_LIB_VERSION);
  Serial.println();
  delay(100);

  while (1)
  {
    op.pulseHigh(80);
    // op.pulseHigh(50);
    delayMicroseconds(25);
  }
}


void loop()
{
}


//  -- END OF FILE --
