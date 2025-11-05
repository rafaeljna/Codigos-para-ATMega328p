/*
3. LENDO BOTÕES

3.4 Enquanto o botão estiver pressionado, LED pisca com frequência crescente até ficar aceso fixo; ao soltar, apaga.
 */

// - Macros de Hardware

// LED (Pino 3 -> PORTD, Bit 3)
#define LED_DDR     DDRD
#define LED_PORT    PORTD
#define LED_BIT     PD3 

// Botão (Pino 11 -> PORTB, Bit 3)
#define BOTAO_DDR     DDRB
#define BOTAO_PORT    PORTB
#define BOTAO_PIN_REG PINB
#define BOTAO_BIT     PB3 

// - Manipulação de Bits
#define CONFIG_SAIDA(ddr, bit)   ( (ddr) |= (1 << (bit)) )
#define CONFIG_ENTRADA(ddr, bit) ( (ddr) &= ~(1 << (bit)) )
#define PIN_HIGH(port, bit)      ( (port) |= (1 << (bit)) ) 
#define PIN_LOW(port, bit)       ( (port) &= ~(1 << (bit)) )
#define LER_PIN(pin_reg, bit)    ( (pin_reg) & (1 << (bit)) )

// - Constantes de Temporização
const float INTERVALO_INICIAL = 400.0; // Começa piscando 1x a cada ~800ms
const float INTERVALO_MINIMO = 30.0;   // Intervalo mais rápido
const float FATOR_REDUCAO = 0.85;      // Reduz o tempo em 15% a cada ciclo

// - Variáveis de Estado
unsigned long millisAnteriorPisca = 0;
float intervaloAtual = INTERVALO_INICIAL; // O timer que irá diminuir
bool ledFixo = false;          // Flag para saber se chegamos ao máximo
int estadoLed = 0;             // 0=LOW, 1=HIGH
bool botaoEstavaPressionado = false; // Detecção de borda (quando soltar)

void setup() {
  // LED como saída e desligado
  CONFIG_SAIDA(LED_DDR, LED_BIT);
  PIN_LOW(LED_PORT, LED_BIT);
  
  // Botão como entrada
  CONFIG_ENTRADA(BOTAO_DDR, BOTAO_BIT);
  // Pull-up interno desligado
  PIN_LOW(BOTAO_PORT, BOTAO_BIT); 
}

void loop() {
  // Lógica Pull-Down: Se LER_PIN > 0, está pressionado (HIGH)
  if (LER_PIN(BOTAO_PIN_REG, BOTAO_BIT)) {
    // Botão está pressionado
    processarLogicaPressionado();
    botaoEstavaPressionado = true;
  } else {
    // Botão NÃO pressionado (solto)
    
    // Verifica se acabou de ser solto
    if (botaoEstavaPressionado == true) {
      resetarLed();
    }
    botaoEstavaPressionado = false;
  }
}

// Reseta o LED e os timers
void resetarLed() {
  PIN_LOW(LED_PORT, LED_BIT);       // Desliga o LED
  intervaloAtual = INTERVALO_INICIAL; // Reseta o timer
  ledFixo = false;                  // Reseta o flag de "fixo"
  estadoLed = 0;                    // Reseta o estado do LED
}

// Piscar e acelerar
void processarLogicaPressionado() {
  
  // Manter aceso (se atingiu a velocidade máxima)
  if (ledFixo) {
    PIN_HIGH(LED_PORT, LED_BIT);
    return;
  }
  
  // Verifica se já deu o tempo de piscar
  if (millis() - millisAnteriorPisca >= (unsigned long)intervaloAtual) {
    
    // Reseta o cronômetro
    millisAnteriorPisca = millis();
    
    // Inverte o estado do LED
    estadoLed = !estadoLed;
    
    if (estadoLed) {
      // Se o estado é 1 (HIGH)
      PIN_HIGH(LED_PORT, LED_BIT);
    } else {
      // Se o estado é 0 (LOW)
      PIN_LOW(LED_PORT, LED_BIT);
      
      // Quando o LED apaga), o tempo do *próximo* ciclo diminui.
      intervaloAtual = intervaloAtual * FATOR_REDUCAO;
      
      // Verifica se atingiu o limite mínimo
      if (intervaloAtual <= INTERVALO_MINIMO) {
        intervaloAtual = INTERVALO_MINIMO;
        ledFixo = true; // Na próxima vez, ficará fixo
      }
    }
  }
}