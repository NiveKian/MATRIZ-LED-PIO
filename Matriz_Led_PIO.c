#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "pico/bootrom.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "matrizLedControl.h"
#include "keypad.h"
#include "drawing.h"

void menu_message()
{
  printf("\nMatrix Led Menu:\n");
  printf("Number - Exibits the number animation\n");
  printf("A - Turn off leds\n");
  printf("B - All Leds blue. 100.\n");
  printf("C - All Leds red. 80.\n");
  printf("D - All Leds green. 50.\n");
  printf("# - All Leds white. 20.\n");
  printf("* - \n\n");
}

void clean_leds()
{
  npClear();
  npWrite();
}

int main()
{
  // Inicializa entradas e saídas.
  stdio_init_all();
  printf("Init: ALL\n");

  // Inicializa o Keypad
  keypad_init();
  printf("Init: Keypad\n");

  // Inicializa matriz de LEDs NeoPixel.
  npInit();
  printf("Init: LEDS\n");
  npClear();

  // Loop de ações a serem completadas
  while (true)
  {
    // Get the input from the board
    char press_key = keypad_get_key();
    if (press_key)
      printf("Tecla pressionada: %c\n", press_key);
    switch (press_key)
    {
    case '1':
      //
      break;
    case '2':
      //
      break;
    case '3':
      //
      break;
    case '4':
       printf("Animação iniciada: Emerson Novais Oliveira.\n");
      startAnimation(animation_type4);
      break;
    case '5':
      //
      break;
    case '6':
      //
      break;
    case '7':
      //
      break;
    case '8':
      //
      break;
    case '9':
      //
      break;
    case 'A':
      clean_leds();
      changeDrawing(draw_type_B);
      break;
    case 'B':
      //
      break;
    case 'C':
      //
      break;
    case 'D':
      //
      break;
    case '#':
      //
      break;
    case '*':
      //
      break;
    default:
      sleep_ms(100);
    }
  }
}
