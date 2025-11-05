/*
3.8 Dois botões + 3 LEDs:

Botão 1 → sequência 1-2-3.
Botão 2 → sequência 3-2-1.
Ambos → apagam.
 */

// - Macros de Hardware
// LEDs em PORTD
#define LED_DDR  DDRD
#define LED_PORT PORTD
// Botões em PORTB
#define BOTAO_DDR DDRB
#define BOTAO_PORT PORTB
#define BOTAO_PIN_REG PINB 

// Bits dos LEDs
const uint8_t bitsLeds[] = {PD2, PD3, PD4};
// Bits dos Botões
#define BOTAO1_BIT PB3 
#define BOTAO2_BIT PB4 

// - Macros de Manipulação de Bits
#define CONFIG_SAIDA(ddr, bit)   ( (ddr) |= (1 << (bit)) )
#define CONFIG_ENTRADA(ddr, bit) ( (ddr) &= ~(1 << (bit)) )
#define PIN_HIGH(port, bit)      ( (port) |= (1 << (bit)) ) 
#define PIN_LOW(port, bit)       ( (port) &= ~(1 << (bit)) )
#define LER_PIN(pin_reg, bit)    ( (pin_reg) & (1 << (bit)) )

// -Variáveis de Estado e Temporização
unsigned long millisAnteriorSeq = 0;
const long intervaloSequencia = 300;

int ledAtualIndex = 0; // Guarda qual LED está aceso
int estadoAnterior = 0; // Guarda o estado do loop anterior

#define ESTADO_OFF 0
#define ESTADO_FRT 1 // Avanço
#define ESTADO_REV 2 // Reverso

void setup() {
  //  Botões como entrada
  CONFIG_ENTRADA(BOTAO_DDR, BOTAO1_BIT);
  CONFIG_ENTRADA(BOTAO_DDR, BOTAO2_BIT);
  PIN_LOW(BOTAO_PORT, BOTAO1_BIT); // Pull-up desligado
  PIN_LOW(BOTAO_PORT, BOTAO2_BIT); 
  
  // LEDs como saída
  for(int i = 0; i < 3; i++) {
    CONFIG_SAIDA(LED_DDR, bitsLeds[i]);
  }
  
  // Garante que tudo comece desligado
  apagarTodosLeds();
}

// Desligar todos os LEDs
void apagarTodosLeds() {
  PIN_LOW(LED_PORT, bitsLeds[0]);
  PIN_LOW(LED_PORT, bitsLeds[1]);
  PIN_LOW(LED_PORT, bitsLeds[2]);
}

void loop() {
  // - Ler os Botões
  bool b1_pressionado = LER_PIN(BOTAO_PIN_REG, BOTAO1_BIT);
  bool b2_pressionado = LER_PIN(BOTAO_PIN_REG, BOTAO2_BIT);
  
  // - Determinar o Estado Atual 
  int estadoAtual;
  
  if (b1_pressionado && b2_pressionado) {
    estadoAtual = ESTADO_OFF;
  } else if (b1_pressionado) {
    estadoAtual = ESTADO_FRT;
  } else if (b2_pressionado) {
    estadoAtual = ESTADO_REV;
  } else {
    estadoAtual = ESTADO_OFF;
  }

  // - Processar a Lógica de Estado

  // Verifica se o estado mudou
  if (estadoAtual != estadoAnterior) {
    // Reseta se o estado mudou
    apagarTodosLeds();
    
    // Se o novo estado for FRT (começa do LED 0)
    if (estadoAtual == ESTADO_FRT) {
      ledAtualIndex = 0;
      PIN_HIGH(LED_PORT, bitsLeds[ledAtualIndex]);
    }
    // Se o NOVO estado for REV (começa do LED 2)
    else if (estadoAtual == ESTADO_REV) {
      ledAtualIndex = 2;
      PIN_HIGH(LED_PORT, bitsLeds[ledAtualIndex]);
    }
    
    // Reseta o temporizador
    millisAnteriorSeq = millis();
  }

  // -Executa a sequência
  
  if (estadoAtual == ESTADO_OFF) {
    apagarTodosLeds(); // Garante que fiquem apagados
  } 
  // Verificamos o tempo
  else if (millis() - millisAnteriorSeq >= intervaloSequencia) {
    
    // Reseta o relógio
    millisAnteriorSeq = millis();
    
    // Apaga o LED atual
    PIN_LOW(LED_PORT, bitsLeds[ledAtualIndex]);
    
    // Calcula o próximo LED
    if (estadoAtual == ESTADO_FRT) {
      ledAtualIndex++;
      if (ledAtualIndex > 2) {
        ledAtualIndex = 0; // Loop 0, 1, 2, 0...
      }
    } else { // (estadoAtual == ESTADO_REV)
      ledAtualIndex--;
      if (ledAtualIndex < 0) {
        ledAtualIndex = 2; // Loop 2, 1, 0, 2...
      }
    }
    // Acende o novo LED
    PIN_HIGH(LED_PORT, bitsLeds[ledAtualIndex]);
  }
  // - Guardar o estado para a próxima volta
  estadoAnterior = estadoAtual;
}