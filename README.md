# Codes-for-ATMega328p
Códigos feitos no microcontrolador do Arduino UNO - ATMega328p para controle de LEDs, botões e displays de 7 segmentos usando cristal de quartzo e capacitores para clock, bargraph, resistores de proteção, botões com pull-up e pull-down. Foi utilizado o Arduino IDE utilizando macros, manipulação de registradores e bits e o software Proteus 8.16v para simulação.

1. LIGANDO LEDs

1.1 Desenvolver um programa para piscar um LED 3 vezes rápido e 3 vezes devagar.

1.2 Programar os 8 LEDs de um bargraph para os seguintes padrões:
a) Acender da direita para a esquerda, mantendo cada LED ligado até todos acenderem; depois apagar todos e repetir.
b) O mesmo da esquerda para a direita.
c) Apenas um LED aceso por vez, da direita para a esquerda.
d) Vai e volta (ping-pong), um LED aceso por vez.
e) Todos acesos e apagar 1 LED por vez, vai e volta.
f) Da esquerda para a direita, mantendo acesos; depois piscar todos 5 vezes; depois apagar.
g) Da direita para a esquerda, manter acesos; depois apagar; depois esquerda para direita.
h) Contagem binária crescente (0–255) com passo de 250 ms.
i) Contagem binária decrescente (255–0) com passo de 250 ms.

2. DISPLAYS DE 7 SEGMENTOS

Usar 2 displays multiplexados:

Display 1: contagem crescente de 0 a F.
Display 2: contagem decrescente de F a 0.

3. LENDO BOTÕES

3.1 LED acende com 1º clique, apaga com 2º.

3.2 LED alterna entre: ON → Piscar → Piscar rápido → OFF.

3.3 Sequência de 3 LEDs (1-2-3); ao apertar novamente, inverter (3-2-1).

3.4 Enquanto o botão estiver pressionado, LED pisca com frequência crescente até ficar aceso fixo; ao soltar, apaga.

3.5 Cada clique aumenta a frequência até ficar aceso fixo; se segurar por 5 s, apaga.

3.6 Dois botões + 1 LED: LED acende se qualquer um for pressionado; apaga se ambos forem pressionados.

3.7 Dois botões + 2 LEDs:

Botão 1 → LED1 aceso, LED2 piscando.
Botão 2 → inverter funções.
Ambos → apaga.

3.8 Dois botões + 3 LEDs:

Botão 1 → sequência 1-2-3.
Botão 2 → sequência 3-2-1.
Ambos → apagam.

3.9 Três botões + 4 LEDs:

Botão 1 → todos acesos.
Botão 2 → apenas LED1 e LED2.
Botão 3 → apenas LED3 e LED4.
Botão 1 + Botão 3 → todos apagam.

3.10 Três botões + 3 LEDs + display:

Botão 1 → display mostra “1”; LEDs: [1-ON, 2-OFF, 3-piscando].
Botão 2 → display mostra “2”; LEDs: [1-piscando, 2-ON, 3-ON].
Botão 3 → display mostra “3”; LEDs: [1-OFF, 2-piscando, 3-OFF].

Códigos feitos para a 1ª avaliação da disciplina de Microprocessadores e Microcontroladores. - Rafael Alencar, Engenharia da Computação - Universidade Estadual do Maranhão, 2025.
