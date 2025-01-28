#include "pico/stdlib.h"
#include "buzzer.h"  // Inclui o arquivo de cabeçalho buzzer.h

void BUZZER_CONFIG(uint gpio, float frequencia, float duty_cycle){
    gpio_set_function(gpio, GPIO_FUNC_PWM); // DEFINE O PINO COMO SAÍDA PWM
    uint slice_num = pwm_gpio_to_slice_num(gpio); // OBTÉM O NÚMERO DA SLICE ASSOCIADA AO PINO
    uint channel = pwm_gpio_to_channel(gpio); // OBTÉM O NÚMERO DO CANAL

    float clk_div = 8.0f;  // DIVISOR DE CLOCK
    uint32_t wrap = (uint32_t)((125e6 / (frequencia * clk_div)) - 1);
   
    // CONFIGURAÇÃO PWM
    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, clk_div);
    pwm_config_set_wrap(&config, wrap);
    pwm_init(slice_num, &config, true);
    
    // CONFIGURAÇÃO DUTY_CICLE
    uint32_t level = (uint32_t)(wrap * duty_cycle);
    pwm_set_chan_level(slice_num, channel, level);
    pwm_set_enabled(slice_num, true);
}

void DESLIGAR_BUZZER() {
    uint slice_num = pwm_gpio_to_slice_num(BUZZER);
    uint channel = pwm_gpio_to_channel(BUZZER);
    pwm_set_chan_level(slice_num, channel, 0);  // Desliga o buzzer
    pwm_set_enabled(slice_num, false);
}

void TOCAR_NOTAS_TYPE9() {
    // Notas e durações para "Parabéns pra Você"
    float notas[] = {C, C, D, C, F, E, 
                     C, C, D, C, G, F, 
                     C, C, Ch, A, F, E, D, 
                     Ah, Ah, A, F, G, F};
    int duracoes[] = {500, 500, 1000, 1000, 1000, 2000, 
                      500, 500, 1000, 1000, 1000, 2000, 
                      500, 500, 1000, 1000, 1000, 1000, 2000, 
                      500, 500, 1000, 1000, 1000, 2000};
    int num_notas = sizeof(notas) / sizeof(notas[0]);

    for (int i = 0; i < num_notas; i++) {
        BUZZER_CONFIG(BUZZER, notas[i], 0.5f); // Toca cada nota com duty cycle de 50%
        sleep_ms(duracoes[i]); // Duração da nota
        DESLIGAR_BUZZER(); // Desliga o buzzer após tocar a nota
    }
}

int main() {
    stdio_init_all();
    TOCAR_NOTAS_TYPE9();
    return 0;
}
