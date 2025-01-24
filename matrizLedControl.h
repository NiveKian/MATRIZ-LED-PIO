#ifndef MATRIZLEDCONTROL_H
#define MATRIZLEDCONTROL_H

#include <stdint.h> // to uint work
#include "ledStruct.h"

void npInit(); // Init the LED matriz pin
void npClear(); // Clear the matriz buffer
void npWrite(); // Write the buffer in the LEDS
void changeDrawing(npLED_t newDraw[]); // Change the LEDS colors based on 25(R,G,B) array.

#endif