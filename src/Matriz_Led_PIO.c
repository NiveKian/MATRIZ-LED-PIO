#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "pico/bootrom.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "util/matrizLedControl.h"
#include "util/keypad.h"
#include "util/drawing.h"
#include "pico/bootrom.h"

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
      printf("Animação iniciada: Ian Antunes.\n");
      startAnimation(animation_type1);
      break;
    case '2':
       printf("Animação iniciada: Matheus Vinicius.\n");
      startAnimation(animation_type2);
      break;
    case '3':
       printf("Animação iniciada: Rayane Queiroz.\n");
      startAnimation(animation_type3);
      break;
    case '4':
      //
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
      break;
    case 'B':
      printf("Todos os LEDs azul, 100%% intensidade.\n");
      changeDrawing(draw_type_B);
      // matrizLedControl_setAll(0, 0, 255); // Azul puro (R=0, G=0, B=255)
      break;

    case 'C':
      printf("Todos os LEDs vermelhos, 80%% intensidade.\n");
      changeDrawing(draw_type_C);
      // matrizLedControl_setAll(draw_type_C);
      break;
    case 'D':
      //
      break;
    case '#':
      //
      break;
    case '*':
      printf("Reiniciando no modo USB...\n");
      sleep_ms(2000);
      reset_usb_boot(0, 0);

      break;
    default:
      sleep_ms(100);
    }
  }
}
