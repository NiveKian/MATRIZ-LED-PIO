#ifndef BUZZER_H
#define BUZZER_H

#include "pico/stdlib.h"
#include "hardware/pwm.h"  // BIBLIOTECAS BÁSICAS

// Definição de pinos e frequências das notas musicais
#define BUZZER 21 // PINO 21 DEFINIDO COMO BUZZER
#define C 261.63 // Nota Dó
#define Ch 277.18 // Dó#
#define D 293.66 // Nota Ré
#define Dh 311.13 // Ré#
#define E 329.63 // Nota Mi
#define F 349.23 // Nota Fá
#define Fh 369.99 // Fá#
#define G 392 // Nota Sol
#define Gh 415.30 // Sol#
#define A 440 // Nota Lá
#define Ah 466.16 // Lá#
#define B 493.88 // Nota Si
#define Bh 493.88 // Si#

// Declarações das funções
void BUZZER_CONFIG(uint gpio, float frequencia, float duty_cycle);
void DESLIGAR_BUZZER();
void TOCAR_NOTAS_TYPE9();

#endif // BUZZER_H
