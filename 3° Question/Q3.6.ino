/*
3. LENDO BOTÕES

3.6 Dois botões + 1 LED: LED acende se qualquer um for pressionado; apaga se ambos forem pressionados.
 */

// - Macros de Hardware

// LED
#define LED_DDR     DDRD
#define LED_PORT    PORTD
#define LED_BIT     PD3 // Bit 3

// Botões
#define BOTAO_DDR     DDRB
#define BOTAO_PORT    PORTB
#define BOTAO_PIN_REG PINB // Registrador de Leitura

// Bits específicos
#define BOTAO1_BIT    PB3
#define BOTAO2_BIT    PB4 

// - Manipulação de Bits
#define CONFIG_SAIDA(ddr, bit)   ( (ddr) |= (1 << (bit)) )
#define CONFIG_ENTRADA(ddr, bit) ( (ddr) &= ~(1 << (bit)) )
#define PIN_HIGH(port, bit)      ( (port) |= (1 << (bit)) ) 
#define PIN_LOW(port, bit)       ( (port) &= ~(1 << (bit)) )
#define LER_PIN(pin_reg, bit)    ( (pin_reg) & (1 << (bit)) )

void setup() {
  // LEDcomo saída e desligado
  CONFIG_SAIDA(LED_DDR, LED_BIT);
  PIN_LOW(LED_PORT, LED_BIT);
  
  // Botão 1 como entrada
  CONFIG_ENTRADA(BOTAO_DDR, BOTAO1_BIT);
  // Pull-up interno desligado
  PIN_LOW(BOTAO_PORT, BOTAO1_BIT);

  // Botão 2 como entrada
  CONFIG_ENTRADA(BOTAO_DDR, BOTAO2_BIT);
  // Pull-up desligado
  PIN_LOW(BOTAO_PORT, BOTAO2_BIT);
}

void loop() {
  // Lê os dois botões.
  bool estadoBotao1 = LER_PIN(BOTAO_PIN_REG, BOTAO1_BIT);
  bool estadoBotao2 = LER_PIN(BOTAO_PIN_REG, BOTAO2_BIT);
  
  // Lógica dos botões
  if (estadoBotao1 ^ estadoBotao2) {
    // Se APENAS UM estiver pressionado, liga o LED
    PIN_HIGH(LED_PORT, LED_BIT);
  } else {
    // Se AMBOS ou NENHUM estiverem pressionados, desliga o LED
    PIN_LOW(LED_PORT, LED_BIT);
  }
  
}