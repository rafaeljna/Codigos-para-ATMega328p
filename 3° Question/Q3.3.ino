/*
3. LENDO BOTÕES

3.1 LED acende com 1º clique, apaga com 2º.
3.2 LED alterna entre: ON → Piscar → Piscar rápido → OFF.
3.3 Sequência de 3 LEDs (1-2-3); ao apertar novamente, inverter (3-2-1).
 */

// - Macros de Hardware

// LEDs (PORTD)
#define LED_DDR     DDRD
#define LED_PORT    PORTD

// Botão (PORTB)
#define BOTAO_DDR     DDRB   
#define BOTAO_PORT    PORTB  
#define BOTAO_PIN_REG PINB   

// Bits específicos
#define BOTAO_BIT   PB3 

// Bits dos LEDs
const uint8_t bitsLeds[] = {PD2, PD3, PD4};

// - Manipulação de Bits
#define CONFIG_SAIDA(ddr, bit)   ( (ddr) |= (1 << (bit)) )
#define CONFIG_ENTRADA(ddr, bit) ( (ddr) &= ~(1 << (bit)) )
#define PIN_HIGH(port, bit)      ( (port) |= (1 << (bit)) ) 
#define PIN_LOW(port, bit)       ( (port) &= ~(1 << (bit)) )
#define LER_PIN(pin_reg, bit)    ( (pin_reg) & (1 << (bit)) )

// - Variáveis de Lógica
int ledAtualIndex = 0; 
bool direcaoAvanco = true;
bool maquinaAtiva = false;

// - Variáveis de Temporização-
unsigned long millisSequenciaAnterior = 0;
const long intervaloSequencia = 300; 
unsigned long millisUltimoDebounce = 0;
const long atrasoDebounce = 50;
int estadoEstavelBotao = 0; 
int ultimoEstadoBotao = 0;


void setup() {
  // LEDs como saída e desligados
  for (int i = 0; i < 3; i++) {
    CONFIG_SAIDA(LED_DDR, bitsLeds[i]);
    PIN_LOW(LED_PORT, bitsLeds[i]); 
  }

  // Botão como entrada
  CONFIG_ENTRADA(BOTAO_DDR, BOTAO_BIT);
  
  // Pull-up desligado
  PIN_LOW(BOTAO_PORT, BOTAO_BIT);
}

void loop() {
  controlarSequencia();
  verificarBotao();
}

// - Sequência de LEDs
void controlarSequencia() {
  if (maquinaAtiva == false) {
    return;
  }
  
  if (millis() - millisSequenciaAnterior >= intervaloSequencia) {
    
    millisSequenciaAnterior = millis();
    PIN_LOW(LED_PORT, bitsLeds[ledAtualIndex]);

    if (direcaoAvanco == true) {
      ledAtualIndex++;
      if (ledAtualIndex > 2) {
        ledAtualIndex = 0; 
      }
    } else {
      ledAtualIndex--;
      if (ledAtualIndex < 0) {
        ledAtualIndex = 2; 
      }
    }
    PIN_HIGH(LED_PORT, bitsLeds[ledAtualIndex]);
  }
}


// - Verifica o botão
void verificarBotao() {
  int leitura = LER_PIN(BOTAO_PIN_REG, BOTAO_BIT);

  if (leitura != ultimoEstadoBotao) {
    millisUltimoDebounce = millis();
  }

  if ((millis() - millisUltimoDebounce) > atrasoDebounce) {
    
    if (leitura != estadoEstavelBotao) {
      estadoEstavelBotao = leitura; 

      if (estadoEstavelBotao) { // Agir se HIGH
        
        if (maquinaAtiva == false) {
          maquinaAtiva = true;
          ledAtualIndex = 0;
          direcaoAvanco = true; 
          
          PIN_HIGH(LED_PORT, bitsLeds[ledAtualIndex]);
          millisSequenciaAnterior = millis();
          
        } else {
          direcaoAvanco = !direcaoAvanco; 
        }
      }
    }
  }
  
  ultimoEstadoBotao = leitura;
}