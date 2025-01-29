## Tarefa Embarcatech: Animações em matriz LED 5x5

Atividade em grupo com o objetivo de realizar o acionamento dos 25 LEDs, de forma a criar diferentes tipos de animações de curta duração.

# Modelo das pastas

```
SRC  
├── Matriz_Led_PIO.c (arquivo principal a ser rodado)  
└── Util  
    ├── drawing.c (local de produção dos arrays de desenho)  
    ├── drawing.h  
    ├── keypad.c (responsável pela coordenação do keypad)  
    ├── keypad.h  
    ├── matrizLedControl.c (responsável pelo controle da matriz de led 5x5)  
    ├── matrizLedControl.h  
    ├── definitions.h (guarda e salva Definições usadas)  
    └── ledStruct.h (salva o struct utilizado para o RGB) 
``` 

# Funções

Comandos do keypad:
  
[0-9]) Uma animação feita cada uma por um membro da equipe. utilizando 5 frames. A X frames por segundo.
5) Animação dos quadrados.

   Representação em imagem :
![Imagem] (imagens/Botão 5.png)
6) Animação do coração pulsando.

 Representação em imagem :
![Imagem] (imagens/Botão 6.png)
7) Animação dos relacionamnetos atuais.

    1- carinha triste
    2- coração (se apaixonou)
    3- carinha feliz
    4- carinha surpreso (descobriu uma traição)
    5- ficou triste novamente

 Representação em imagem:
![Imagem] (imagens/Botão 7.png)
8) Animação semáforo.

    1-semáforo com todos os leds ligados
    2-Liga a cor vermelha (Pare)
    3-Liga a cor laranja (Atenção diminua a velocidade)
    4-Liga a cor verde (Seguir)
    5-Um carro (Para demonstrar que está seguindo caminho)

 Representação em imagem:
![Imagem] (imagens/Botão 8.png)

A) Desliga todos os LEDS  
B) Liga todos os LEDS na cor azul. Intensidade 100%.  
C) Liga todos os LEDS na cor vermelha. Intensidade 80%.  
D) Liga todos os LEDS na cor verde. Intensidade 50%.  
\#) Liga todos os LEDS na cor branca. Intensidade 20%. 

# Como compilar

1) Instale a extensão Raspeberry PI PICO do vscode
2) Compile o código clicando em compile project na aba da extensão, ou rodando o arquivo cmakelist manualmente.

![Como compilar](images/compile_image.png)

# Como rodar

1) Primeiramente compile o código para que a biblioteca ws2818b.pio seja adicionada ao sistema.
2) Instale a extensão Wokwi do vscode e confira se esta esta corretamente configurada (consulte o site https://docs.wokwi.com/pt-BR/vscode/getting-started em caso de dúvidas)
3) Abra o arquivo diagram e click no botão de play

![Como compilar](images/wokwi_diagram.png)

# Video do funcionamento
