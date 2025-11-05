/*
3. LENDO BOTÕES

3.2 LED alterna entre: ON → Piscar → Piscar rápido → OFF.

*/

// - Macros de Hardware
// Mapeamento do LED (Pino 3 -> PORTD, Bit 3)
#define LED_DDR     DDRD
#define LED_PORT    PORTD
#define LED_BIT     PD3    // Bit 3 no PORTD

// Mapeamento do Botão (Pino 11 -> PORTB, Bit 3)
#define BOTAO_DDR     DDRB
#define BOTAO_PORT    PORTB
#define BOTAO_PIN_REG PINB   // Registrador de LEITURA (PINB)
#define BOTAO_BIT     PB3    //  Bit 3 no PORTB

// Manipulação de Bits
#define CONFIG_SAIDA(ddr, bit)   ( (ddr) |= (1 << (bit)) )
#define CONFIG_ENTRADA(ddr, bit) ( (ddr) &= ~(1 << (bit)) )
#define PIN_HIGH(port, bit)      ( (port) |= (1 << (bit)) ) 
#define PIN_LOW(port, bit)       ( (port) &= ~(1 << (bit)) )

// Retorna 0 (LOW) ou um valor > 0 (HIGH)
#define LER_PIN(pin_reg, bit)    ( (pin_reg) & (1 << (bit)) )


// - Definição dos Estados
#define ESTADO_DESLIGADO 0
#define ESTADO_LIGADO    1
#define ESTADO_PISCA_LENTO 2
#define ESTADO_PISCA_RAPIDO 3

// - Variáveis de Controle de Estado
int estadoMaquina = ESTADO_DESLIGADO;
int estadoLed = 0; //LOW

// - Temporização - Pisca
unsigned long millisAnteriorPisca = 0;
const long intervaloPiscaLento = 500;
const long intervaloPiscaRapido = 150;

// - Temporização - Debounce do Botão
const long atrasoDebounce = 50; 
unsigned long millisUltimoDebounce = 0;
// 0 para LOW - estado solto/pull-down
int estadoEstavelBotao = 0; 
int ultimoEstadoBotao = 0;


void setup() {
  CONFIG_SAIDA(LED_DDR, LED_BIT);
  
  CONFIG_ENTRADA(BOTAO_DDR, BOTAO_BIT);
  
  // Pull-Down
  PIN_LOW(BOTAO_PORT, BOTAO_BIT);
  
  // LED começar desligado
  PIN_LOW(LED_PORT, LED_BIT); 
}

void loop() {
  verificarBotao();
  executarMaquinaEstados();
}

// - Leitura do botão
void verificarBotao() {
  // Lê o estado ATUAL do pino
  int leitura = LER_PIN(BOTAO_PIN_REG, BOTAO_BIT);

  if (leitura != ultimoEstadoBotao) {
    millisUltimoDebounce = millis();
  }

  if ((millis() - millisUltimoDebounce) > atrasoDebounce) {
    
    if (leitura != estadoEstavelBotao) {
      
      estadoEstavelBotao = leitura; // Guarda o novo estado estável

      // Lógica Pull-Down:
      if (estadoEstavelBotao) { // (Valor > 0)
        avancarEstado();
      }
    }
  }
  
  ultimoEstadoBotao = leitura;
}

// - Avança o estado do botão
void avancarEstado() {
  estadoMaquina = (estadoMaquina + 1) % 4; 

  if (estadoMaquina == ESTADO_DESLIGADO) {
    estadoLed = 0; // LOW
  } else {
    estadoLed = 1; // HIGH
  }
  
  if (estadoLed) {
    PIN_HIGH(LED_PORT, LED_BIT);
  } else {
    PIN_LOW(LED_PORT, LED_BIT);
  }
  
  millisAnteriorPisca = millis();
}

// - Função que cuida do LED
void executarMaquinaEstados() {
  unsigned long millisAtual = millis();

  switch (estadoMaquina) {
    
    case ESTADO_DESLIGADO:
    case ESTADO_LIGADO:
      break;

    case ESTADO_PISCA_LENTO:
      if (millisAtual - millisAnteriorPisca >= intervaloPiscaLento) {
        millisAnteriorPisca = millisAtual;
        
        // Inverte o estado
        estadoLed = !estadoLed; // (0 vira 1, 1 vira 0)
        
        if (estadoLed) {
          PIN_HIGH(LED_PORT, LED_BIT);
        } else {
          PIN_LOW(LED_PORT, LED_BIT);
        }
      }
      break;

    case ESTADO_PISCA_RAPIDO:
      if (millisAtual - millisAnteriorPisca >= intervaloPiscaRapido) {
        millisAnteriorPisca = millisAtual;
        
        estadoLed = !estadoLed;
        
        if (estadoLed) {
          PIN_HIGH(LED_PORT, LED_BIT);
        } else {
          PIN_LOW(LED_PORT, LED_BIT);
        }
      }
      break;
  }
}