#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "hardware/pwm.h"

// Biblioteca gerada pelo arquivo .pio durante compilação.
#include "ws2818b.pio.h"

// Definição do número de LEDs e pino.
#define LED_COUNT 25
#define LED_PIN 7
// Frequências das notas musicais (em Hz)
#define C4 261
#define D4 294
#define F4 349
#define E4 329

// Pino do GPIO onde o buzzer está conectado
#define BUZZER_PIN 21

//Inicializa o PWM no pino do buzzer
void pwm_init_buzzer(uint pin) {
    // Configura o pino como saída PWM
    gpio_set_function(pin, GPIO_FUNC_PWM);

    // Obtém o número do slice PWM associado ao pino
    uint slice_num = pwm_gpio_to_slice_num(pin);

    // Configura o PWM com as configurações padrão
    pwm_config config = pwm_get_default_config();
    pwm_init(slice_num, &config, true);

    // Inicia o PWM no nível baixo (0)
    pwm_set_gpio_level(pin, 0);
}

// Função que emite um som (beep) com uma frequência e duração especificadas
void beep(uint pin, uint frequency, uint duration_ms) {
    // Obtém o número do slice PWM associado ao pino
    uint slice_num = pwm_gpio_to_slice_num(pin);

    // Calcula a frequência do clock do sistema
    uint32_t clock_freq = clock_get_hz(clk_sys);
    // Calcula o divisor de clock necessário
    float divider = (float)clock_freq / (float)(frequency * 4096);
    // Define o divisor de clock para o slice PWM
    pwm_set_clkdiv(slice_num, divider);

    // Calcula o valor de 'top' baseado na frequência desejada
    uint32_t top = clock_freq / (frequency * divider) - 1;

    // Configura o valor de 'top' no PWM e ajusta o duty cycle para 50% (ativo)
    pwm_set_wrap(slice_num, top);
    pwm_set_gpio_level(pin, top / 2);

    // Aguarda pelo tempo de duração do beep
    sleep_ms(duration_ms);

    // Desativa o sinal PWM (duty cycle 0)
    pwm_set_gpio_level(pin, 0);

    // Pausa de 50ms entre os beeps
    sleep_ms(50);
}


// Definição de pixel GRB
struct pixel_t {
  uint8_t G, R, B; // Três valores de 8-bits compõem um pixel.
};
typedef struct pixel_t pixel_t;
typedef pixel_t npLED_t; // Mudança de nome de "struct pixel_t" para "npLED_t" por clareza.

// Declaração do buffer de pixels que formam a matriz.
npLED_t leds[LED_COUNT];

// Variáveis para uso da máquina PIO.
PIO np_pio;
uint sm;

/**
 * Inicializa a máquina PIO para controle da matriz de LEDs.
 */
void npInit(uint pin) {

  // Cria programa PIO.
  uint offset = pio_add_program(pio0, &ws2818b_program);
  np_pio = pio0;

  // Toma posse de uma máquina PIO.
  sm = pio_claim_unused_sm(np_pio, false);
  if (sm < 0) {
    np_pio = pio1;
    sm = pio_claim_unused_sm(np_pio, true); // Se nenhuma máquina estiver livre, panic!
  }

  // Inicia programa na máquina PIO obtida.
  ws2818b_program_init(np_pio, sm, offset, pin, 800000.f);

  // Limpa buffer de pixels.
  for (uint i = 0; i < LED_COUNT; ++i) {
    leds[i].R = 0;
    leds[i].G = 0;
    leds[i].B = 0;
  }
}

/**
 * Atribui uma cor RGB a um LED.
 */
void npSetLED(const uint index, const uint8_t r, const uint8_t g, const uint8_t b) {
  leds[index].R = r;
  leds[index].G = g;
  leds[index].B = b;
}

/**
 * Limpa o buffer de pixels.
 */
void npClear() {
  for (uint i = 0; i < LED_COUNT; ++i)
    npSetLED(i, 0, 0, 0);
}

/**
 * Escreve os dados do buffer nos LEDs.
 */
