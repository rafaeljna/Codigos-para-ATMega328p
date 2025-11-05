/*
3.10 Três botões + 3 LEDs + display:

Botão 1 → display mostra “1”; LEDs: [1-ON, 2-OFF, 3-piscando].
Botão 2 → display mostra “2”; LEDs: [1-piscando, 2-ON, 3-ON].
Botão 3 → display mostra “3”; LEDs: [1-OFF, 2-piscando, 3-OFF].
 */

// - Macros de Hardware

// - LEDs 1 e 3 Invertidos
#define LED_DDR     DDRD
#define LED_PORT    PORTD
#define LED1_BIT    PD4 
#define LED2_BIT    PD3 
#define LED3_BIT    PD2 

// Botões PORTB
#define BOTAO_DDR_B     DDRB
#define BOTAO_PORT_B    PORTB
#define BOTAO_PIN_REG   PINB
#define BOTAO1_BIT      PB0 
#define BOTAO2_BIT      PB1 
#define BOTAO3_BIT      PB2 

// Segmentos do Display
#define SEG_DDR_B     DDRB
#define SEG_PORT_B    PORTB
#define SEG_DDR_C     DDRC
#define SEG_PORT_C    PORTC

#define SEG_A_BIT     PB3
#define SEG_B_BIT     PB4
#define SEG_C_BIT     PB5
#define SEG_D_BIT     PC0
#define SEG_E_BIT     PC1 
#define SEG_F_BIT     PC2 
#define SEG_G_BIT     PC3

// - Manipulação de Bits
#define CONFIG_SAIDA(ddr, bit)   ( (ddr) |= (1 << (bit)) )
#define CONFIG_ENTRADA(ddr, bit) ( (ddr) &= ~(1 << (bit)) )
#define PIN_HIGH(port, bit)      ( (port) |= (1 << (bit)) ) 
#define PIN_LOW(port, bit)       ( (port) &= ~(1 << (bit)) )
#define LER_PIN(pin_reg, bit)    ( (pin_reg) & (1 << (bit)) )

// - Definições de Estado 
#define ESTADO_OFF 0
#define ESTADO_1   1
#define ESTADO_2   2
#define ESTADO_3   3

// - Variáveis de Temporização
unsigned long millisAnteriorPisca = 0;
const long intervaloPisca = 400; 
bool estadoLedPisca = false; 

// - Variáveis do Display
void displayApaga();
void displayNumero(int num);

void setup() {
  // LEDs como saída
  CONFIG_SAIDA(LED_DDR, LED1_BIT);
  CONFIG_SAIDA(LED_DDR, LED2_BIT);
  CONFIG_SAIDA(LED_DDR, LED3_BIT);
  
  // Botões como entrada
  CONFIG_ENTRADA(BOTAO_DDR_B, BOTAO1_BIT);
  CONFIG_ENTRADA(BOTAO_DDR_B, BOTAO2_BIT);
  CONFIG_ENTRADA(BOTAO_DDR_B, BOTAO3_BIT);
  PIN_LOW(BOTAO_PORT_B, BOTAO1_BIT);
  PIN_LOW(BOTAO_PORT_B, BOTAO2_BIT);
  PIN_LOW(BOTAO_PORT_B, BOTAO3_BIT);
  
  // Display 
  CONFIG_SAIDA(SEG_DDR_B, SEG_A_BIT); 
  CONFIG_SAIDA(SEG_DDR_B, SEG_B_BIT); 
  CONFIG_SAIDA(SEG_DDR_B, SEG_C_BIT); 
  CONFIG_SAIDA(SEG_DDR_C, SEG_D_BIT); 
  CONFIG_SAIDA(SEG_DDR_C, SEG_E_BIT); 
  CONFIG_SAIDA(SEG_DDR_C, SEG_F_BIT); 
  CONFIG_SAIDA(SEG_DDR_C, SEG_G_BIT); 

  // LEDs desligado
  displayApaga();
  PIN_LOW(LED_PORT, LED1_BIT); // Agora desliga PD4
  PIN_LOW(LED_PORT, LED2_BIT); // Desliga PD3
  PIN_LOW(LED_PORT, LED3_BIT); // Agora desliga PD2
}

