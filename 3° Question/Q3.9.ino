/*
3.9 Três botões + 4 LEDs:

Botão 1 → todos acesos.
Botão 2 → apenas LED1 e LED2.
Botão 3 → apenas LED3 e LED4.
Botão 1 + Botão 3 → todos apagam.
 */

// - Macros de Hardware
// LEDs (PORTD)
#define LED_DDR     DDRD
#define LED_PORT    PORTD
#define LED1_BIT    PD2 // Pino 2
#define LED2_BIT    PD3 // Pino 3
#define LED3_BIT    PD4 // Pino 4
#define LED4_BIT    PD5 // Pino 5

// Botões (PORTB)
#define BOTAO_DDR     DDRB
#define BOTAO_PORT    PORTB
#define BOTAO_PIN_REG PINB
#define BOTAO1_BIT    PB3 
#define BOTAO2_BIT    PB4 
#define BOTAO3_BIT    PB0 

// - Manipulação de Bits
#define CONFIG_SAIDA(ddr, bit)   ( (ddr) |= (1 << (bit)) )
#define CONFIG_ENTRADA(ddr, bit) ( (ddr) &= ~(1 << (bit)) )
#define PIN_HIGH(port, bit)      ( (port) |= (1 << (bit)) ) 
#define PIN_LOW(port, bit)       ( (port) &= ~(1 << (bit)) )
#define LER_PIN(pin_reg, bit)    ( (pin_reg) & (1 << (bit)) )

void setup() {
  // Configura LED como saída
  CONFIG_SAIDA(LED_DDR, LED1_BIT);
  CONFIG_SAIDA(LED_DDR, LED2_BIT);
  CONFIG_SAIDA(LED_DDR, LED3_BIT);
  CONFIG_SAIDA(LED_DDR, LED4_BIT);
  
  // Configura Botões como entrada
  CONFIG_ENTRADA(BOTAO_DDR, BOTAO1_BIT);
  CONFIG_ENTRADA(BOTAO_DDR, BOTAO2_BIT);
  CONFIG_ENTRADA(BOTAO_DDR, BOTAO3_BIT);
  
  // Pull-ups desligados
  PIN_LOW(BOTAO_PORT, BOTAO1_BIT);
  PIN_LOW(BOTAO_PORT, BOTAO2_BIT);
  PIN_LOW(BOTAO_PORT, BOTAO3_BIT);
  
  // LEDs desligados
  PIN_LOW(LED_PORT, LED1_BIT);
  PIN_LOW(LED_PORT, LED2_BIT);
  PIN_LOW(LED_PORT, LED3_BIT);
  PIN_LOW(LED_PORT, LED4_BIT);
}

void loop() {
  bool b1 = LER_PIN(BOTAO_PIN_REG, BOTAO1_BIT);
  bool b2 = LER_PIN(BOTAO_PIN_REG, BOTAO2_BIT);
  bool b3 = LER_PIN(BOTAO_PIN_REG, BOTAO3_BIT);
  
  // 2. Aplica a lógica de prioridade.  
  if (b1 && b3) {
    // ESTADO 4: (B1 + B3) -> Todos apagam
    PIN_LOW(LED_PORT, LED1_BIT);
    PIN_LOW(LED_PORT, LED2_BIT);
    PIN_LOW(LED_PORT, LED3_BIT);
    PIN_LOW(LED_PORT, LED4_BIT);
    
  } else if (b1) {
    // ESTADO 1 -> Todos acesos
    PIN_HIGH(LED_PORT, LED1_BIT);
    PIN_HIGH(LED_PORT, LED2_BIT);
    PIN_HIGH(LED_PORT, LED3_BIT);
    PIN_HIGH(LED_PORT, LED4_BIT);
    
  } else if (b2) {
    // ESTADO 2 -> Apenas 1 e 2
    PIN_HIGH(LED_PORT, LED1_BIT);
    PIN_HIGH(LED_PORT, LED2_BIT);
    PIN_LOW(LED_PORT, LED3_BIT);
    PIN_LOW(LED_PORT, LED4_BIT);

  } else if (b3) {
    // ESTADO 3 -> Apenas 3 e 4
    PIN_LOW(LED_PORT, LED1_BIT);
    PIN_LOW(LED_PORT, LED2_BIT);
    PIN_HIGH(LED_PORT, LED3_BIT);
    PIN_HIGH(LED_PORT, LED4_BIT);

  } else {
    // Nenhum botão pressionado
    PIN_LOW(LED_PORT, LED1_BIT);
    PIN_LOW(LED_PORT, LED2_BIT);
    PIN_LOW(LED_PORT, LED3_BIT);
    PIN_LOW(LED_PORT, LED4_BIT);
  }
}