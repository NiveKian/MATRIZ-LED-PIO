#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "pico/bootrom.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "ws2818b.pio.h"
#include "matrizLedControl.h"
#include "ledStruct.h"
#include "definitions.h"
#include "drawing.h"
#include "ws2818b.pio.h"


extern PIO np_pio;
extern uint sm;

// Definição do pino dos LEDS.
#define LED_PIN 7

// Declaração do buffer de pixels que formam a matriz.
npLED_t leds[LED_COUNT];

// Variáveis para uso da máquina PIO.
PIO np_pio;
uint sm;

/**
 * Inicializa a máquina PIO para controle da matriz de LEDs.
 */
void npInit()
{

  // Cria programa PIO.
  uint offset = pio_add_program(pio0, &ws2818b_program);
  np_pio = pio0;

  // Toma posse de uma máquina PIO.
  sm = pio_claim_unused_sm(np_pio, false);
  if (sm < 0)
  {
    np_pio = pio1;
    sm = pio_claim_unused_sm(np_pio, true); // Se nenhuma máquina estiver livre, panic!
  }

  // Inicia programa na máquina PIO obtida.
  ws2818b_program_init(np_pio, sm, offset, LED_PIN, 800000.f);

  // Limpa buffer de pixels.
  for (uint i = 0; i < LED_COUNT; ++i)
  {
    leds[i].R = 0;
    leds[i].G = 0;
    leds[i].B = 0;
  }
}

/**
 * Atribui uma cor RGB a um LED.
 */
void npSetLED(const uint index, const uint8_t r, const uint8_t g, const uint8_t b)
{
  leds[index].R = r;
  leds[index].G = g;
  leds[index].B = b;
}

/**
 * Limpa o buffer de pixels.
 */
void npClear()
{
  for (uint i = 0; i < LED_COUNT; ++i)
    npSetLED(i, 0, 0, 0);
}

/**
 * Escreve os dados do buffer nos LEDs.
 */
void npWrite()
{
  // Escreve cada dado de 8-bits dos pixels em sequência no buffer da máquina PIO.
  for (uint i = 0; i < LED_COUNT; ++i)
  {
    pio_sm_put_blocking(np_pio, sm, leds[i].G);
    pio_sm_put_blocking(np_pio, sm, leds[i].R);
    pio_sm_put_blocking(np_pio, sm, leds[i].B);
  }
  sleep_us(100); // Espera 100us, sinal de RESET do datasheet.
}

/**
 * Atera o brilho dos LEDS no buffer.
 */
void matrizLedControl_setAll(uint8_t r, uint8_t g, uint8_t b)
{
    for (int i = 0; i < LED_COUNT; i++)
    {
        npSetLED(i, r, g, b); // Configura cada LED
    }
    npWrite(); // Aplica as mudanças nos LEDs
}
void brightness()
{
  // CODE
}

/**
 * Troca o desenho dos LEDS.
 */
void changeDrawing(npLED_t newDraw[])
{
  // Coloca os novos valores fornecidos no buffer
  for (size_t i = 0; i < LED_COUNT; i++)
  {
    npSetLED(i, newDraw->R, newDraw->G, newDraw->B);
  }

  // Escreve nos LEDS
  npWrite();
}

/**
 * Realiza uma animação dos LEDS em um intervalo de tempo, usando a mudança de desenhos.
 */
void startAnimation()
{
  // CODE
}

void npDrawFrame(double *frame) {
    uint32_t valor_led;
    for (int i = 0; i < 25; i++) {
        // Adapte para enviar valores RGB para a matriz de LEDs
        valor_led = matrix_rgb(frame[i] * 0.8, frame[i] * 0.0, frame[i] * 0.0); // Cor vermelha
        pio_sm_put_blocking(np_pio, sm, valor_led);
    }
}

// Função para animação de coração pulsando
void animar_coracao() {
    for (int i = 0; i < 3; i++) {
        npDrawFrame(coracao1);
        sleep_ms(500); // Tempo de exibição do primeiro frame
        npDrawFrame(coracao2);
        sleep_ms(500); // Tempo de exibição do segundo frame
    }
}

// matrizLedControl.c
uint32_t matrix_rgb(double r, double g, double b) {
    unsigned char R, G, B;
    R = (unsigned char)(r * 255);  // Red
    G = (unsigned char)(g * 255);  // Green
    B = (unsigned char)(b * 255);  // Blue

    // Retorna o valor RGB no formato desejado
    return (G << 24) | (R << 16) | (B << 8);  // No formato (G << 24) | (R << 16) | (B << 8)
}
