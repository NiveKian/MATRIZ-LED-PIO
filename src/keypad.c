#include <stdio.h>
#include "pico/stdlib.h"
#include "keypad.h"

// Numer of Rows and collumns
#define ROWS 4
#define COLS 4

// KEYPAD GPIO
const uint8_t row_pins[ROWS] = {16, 17, 18, 19};
const uint8_t col_pins[COLS] = {20, 21, 22, 26};

// KEYS MAPING
const char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

void keypad_init() {
  // Initialize rows [HIGH]
  for (int i = 0; i < ROWS; i++) {
    gpio_init(row_pins[i]);
    gpio_set_dir(row_pins[i], GPIO_OUT);
    gpio_put(row_pins[i], 1);
  }
  // Initialize collums [LOW]
  for (int j = 0; j < COLS; j++) {
    gpio_init(col_pins[j]);
    gpio_set_dir(col_pins[j], GPIO_IN);
    gpio_pull_up(col_pins[j]);
  }
}

char keypad_get_key() {
  // Loops trough ROWS
  for (int row = 0; row < ROWS; row++) {
    // sets row to LOW state
    gpio_put(row_pins[row], 0);

    // Loops trough COLLUMS
    for (int col = 0; col < COLS; col++) {
      if (gpio_get(col_pins[col]) == 0) { // Detects pressing (LOW)
        // waits for key release
        while (gpio_get(col_pins[col]) == 0);
        // sets col back to HIGH state
        gpio_put(row_pins[row], 1);
        return keys[row][col];
      }
    }

    // sets row back to HIGH state
    gpio_put(row_pins[row], 1);
  }

  // No INPUT
  return 0;
}