/*
1° Avaliação de Microcontroladores - 1. Ligando LEDS
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

-Mapeamento de Pinos (Arduino -> ATmega328P):
 * BG 1: Pino 4  -> PORTD, Bit 4 (PD4)
 * BG 2: Pino 3  -> PORTD, Bit 3 (PD3)
 * BG 3: Pino 2  -> PORTD, Bit 2 (PD2)
 * BG 4: Pino 1  -> PORTD, Bit 1 (PD1)
 * BG 5: Pino 8  -> PORTB, Bit 0 (PB0)
 * BG 6: Pino 9  -> PORTB, Bit 1 (PB1)
 * BG 7: Pino 10 -> PORTB, Bit 2 (PB2)
 * BG 8: Pino 11 -> PORTB, Bit 3 (PB3)
 * BG 9: Pino 12 -> PORTB, Bit 4 (PB4)
 */

// Biblioteca de registradores do ATmega
#include <avr/io.h>

// - Macros de Manipulação de Bits
// Valor do Bit
#define BIT(x) (1 << (x)) 
// Seta um bit (liga)
#define SET_BIT(reg, bit) ((reg) |= BIT(bit))
// Limpa um bit (desliga)
#define CLR_BIT(reg, bit) ((reg) &= ~BIT(bit))


// - Definições do LED1 (Pino 4 -> PD4)
#define LED1_DDR  DDRD
#define LED1_PORT PORTD
#define LED1_PIN  4    // Bit 4 do PORTD
#define LED1_SETUP() SET_BIT(LED1_DDR, LED1_PIN)
#define LED1_ON()    SET_BIT(LED1_PORT, LED1_PIN)
#define LED1_OFF()   CLR_BIT(LED1_PORT, LED1_PIN)

// - Mapeamento dos Bargraph para seus registradores e bits
#define BG0_DDR   DDRD
#define BG0_PORT  PORTD
#define BG0_PIN   3     // BG 2 (Pino 3 Arduino)
#define BG0_ON()  SET_BIT(BG0_PORT, BG0_PIN)
#define BG0_OFF() CLR_BIT(BG0_PORT, BG0_PIN)

#define BG1_DDR   DDRD
#define BG1_PORT  PORTD
#define BG1_PIN   2     // BG 3 (Pino 2 Arduino)
#define BG1_ON()  SET_BIT(BG1_PORT, BG1_PIN)
#define BG1_OFF() CLR_BIT(BG1_PORT, BG1_PIN)

#define BG2_DDR   DDRD
#define BG2_PORT  PORTD
#define BG2_PIN   1     // BG 4 (Pino 1 Arduino)
#define BG2_ON()  SET_BIT(BG2_PORT, BG2_PIN)
#define BG2_OFF() CLR_BIT(BG2_PORT, BG2_PIN)

#define BG3_DDR   DDRB
#define BG3_PORT  PORTB
#define BG3_PIN   0     // BG 5 (Pino 8 Arduino)
#define BG3_ON()  SET_BIT(BG3_PORT, BG3_PIN)
#define BG3_OFF() CLR_BIT(BG3_PORT, BG3_PIN)

#define BG4_DDR   DDRB
#define BG4_PORT  PORTB
#define BG4_PIN   1     // BG 6 (Pino 9 Arduino)
#define BG4_ON()  SET_BIT(BG4_PORT, BG4_PIN)
#define BG4_OFF() CLR_BIT(BG4_PORT, BG4_PIN)

#define BG5_DDR   DDRB
#define BG5_PORT  PORTB
#define BG5_PIN   2     // BG 7 (Pino 10 Arduino)
#define BG5_ON()  SET_BIT(BG5_PORT, BG5_PIN)
#define BG5_OFF() CLR_BIT(BG5_PORT, BG5_PIN)

#define BG6_DDR   DDRB
#define BG6_PORT  PORTB
#define BG6_PIN   3     // BG 8 (Pino 11 Arduino)
#define BG6_ON()  SET_BIT(BG6_PORT, BG6_PIN)
#define BG6_OFF() CLR_BIT(BG6_PORT, BG6_PIN)