void npWrite() {
  // Escreve cada dado de 8-bits dos pixels em sequência no buffer da máquina PIO.
  for (uint i = 0; i < LED_COUNT; ++i) {
    pio_sm_put_blocking(np_pio, sm, leds[i].G);
    pio_sm_put_blocking(np_pio, sm, leds[i].R);
    pio_sm_put_blocking(np_pio, sm, leds[i].B);
  }
  sleep_us(100); // Espera 100us, sinal de RESET do datasheet.
}

// Modificado do github: https://github.com/BitDogLab/BitDogLab-C/tree/main/neopixel_pio
// Função para converter a posição do matriz para uma posição do vetor.
int getIndex(int x, int y) {
    // Se a linha for par (0, 2, 4), percorremos da esquerda para a direita.
    // Se a linha for ímpar (1, 3), percorremos da direita para a esquerda.
    if (y % 2 == 0) {
        return 24-(y * 5 + x); // Linha par (esquerda para direita).
    } else {
        return 24-(y * 5 + (4 - x)); // Linha ímpar (direita para esquerda).
    }
}

void animation_type9() {

  // Inicializa entradas e saídas.
  stdio_init_all();

  // Inicializa matriz de LEDs NeoPixel.
  npInit(LED_PIN);
  npClear();

  // Aqui, você desenha nos LEDs.

  npWrite(); // Escreve os dados nos LEDs.

 // Inicializa o PWM no pino do buzzer
  pwm_init_buzzer(BUZZER_PIN);

  // Não faz mais nada. Loop infinito.
  while (true) {


    beep(BUZZER_PIN, C4, 500); // Toca a nota C4 por 500ms
    beep(BUZZER_PIN, C4, 500); // Toca a nota C4 por 500ms
    beep(BUZZER_PIN, D4, 1000); // Toca a nota D4 por 1000ms
    beep(BUZZER_PIN, C4, 1000); // Toca a nota C4 por 1000ms
    beep(BUZZER_PIN, F4, 1000); // Toca a nota F4 por 1000ms
    beep(BUZZER_PIN, E4, 2000); // Toca a nota E4 por 2000ms

    int matrizH [5][5][3] = {
    {{0, 0, 0}, {255, 0, 0}, {0, 0, 0}, {255, 0, 0}, {0, 0, 0}},
    {{0, 0, 0}, {255, 0, 0}, {0, 0, 0}, {255, 0, 0}, {0, 0, 0}},
    {{0, 0, 0}, {255, 0, 0}, {255, 0, 0}, {255, 0, 0}, {0, 0, 0}},
    {{0, 0, 0}, {255, 0, 0}, {0, 0, 0}, {255, 0, 0}, {0, 0, 0}},
    {{0, 0, 0}, {255, 0, 0}, {0, 0, 0}, {255, 0, 0}, {0, 0, 0}}};
      // Desenhando Sprite contido na matriz.c
   for(int linha = 0; linha < 5; linha++){
    for(int coluna = 0; coluna < 5; coluna++){
      int posicao = getIndex(linha, coluna);
      npSetLED(posicao, matrizH[coluna][linha][0], matrizH[coluna][linha][1], matrizH[coluna][linha][2]);
    }
  } 
    npWrite();    
    sleep_ms(500);
    npClear();
    npWrite();
    sleep_ms(500);

    int matrizA [5][5][3] = {
   
    {{0, 0, 0}, {255, 0, 0}, {255, 0, 0}, {255, 0, 0}, {0, 0, 0}},
    {{0, 0, 0}, {255, 0, 0}, {0, 0, 0}, {255, 0, 0}, {0, 0, 0}},
    {{0, 0, 0}, {255, 0, 0}, {0, 0, 0}, {255, 0, 0}, {0, 0, 0}},
    {{0, 0, 0}, {255, 0, 0}, {255, 0, 0}, {255, 0, 0}, {0, 0, 0}},
    {{0, 0, 0}, {255, 0, 0}, {0, 0, 0}, {255, 0, 0}, {0, 0, 0}}};
      // Desenhando Sprite contido na matriz.c
   for(int linha = 0; linha < 5; linha++){
    for(int coluna = 0; coluna < 5; coluna++){
      int posicao = getIndex(linha, coluna);
      npSetLED(posicao, matrizA[coluna][linha][0], matrizA[coluna][linha][1], matrizA[coluna][linha][2]);
    }
  } 
    npWrite();    
    sleep_ms(500);
    npClear();
    npWrite();
    sleep_ms(500);

    int matrizP [5][5][3] = {
    
    {{0, 0, 0}, {255, 0, 0}, {255, 0, 0}, {255, 0, 0}, {0, 0, 0}},
    {{0, 0, 0}, {255, 0, 0}, {0, 0, 0}, {255, 0, 0}, {0, 0, 0}},
    {{0, 0, 0}, {255, 0, 0}, {255, 0, 0}, {255, 0, 0}, {0, 0, 0}},
    {{0, 0, 0}, {255, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
    {{0, 0, 0}, {255, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}
};
      // Desenhando Sprite contido na matriz.c
   for(int linha = 0; linha < 5; linha++){
    for(int coluna = 0; coluna < 5; coluna++){
      int posicao = getIndex(linha, coluna);
      npSetLED(posicao, matrizP[coluna][linha][0], matrizP[coluna][linha][1], matrizP[coluna][linha][2]);
    }
  } 
    npWrite();    
    sleep_ms(500);
    npClear();
    npWrite();
    sleep_ms(500);

    int matrizp [5][5][3] = {
    {{0, 0, 0}, {255, 0, 0}, {255, 0, 0}, {255, 0, 0}, {0, 0, 0}},
    {{0, 0, 0}, {255, 0, 0}, {0, 0, 0}, {255, 0, 0}, {0, 0, 0}},
    {{0, 0, 0}, {255, 0, 0}, {255, 0, 0}, {255, 0, 0}, {0, 0, 0}},
    {{0, 0, 0}, {255, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
    {{0, 0, 0}, {255, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}};
      // Desenhando Sprite contido na matriz.c
   for(int linha = 0; linha < 5; linha++){
    for(int coluna = 0; coluna < 5; coluna++){
      int posicao = getIndex(linha, coluna);
      npSetLED(posicao, matrizp[coluna][linha][0], matrizp[coluna][linha][1], matrizp[coluna][linha][2]);
    }
  } 
    npWrite();    
    sleep_ms(500);
    npClear();
    npWrite();
    sleep_ms(500);

    int matrizY [5][5][3] = {
    {{255, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {255, 0, 0}},
    {{0, 0, 0}, {255, 0, 0}, {0, 0, 0}, {255, 0, 0}, {0, 0, 0}},
    {{0, 0, 0}, {0, 0, 0}, {255, 0, 0}, {0, 0, 0}, {0, 0, 0}},
    {{0, 0, 0}, {0, 0, 0}, {255, 0, 0}, {0, 0, 0}, {0, 0, 0}},
    {{0, 0, 0}, {0, 0, 0}, {255, 0, 0}, {0, 0, 0}, {0, 0, 0}}
};
      // Desenhando Sprite contido na matriz.c
   for(int linha = 0; linha < 5; linha++){
    for(int coluna = 0; coluna < 5; coluna++){
      int posicao = getIndex(linha, coluna);
      npSetLED(posicao, matrizY[coluna][linha][0], matrizY[coluna][linha][1], matrizY[coluna][linha][2]);
    }
  } 
    npWrite();    
    sleep_ms(500);
    npClear();
    npWrite();
    sleep_ms(1000);

    int matrizB [5][5][3] = {
    
    {{255, 0, 0}, {255, 0, 0}, {255, 0, 0}, {0, 0, 0}, {0, 0, 0}},
    {{255, 0, 0}, {0, 0, 0}, {0, 0, 0}, {255, 0, 0}, {0, 0, 0}},
    {{255, 0, 0}, {255, 0, 0}, {255, 0, 0}, {0, 0, 0}, {0, 0, 0}},
    {{255, 0, 0}, {0, 0, 0}, {0, 0, 0}, {255, 0, 0}, {0, 0, 0}},
    {{255, 0, 0}, {255, 0, 0}, {255, 0, 0}, {0, 0, 0}, {0, 0, 0}}
    };
      // Desenhando Sprite contido na matriz.c
   for(int linha = 0; linha < 5; linha++){
    for(int coluna = 0; coluna < 5; coluna++){
      int posicao = getIndex(linha, coluna);
      npSetLED(posicao, matrizB[coluna][linha][0], matrizB[coluna][linha][1], matrizB[coluna][linha][2]);
    }
  } 
    npWrite();    
    sleep_ms(500);
    npClear();
    npWrite();
    sleep_ms(500);

    int matrizI [5][5][3] = {
   {{0, 0, 0}, {0, 0, 0}, {255, 0, 0}, {0, 0, 0}, {0, 0, 0}},
    {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
    {{0, 0, 0}, {0, 0, 0}, {255, 0, 0}, {0, 0, 0}, {0, 0, 0}},
    {{0, 0, 0}, {0, 0, 0}, {255, 0, 0}, {0, 0, 0}, {0, 0, 0}},
    {{0, 0, 0}, {0, 0, 0}, {255, 0, 0}, {0, 0, 0}, {0, 0, 0}}
};
      // Desenhando Sprite contido na matriz.c
   for(int linha = 0; linha < 5; linha++){
    for(int coluna = 0; coluna < 5; coluna++){
      int posicao = getIndex(linha, coluna);
      npSetLED(posicao, matrizI[coluna][linha][0], matrizI[coluna][linha][1], matrizI[coluna][linha][2]);
    }
  } 
    npWrite();    
    sleep_ms(500);
    npClear();
    npWrite();
    sleep_ms(500);

    int matrizR [5][5][3] = {
{{255, 0, 0}, {255, 0, 0}, {255, 0, 0}, {0, 0, 0}, {0, 0, 0}},
    {{255, 0, 0}, {0, 0, 0}, {0, 0, 0}, {255, 0, 0}, {0, 0, 0}},
    {{255, 0, 0}, {0, 0, 0}, {0, 0, 0}, {255, 0, 0}, {0, 0, 0}},
    {{255, 0, 0}, {255, 0, 0}, {255, 0, 0}, {0, 0, 0}, {0, 0, 0}},
    {{255, 0, 0}, {0, 0, 0}, {0, 0, 0}, {255, 0, 0}, {0, 0, 0}}
};
      // Desenhando Sprite contido na matriz.c
   for(int linha = 0; linha < 5; linha++){
    for(int coluna = 0; coluna < 5; coluna++){
      int posicao = getIndex(linha, coluna);
      npSetLED(posicao, matrizR[coluna][linha][0], matrizR[coluna][linha][1], matrizR[coluna][linha][2]);
    }
  } 
    npWrite();    
    sleep_ms(500);
    npClear();
    npWrite();
    sleep_ms(500);

    int matrizT [5][5][3] = { 
    {{255, 0, 0}, {255, 0, 0}, {255, 0, 0}, {255, 0, 0}, {255, 0, 0}},
    {{0, 0, 0}, {0, 0, 0}, {255, 0, 0}, {0, 0, 0}, {0, 0, 0}},
    {{0, 0, 0}, {0, 0, 0}, {255, 0, 0}, {0, 0, 0}, {0, 0, 0}},
    {{0, 0, 0}, {0, 0, 0}, {255, 0, 0}, {0, 0, 0}, {0, 0, 0}},
    {{0, 0, 0}, {0, 0, 0}, {255, 0, 0}, {0, 0, 0}, {0, 0, 0}}

    };
      // Desenhando Sprite contido na matriz.c
   for(int linha = 0; linha < 5; linha++){
    for(int coluna = 0; coluna < 5; coluna++){
      int posicao = getIndex(linha, coluna);
      npSetLED(posicao, matrizT[coluna][linha][0], matrizT[coluna][linha][1], matrizT[coluna][linha][2]);
    }
  } 
    npWrite();    
    sleep_ms(500);
    npClear();
    npWrite();
    sleep_ms(500);

    int matrizh [5][5][3] = {
    {{0, 0, 0}, {255, 0, 0}, {0, 0, 0}, {255, 0, 0}, {0, 0, 0}},
    {{0, 0, 0}, {255, 0, 0}, {0, 0, 0}, {255, 0, 0}, {0, 0, 0}},
    {{0, 0, 0}, {255, 0, 0}, {255, 0, 0}, {255, 0, 0}, {0, 0, 0}},
    {{0, 0, 0}, {255, 0, 0}, {0, 0, 0}, {255, 0, 0}, {0, 0, 0}},
    {{0, 0, 0}, {255, 0, 0}, {0, 0, 0}, {255, 0, 0}, {0, 0, 0}}};
      // Desenhando Sprite contido na matriz.c
   for(int linha = 0; linha < 5; linha++){
    for(int coluna = 0; coluna < 5; coluna++){
      int posicao = getIndex(linha, coluna);
      npSetLED(posicao, matrizh[coluna][linha][0], matrizh[coluna][linha][1], matrizh[coluna][linha][2]);
    }
  } 
    npWrite();    
    sleep_ms(500);
    npClear();
    npWrite();
    sleep_ms(500);

    int matrizD [5][5][3] = {
    {{255, 0, 0}, {255, 0, 0}, {255, 0, 0}, {0, 0, 0}, {0, 0, 0}},
    {{255, 0, 0}, {0, 0, 0}, {0, 0, 0}, {255, 0, 0}, {0, 0, 0}},
    {{255, 0, 0}, {0, 0, 0}, {0, 0, 0}, {255, 0, 0}, {0, 0, 0}},
    {{255, 0, 0}, {0, 0, 0}, {0, 0, 0}, {255, 0, 0}, {0, 0, 0}},
    {{255, 0, 0}, {255, 0, 0}, {255, 0, 0}, {0, 0, 0}, {0, 0, 0}}
};
      // Desenhando Sprite contido na matriz.c
   for(int linha = 0; linha < 5; linha++){
    for(int coluna = 0; coluna < 5; coluna++){
      int posicao = getIndex(linha, coluna);
      npSetLED(posicao, matrizD[coluna][linha][0], matrizD[coluna][linha][1], matrizD[coluna][linha][2]);
    }
  } 
    npWrite();    
    sleep_ms(500);
    npClear();
    npWrite();
    sleep_ms(500);

    int matriza [5][5][3] = {
 {{0, 0, 0}, {255, 0, 0}, {255, 0, 0}, {255, 0, 0}, {0, 0, 0}},
    {{0, 0, 0}, {255, 0, 0}, {0, 0, 0}, {255, 0, 0}, {0, 0, 0}},
    {{0, 0, 0}, {255, 0, 0}, {0, 0, 0}, {255, 0, 0}, {0, 0, 0}},
    {{0, 0, 0}, {255, 0, 0}, {255, 0, 0}, {255, 0, 0}, {0, 0, 0}},
    {{0, 0, 0}, {255, 0, 0}, {0, 0, 0}, {255, 0, 0}, {0, 0, 0}}};
  // Desenhando Sprite contido na matriz.c
   for(int linha = 0; linha < 5; linha++){
    for(int coluna = 0; coluna < 5; coluna++){
      int posicao = getIndex(linha, coluna);
      npSetLED(posicao, matriza[coluna][linha][0], matriza[coluna][linha][1], matriza[coluna][linha][2]);
    }
  } 
    npWrite();    
    sleep_ms(500);
    npClear();
    npWrite();
    sleep_ms(500);

    int matrizy [5][5][3] = {
    {{255, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {255, 0, 0}},
    {{0, 0, 0}, {255, 0, 0}, {0, 0, 0}, {255, 0, 0}, {0, 0, 0}},
    {{0, 0, 0}, {0, 0, 0}, {255, 0, 0}, {0, 0, 0}, {0, 0, 0}},
    {{0, 0, 0}, {0, 0, 0}, {255, 0, 0}, {0, 0, 0}, {0, 0, 0}},
    {{0, 0, 0}, {0, 0, 0}, {255, 0, 0}, {0, 0, 0}, {0, 0, 0}}};
      // Desenhando Sprite contido na matriz.c
   for(int linha = 0; linha < 5; linha++){
    for(int coluna = 0; coluna < 5; coluna++){
      int posicao = getIndex(linha, coluna);
      npSetLED(posicao, matrizy[coluna][linha][0], matrizy[coluna][linha][1], matrizy[coluna][linha][2]);
    }
  } 
    npWrite();    
    sleep_ms(500);
    npClear();
    npWrite();
    sleep_ms(2000);

  }
}
