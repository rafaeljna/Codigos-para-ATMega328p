/*
3.7 Dois botões + 2 LEDs:

Botão 1 → LED1 aceso, LED2 piscando.
Botão 2 → inverter funções.
Ambos → apaga.
 */

// -Macros de Hardware
#define LED_DDR     DDRD
#define LED_PORT    PORTD
#define LED1_BIT    PD3 
#define LED2_BIT    PD4 

#define BOTAO_DDR     DDRB
#define BOTAO_PORT    PORTB
#define BOTAO_PIN_REG PINB 
#define BOTAO1_BIT    PB3
#define BOTAO2_BIT    PB4

// - Manipulação de Bits
#define CONFIG_SAIDA(ddr, bit)   ( (ddr) |= (1 << (bit)) )
#define CONFIG_ENTRADA(ddr, bit) ( (ddr) &= ~(1 << (bit)) )
#define PIN_HIGH(port, bit)      ( (port) |= (1 << (bit)) ) 
#define PIN_LOW(port, bit)       ( (port) &= ~(1 << (bit)) )
#define LER_PIN(pin_reg, bit)    ( (pin_reg) & (1 << (bit)) )

// - Variáveis de Temporização 
unsigned long millisAnteriorPisca = 0;
const long intervaloPisca = 400; 
int estadoLedPisca = 0; 

void setup() {
  // LEDs como saída e desligados
  CONFIG_SAIDA(LED_DDR, LED1_BIT);
  CONFIG_SAIDA(LED_DDR, LED2_BIT);
  PIN_LOW(LED_PORT, LED1_BIT);
  PIN_LOW(LED_PORT, LED2_BIT);
  
  // Botões como entrada
  CONFIG_ENTRADA(BOTAO_DDR, BOTAO1_BIT);
  CONFIG_ENTRADA(BOTAO_DDR, BOTAO2_BIT);
  PIN_LOW(BOTAO_PORT, BOTAO1_BIT); // Pull-up desligado
  PIN_LOW(BOTAO_PORT, BOTAO2_BIT); // Pull-up desligado
}

void loop() {
  if (millis() - millisAnteriorPisca >= intervaloPisca) {
    millisAnteriorPisca = millis();
    estadoLedPisca = !estadoLedPisca; 
  }
  
  // - Ler os Botões
  bool b1_pressionado = LER_PIN(BOTAO_PIN_REG, BOTAO1_BIT);
  bool b2_pressionado = LER_PIN(BOTAO_PIN_REG, BOTAO2_BIT);

  // Verificamos o caso "Ambos Pressionados"
  if (b1_pressionado && b2_pressionado) {
    PIN_LOW(LED_PORT, LED1_BIT);
    PIN_LOW(LED_PORT, LED2_BIT);
    
  } else if (b1_pressionado) {
    // -Apenas Botão 1 Pressionado

    // LED 1 ACESO
    PIN_HIGH(LED_PORT, LED1_BIT);
    // LED 2 PISCA (recebe o estado do relógio)
    if (estadoLedPisca) {
      PIN_HIGH(LED_PORT, LED2_BIT);
    } else {
      PIN_LOW(LED_PORT, LED2_BIT);
    }
    
  } else if (b2_pressionado) {
    // - Apenas Botão 2 Pressionado) ---
    // LED 1 PISCA 
    if (estadoLedPisca) {
      PIN_HIGH(LED_PORT, LED1_BIT);
    } else {
      PIN_LOW(LED_PORT, LED1_BIT);
    }
    
    // LED 2 ACESO
    PIN_HIGH(LED_PORT, LED2_BIT);
    
  } else {
    // - Nenhum Botão Pressionado
    PIN_LOW(LED_PORT, LED1_BIT);
    PIN_LOW(LED_PORT, LED2_BIT);
  }
}