#define BG7_DDR   DDRB
#define BG7_PORT  PORTB
#define BG7_PIN   4     // BG 9 (Pino 12 Arduino)
#define BG7_ON()  SET_BIT(BG7_PORT, BG7_PIN)
#define BG7_OFF() CLR_BIT(BG7_PORT, BG7_PIN)

// - Constantes (Macros)
#define numLeds 8
#define delayTime 100

// - Funções Auxiliares
void bargraph_on(int i) {
  switch (i) {
    case 0: BG0_ON(); break;
    case 1: BG1_ON(); break;
    case 2: BG2_ON(); break;
    case 3: BG3_ON(); break;
    case 4: BG4_ON(); break;
    case 5: BG5_ON(); break;
    case 6: BG6_ON(); break;
    case 7: BG7_ON(); break;
  }
}

void bargraph_off(int i) {
  switch (i) {
    case 0: BG0_OFF(); break;
    case 1: BG1_OFF(); break;
    case 2: BG2_OFF(); break;
    case 3: BG3_OFF(); break;
    case 4: BG4_OFF(); break;
    case 5: BG5_OFF(); break;
    case 6: BG6_OFF(); break;
    case 7: BG7_OFF(); break;
  }
}

// Acende todos os LEDs
void acenderTodos() {
  // Seta os 3 bits do PORTD
  BG0_PORT |= BIT(BG0_PIN) | BIT(BG1_PIN) | BIT(BG2_PIN);
  // Seta os 5 bits do PORTB
  BG3_PORT |= BIT(BG3_PIN) | BIT(BG4_PIN) | BIT(BG5_PIN) | BIT(BG6_PIN) | BIT(BG7_PIN);
}

// Apaga todos os LEDs
void apagarTodos() {
  // Limpa os 3 bits do PORTD
  BG0_PORT &= ~(BIT(BG0_PIN) | BIT(BG1_PIN) | BIT(BG2_PIN));
  // Limpa os 5 bits do PORTB
  BG3_PORT &= ~(BIT(BG3_PIN) | BIT(BG4_PIN) | BIT(BG5_PIN) | BIT(BG6_PIN) | BIT(BG7_PIN));
}

//--- SETUP ---
void setup() {
  // 1. Configura o LED1 (Pino 4)
  LED1_SETUP();

  // 2. Configura os LEDs do Bargraph
  // Seta os bits PD1, PD2, PD3 como SAÍDA
  DDRD |= BIT(BG0_PIN) | BIT(BG1_PIN) | BIT(BG2_PIN);
  // Seta os bits PB0, PB1, PB2, PB3, PB4 como SAÍDA
  DDRB |= BIT(BG3_PIN) | BIT(BG4_PIN) | BIT(BG5_PIN) | BIT(BG6_PIN) | BIT(BG7_PIN);

  // LEDs iniciarem apagados
  apagarTodos();
  LED1_OFF();
}

