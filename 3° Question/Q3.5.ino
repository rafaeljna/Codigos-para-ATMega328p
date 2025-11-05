/*
3. LENDO BOTÕES

3.5 Cada clique aumenta a frequência até ficar aceso fixo; se segurar por 5 s, apaga.
 */

// - Macros de Hardware
#define LED_DDR     DDRD
#define LED_PORT    PORTD
#define LED_BIT     PD3

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
const long HOLD_RESET_TEMPO = 5000;
const float INTERVALO_INICIAL = 500.0;
const float INTERVALO_MINIMO = 50.0;
const float FATOR_REDUCAO = 0.70;   // Reduz o tempo em 30% a cada clique

// - Variáveis de Estado
float intervaloAtual = INTERVALO_INICIAL;
bool sistemaLigado = false; // Começa desligado
bool ledFixo = false;       // Para saber se chegou ao máximo
int estadoLed = 0;   

// - Variáveis de Temporização
unsigned long millisAnteriorPisca = 0;
unsigned long millisInicioPress = 0; // Mede o tempo pressionado

// - Variáveis de Debounce
const long atrasoDebounce = 50;
unsigned long millisUltimoDebounce = 0;
int estadoEstavelBotao = 0; // 0=LOW (Pull-down)
int ultimoEstadoBotao = 0;
bool botaoEstaPressionado = false;


void setup() {
  CONFIG_SAIDA(LED_DDR, LED_BIT);
  CONFIG_ENTRADA(BOTAO_DDR, BOTAO_BIT);
  PIN_LOW(BOTAO_PORT, BOTAO_BIT); // Pull-up desligado
  
  // Começa tudo desligado/resetado
  resetarSistema();
}

void loop() {
  // Estado do botão (clique, pressionado, solto)
  gerirBotao();
  
  // lógica do LED (piscar, etc.)
  executarLogicaLed();
}

// Reset
void resetarSistema() {
  PIN_LOW(LED_PORT, LED_BIT);     
  sistemaLigado = false;          
  ledFixo = false;                
  intervaloAtual = INTERVALO_INICIAL;
  estadoLed = 0;       
}

// Verifica e processa o botão
void gerirBotao() {
  // - Lógica de Debounce
  int leitura = LER_PIN(BOTAO_PIN_REG, BOTAO_BIT);

  if (leitura != ultimoEstadoBotao) {
    millisUltimoDebounce = millis();
  }

  if ((millis() - millisUltimoDebounce) > atrasoDebounce) {
    
    // Se a leitura for diferente do estado estável guardado...
    if (leitura != estadoEstavelBotao) {
      estadoEstavelBotao = leitura; // atualiza o estado estável

      // - Lógica de Ação (Baseada na mudança)
      
      if (estadoEstavelBotao) { // Ação ao PRESSIONAR (LOW -> HIGH)
        botaoEstaPressionado = true;
        millisInicioPress = millis(); // Guarda o tempo em que foi pressionado

        if (sistemaLigado == false) {
          // Liga e começa a piscar.
          sistemaLigado = true;
          ledFixo = false;
          intervaloAtual = INTERVALO_INICIAL;
          estadoLed = 1; // Começa aceso
          PIN_HIGH(LED_PORT, LED_BIT);
          millisAnteriorPisca = millis();
        }
        
      } else { // Ação ao SOLTAR (HIGH -> LOW)
        botaoEstaPressionado = false;
        
        
        // clique < 5 segundos
        if (sistemaLigado == true) {
          aumentarFrequencia();
        }
      }
    }
  }
  
  ultimoEstadoBotao = leitura;
  
  // - Lógica de "Hold"
  
  // Se o botão está pressionado e já passaram 5 segundos...
  if (botaoEstaPressionado && sistemaLigado && (millis() - millisInicioPress >= HOLD_RESET_TEMPO)) {
    resetarSistema();
  }
}

// Aumentar a frequência
void aumentarFrequencia() {
  if (ledFixo) return; // Se já está no máximo, não faz nada

  intervaloAtual = intervaloAtual * FATOR_REDUCAO;
  
  if (intervaloAtual <= INTERVALO_MINIMO) {
    intervaloAtual = INTERVALO_MINIMO;
    ledFixo = true; // Atingiu o limite
    estadoLed = 1; 
    PIN_HIGH(LED_PORT, LED_BIT);
  }
}

// Pisca o LED
void executarLogicaLed() {
  if (sistemaLigado == false || ledFixo == true) {
    return;
  }
    if (millis() - millisAnteriorPisca >= (unsigned long)intervaloAtual) {
    millisAnteriorPisca = millis();
    
    // Inverte o estado
    estadoLed = !estadoLed;
    
    if (estadoLed) {
      PIN_HIGH(LED_PORT, LED_BIT);
    } else {
      PIN_LOW(LED_PORT, LED_BIT);
    }
  }
}