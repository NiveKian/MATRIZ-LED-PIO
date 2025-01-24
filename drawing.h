#ifndef DRAWING_H
#define DRAWING_H

#include <stdint.h>
#include "ledStruct.h" // Possible problem

// Export the variables of basic drawing
extern npLED_t draw_type_B[25]; // Basic blue drawing
extern npLED_t draw_type_C[25]; // Basic red drawing
extern npLED_t draw_type_D[25]; // Basic Green drawing
extern npLED_t draw_type_TAG[25]; // Basic white drawing

// Export the variables of animations
extern const npLED_t animation_type1[5][25]; // Ian Antunes - AnimationArray

#endif