void loop() {
  // - Atualiza o timer do Pisca-Pisca
  if (millis() - millisAnteriorPisca >= intervaloPisca) {
    millisAnteriorPisca = millis();
    estadoLedPisca = !estadoLedPisca; 
  }

  // - Lê os botões e define o estado
  bool b1 = LER_PIN(BOTAO_PIN_REG, BOTAO1_BIT);
  bool b2 = LER_PIN(BOTAO_PIN_REG, BOTAO2_BIT);
  bool b3 = LER_PIN(BOTAO_PIN_REG, BOTAO3_BIT);
  
  int estadoAtual = ESTADO_OFF;
  if (b3) {
    estadoAtual = ESTADO_3;
  } else if (b2) {
    estadoAtual = ESTADO_2;
  } else if (b1) {
    estadoAtual = ESTADO_1;
  }

  // -Atualiza saídas (LEDs e display)
  static int estadoAnterior = -1; 
  if (estadoAtual != estadoAnterior) {
    switch (estadoAtual) {
      case ESTADO_1: displayNumero(1); break;
      case ESTADO_2: displayNumero(2); break;
      case ESTADO_3: displayNumero(3); break;
      default:       displayApaga();   break;
    }
    estadoAnterior = estadoAtual;
  }
  
  // Atualiza os LEDs
  switch (estadoAtual) {
    
    case ESTADO_1: // B1: [1-ON (PD4), 2-OFF (PD3), 3-piscando (PD2)]
      PIN_HIGH(LED_PORT, LED1_BIT); // PD4
      PIN_LOW(LED_PORT, LED2_BIT);  // PD3
      if (estadoLedPisca) PIN_HIGH(LED_PORT, LED3_BIT); // PD2
      else PIN_LOW(LED_PORT, LED3_BIT);
      break;
      
    case ESTADO_2: // B2: [1-piscando (PD4), 2-ON (PD3), 3-ON (PD2)]
      if (estadoLedPisca) PIN_HIGH(LED_PORT, LED1_BIT); // PD4
      else PIN_LOW(LED_PORT, LED1_BIT);
      PIN_HIGH(LED_PORT, LED2_BIT); // PD3
      PIN_HIGH(LED_PORT, LED3_BIT); // PD2
      break;
      
    case ESTADO_3: // B3: [1-OFF (PD4), 2-piscando (PD3), 3-OFF (PD2)]
      PIN_LOW(LED_PORT, LED1_BIT);  // PD4
      if (estadoLedPisca) PIN_HIGH(LED_PORT, LED2_BIT); // PD3
      else PIN_LOW(LED_PORT, LED2_BIT);
      PIN_LOW(LED_PORT, LED3_BIT);  // PD2
      break;
      
    default: // ESTADO_OFF
      PIN_LOW(LED_PORT, LED1_BIT); // PD4
      PIN_LOW(LED_PORT, LED2_BIT); // PD3
      PIN_LOW(LED_PORT, LED3_BIT); // PD2
      break;
  }
}

// - Funções de Display 

void displayApaga() {
  PIN_LOW(SEG_PORT_B, SEG_A_BIT); 
  PIN_LOW(SEG_PORT_B, SEG_B_BIT); 
  PIN_LOW(SEG_PORT_B, SEG_C_BIT); 
  PIN_LOW(SEG_PORT_C, SEG_D_BIT); 
  PIN_LOW(SEG_PORT_C, SEG_E_BIT); 
  PIN_LOW(SEG_PORT_C, SEG_F_BIT); 
  PIN_LOW(SEG_PORT_C, SEG_G_BIT); 
}

void displayNumero(int num) {
  switch (num) {
    case 1: // 0110000 (A, B, C, D, E, F, G)
      PIN_LOW(SEG_PORT_B, SEG_A_BIT);  
      PIN_HIGH(SEG_PORT_B, SEG_B_BIT); 
      PIN_HIGH(SEG_PORT_B, SEG_C_BIT); 
      PIN_LOW(SEG_PORT_C, SEG_D_BIT);  
      PIN_LOW(SEG_PORT_C, SEG_E_BIT);  
      PIN_LOW(SEG_PORT_C, SEG_F_BIT);  
      PIN_LOW(SEG_PORT_C, SEG_G_BIT);  
      break;
      
    case 2: // 1101101
      PIN_HIGH(SEG_PORT_B, SEG_A_BIT); 
      PIN_HIGH(SEG_PORT_B, SEG_B_BIT); 
      PIN_LOW(SEG_PORT_B, SEG_C_BIT);  
      PIN_HIGH(SEG_PORT_C, SEG_D_BIT); 
      PIN_HIGH(SEG_PORT_C, SEG_E_BIT); 
      PIN_LOW(SEG_PORT_C, SEG_F_BIT);  
      PIN_HIGH(SEG_PORT_C, SEG_G_BIT); 
      break;
      
    case 3: // 1111001
      PIN_HIGH(SEG_PORT_B, SEG_A_BIT); 
      PIN_HIGH(SEG_PORT_B, SEG_B_BIT); 
      PIN_HIGH(SEG_PORT_B, SEG_C_BIT); 
      PIN_HIGH(SEG_PORT_C, SEG_D_BIT); 
      PIN_LOW(SEG_PORT_C, SEG_E_BIT);  
      PIN_LOW(SEG_PORT_C, SEG_F_BIT);  
      PIN_HIGH(SEG_PORT_C, SEG_G_BIT); 
      break;
      
    default:
      displayApaga();
      break;
  }
}