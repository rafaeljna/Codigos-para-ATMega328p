/*
3. LENDO BOTÕES

3.1 LED acende com 1º clique, apaga com 2º.
 */

// -Macros de Hardware
// Mapeamento do LED (Pino 3 -> PORTD, Bit 3)
#define LED_DDR     DDRD   
#define LED_PORT    PORTD  
#define LED_PIN_REG PIND   
#define LED_BIT     PD3    

// Mapeamento do Botão (Pino 11 -> PORTB, Bit 3)
#define BOTAO_DDR     DDRB   
#define BOTAO_PORT    PORTB  
#define BOTAO_PIN_REG PINB   
#define BOTAO_BIT     PB3    

// - Manipulação de Bits

#define CONFIG_SAIDA(ddr, bit)   ( (ddr) |= (1 << (bit)) )
#define CONFIG_ENTRADA(ddr, bit) ( (ddr) &= ~(1 << (bit)) )
#define PIN_HIGH(port, bit)      ( (port) |= (1 << (bit)) ) 
#define PIN_LOW(port, bit)       ( (port) &= ~(1 << (bit)) )
#define INVERTER_PIN(pin_reg, bit) ( (pin_reg) = (1 << (bit)) ) 
#define LER_PIN(pin_reg, bit)    ( (pin_reg) & (1 << (bit)) )

// Começa em 1 (HIGH - estado de repouso do PULL-UP)
uint8_t estadoBotaoAnterior = 1;

void setup() {
  
  // LED (Pino 3) como SAÍDA
  CONFIG_SAIDA(LED_DDR, LED_BIT);
  
  // Botão (Pino 11) como ENTRADA
  CONFIG_ENTRADA(BOTAO_DDR, BOTAO_BIT);
  
  // Ativa o resistor de PULL-UP interno do Pino 11
  PIN_HIGH(BOTAO_PORT, BOTAO_BIT); 

  // Pino em HIGH para que o LED comece DESLIGADO.
  PIN_HIGH(LED_PORT, LED_BIT); 
}

void loop() {
  
  // Lê o estado ATUAL do botão
  uint8_t leituraAtual = LER_PIN(BOTAO_PIN_REG, BOTAO_BIT);

  // (Pressionado = LOW, Solto = HIGH)
  if (!leituraAtual && estadoBotaoAnterior) {
    
    // Inverte o estado do LED (Pino 3)
    INVERTER_PIN(LED_PIN_REG, LED_BIT);
  }

  // Salva o estado atual para a próxima iteração
  estadoBotaoAnterior = leituraAtual;
  
  // Delay para Debounce
  delay(50);
}