void loop() {
  
// - 1.1: Pisca o LED (PINO 4)
  
  // Pisca 3x rápdio
  for (int i = 0; i < 3; i++) {
    LED1_ON();
    delay(500);
    LED1_OFF();
    delay(500);
  }
  
  // Pisca 3x devagar
  for (int i = 0; i < 3; i++) {
    LED1_ON();
    delay(2000);
    LED1_OFF();
    delay(2000);
  }

// - 1.2 a) Acende o bargraph (2 -> 9) mantendo ligados 2x
  
  for (int j = 0; j < 2; j++) {
    for (int i = 0; i < numLeds; i++) {
      bargraph_on(i);
      delay(delayTime);
    }
    delay(500);
    apagarTodos();
    delay(500);
  }
  
// - 1.2 b) Acende o bargraph (9 -> 2) mantendo ligados 2x
 for (int j = 0; j < 2; j++) {
  for (int i = numLeds - 1; i >= 0; i--) {
    bargraph_on(i);
    delay(delayTime);
    }
  delay(500);
  apagarTodos();
  delay(500);
 }
// - 1.2 c) Um LED aceso por vez (9 -> 2) 

  for (int i = numLeds - 1; i >= 0; i--) {
    bargraph_on(i);
    delay(delayTime);
    bargraph_off(i);
  }
  delay(200);

// - 1.2 d) Ping-Pong

  // (Esquerda -> Direita, ou 2 -> 9)
  for (int i = 0; i < numLeds; i++) {
    bargraph_on(i);
    delay(delayTime);
    bargraph_off(i);
  }

  // (Direita -> Esquerda, ou 8 -> 3)
  for (int i = numLeds - 2; i > 0; i--) { 
    bargraph_on(i);
    delay(delayTime);
    bargraph_off(i);
  }
  delay(500);


// - 1.2 e) Todos acesos e pagar um por vez

  // (Apagando 2 -> 9)
  acenderTodos();
  delay(500);
  for (int i = 0; i < numLeds; i++) {
    bargraph_off(i);
    delay(delayTime);
  }
  delay(500);

  // (Apagando 9 -> 2)
  acenderTodos();
  delay(500);
  for (int i = numLeds - 1; i >= 0; i--) {
    bargraph_off(i);
    delay(delayTime);
  }
  delay(500);

// - 1.2 f) (2 -> 9), Pisca 5x e APAGA

  // 1. Acende (Bargraph 2 para 9)
  for (int i = 0; i < numLeds; i++) {
    bargraph_on(i);
    delay(delayTime);
  }

  // 2. Pisca todos 5 vezes
  for (int i = 0; i < 5; i++) {
    delay(200);
    apagarTodos();
    delay(200);
    acenderTodos();
  }

  // 3. Apaga
  delay(500);
  apagarTodos();
  delay(500);

  
// - 1.2 g) Todos acessos (9 -> 2), Apaga; Todos acesos (2 -> 9), Apaga

  // 1. Acende (9 -> 2)
  for (int i = numLeds - 1; i >= 0; i--) {
    bargraph_on(i);
    delay(delayTime);
  }
  delay(500);
  apagarTodos();
  delay(500);

  // 2. Acende (2 -> 9)
  for (int i = 0; i < numLeds; i++) {
    bargraph_on(i);
    delay(delayTime);
  }
  delay(500);
  apagarTodos();
  delay(500); 

// - 1.2 h) Contagem binária crescente (0-255), passo de 250ms

  // Loop de contagem de 0 a 255
  for (int count = 0; count <= 255; count++) {
    
    // Usa operador ternário ( ? : ) e macros de bit para setar cada LED
    // (BG0 = Bit 7) ... (BG7 = Bit 0)
    (count & BIT(7)) ? BG0_ON() : BG0_OFF(); // LED 2
    (count & BIT(6)) ? BG1_ON() : BG1_OFF(); // LED 3
    (count & BIT(5)) ? BG2_ON() : BG2_OFF(); // LED 4
    (count & BIT(4)) ? BG3_ON() : BG3_OFF(); // LED 5
    (count & BIT(3)) ? BG4_ON() : BG4_OFF(); // LED 6
    (count & BIT(2)) ? BG5_ON() : BG5_OFF(); // LED 7
    (count & BIT(1)) ? BG6_ON() : BG6_OFF(); // LED 8
    (count & BIT(0)) ? BG7_ON() : BG7_OFF(); // LED 9
    
    // Passo
    delay(250); 
  }
  
  delay(500);
  apagarTodos();
  delay(500); 

// - 1.2 i) Contagem binária decrescente (255 - 0), passo de 250ms
  
  // Loop de contagem de 255 a 0
  for (int count = 255; count >= 0; count--) {
    
    (count & BIT(7)) ? BG0_ON() : BG0_OFF(); // LED 2
    (count & BIT(6)) ? BG1_ON() : BG1_OFF(); // LED 3
    (count & BIT(5)) ? BG2_ON() : BG2_OFF(); // LED 4
    (count & BIT(4)) ? BG3_ON() : BG3_OFF(); // LED 5
    (count & BIT(3)) ? BG4_ON() : BG4_OFF(); // LED 6
    (count & BIT(2)) ? BG5_ON() : BG5_OFF(); // LED 7
    (count & BIT(1)) ? BG6_ON() : BG6_OFF(); // LED 8
    (count & BIT(0)) ? BG7_ON() : BG7_OFF(); // LED 9
    
    // Passo
    delay(250); 
  }
  
  delay(500);
  apagarTodos();

  // Espera um momento com tudo apagado antes de reiniciar o loop()
  delay(500);
}