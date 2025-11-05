/*
2. DISPLAYS DE 7 SEGMENTOS

Usar 2 displays multiplexados:

Display 1: contagem crescente de 0 a F.

Display 2: contagem decrescente de F a 0.

Mapeamento de Pinos :
 * Display 1:
 * Segmento A -> Pino 7 (PD7)
 * Segmento B -> Pino 6 (PD6)
 * Segmento C -> Pino 5 (PD5)
 * Segmento D -> Pino 4 (PD4)
 * Segmento E -> Pino 3 (PD3)
 * Segmento F -> Pino 2 (PD2)
 * Segmento G -> Pino 1 (PD1)
 * Segmento DP -> Pino 0 (PD0)
 * * Display 2:
 * Segmento A -> Pino 8  (PB0)
 * Segmento B -> Pino 9  (PB1)
 * Segmento C -> Pino 10 (PB2)
 * Segmento D -> Pino 11 (PB3)
 * Segmento E -> Pino 12 (PB4)
 * Segmento F -> Pino 13 (PB5)
 * Segmento G -> Pino 14 (PC0)
 * Segmento DP -> Pino 15 (PC1)
 */

// --- Macros para Registradores ---
#define DISPLAY1_PORT PORTD
#define DISPLAY1_DDR  DDRD

#define DISPLAY2_PORTB PORTB
#define DISPLAY2_DDRB  DDRB
#define DISPLAY2_PORTC PORTC
#define DISPLAY2_DDRC  DDRC

// Padrões de bits para Cátodo Comum (1 = aceso, 0 = apagado)
// Ordem dos bits: A B C D E F G DP
// (Bit 7)        (Bit 0)
const byte hexPatterns[16] = {
  0b11111100, // 0 (0xFC)
  0b01100000, // 1 (0x60)
  0b11011010, // 2 (0xDA)
  0b11110010, // 3 (0xF2)
  0b01100110, // 4 (0x66)
  0b10110110, // 5 (0xB6)
  0b10111110, // 6 (0xBE)
  0b11100000, // 7 (0xE0)
  0b11111110, // 8 (0xFE)
  0b11110110, // 9 (0xF6)
  0b11101110, // A (0xEE)
  0b00111110, // b (0x3E)
  0b10011100, // C (0x9C)
  0b01111010, // d (0x7A)
  0b10011110, // E (0x9E)
  0b10001110  // F (0x8E)
};

void setup() {
  // - Configuração dos Registradores DDR

  // Display 1 (Pinos 0-7):
  // 0xFF = 0b11111111 -> Define todos os 8 pinos do PORTD como SAÍDA
  DISPLAY1_DDR = 0xFF;

  // Display 2 (Pinos 8-15):
  // Pinos 8-13 (PB0 a PB5)
  // 0x3F = 0b00111111 -> Define os bits 0-5 do PORTB como SAÍDA
  DISPLAY2_DDRB |= 0x3F;

  // Pinos 14-15 (PC0 a PC1)
  // 0x03 = 0b00000011 -> Define os bits 0-1 do PORTC como SAÍDA
  DISPLAY2_DDRC |= 0x03;

  // --- Exibição Inicial ---
  // Exibe '0' no Display 1
  DISPLAY1_PORT = hexPatterns[0];
  
  // Exibe 'F' (15) no Display 2
  setDisplay2(hexPatterns[15]);

  delay(2000);
}

void loop() {
  // Loop de 0 a 15
  for (int i = 0; i <= 15; i++)
  {
    // Contador reverso para o Display 2
    int j = 15 - i;

    // - Atualização do Display 1
    // Escreve o padrão de bits diretamente no registrador PORTD
    DISPLAY1_PORT = hexPatterns[i];

    // - Atualização do Display 2
    // Chama a função auxiliar que faz a manipulação de bits
    setDisplay2(hexPatterns[j]);

    delay(2000);
  }
}


void setDisplay2(byte pattern) {
  byte portb_val = 0;
  
  // Mapeia os bits do padrão para os bits do PORTB
  // bitRead(pattern, 7) -> Pega o bit 7 (A)
  // bitWrite(portb_val, 0, ...) -> Escreve no bit 0 (PB0 / Pino 8)
  bitWrite(portb_val, 0, bitRead(pattern, 7)); // Seg A (bit 7) -> PB0 (Pino 8)
  bitWrite(portb_val, 1, bitRead(pattern, 6)); // Seg B (bit 6) -> PB1 (Pino 9)
  bitWrite(portb_val, 2, bitRead(pattern, 5)); // Seg C (bit 5) -> PB2 (Pino 10)
  bitWrite(portb_val, 3, bitRead(pattern, 4)); // Seg D (bit 4) -> PB3 (Pino 11)
  bitWrite(portb_val, 4, bitRead(pattern, 3)); // Seg E (bit 3) -> PB4 (Pino 12)
  bitWrite(portb_val, 5, bitRead(pattern, 2)); // Seg F (bit 2) -> PB5 (Pino 13)

  // --- Manipulação para PORTC (Pinos 14-15) ---  
  byte portc_val = 0;
  
  bitWrite(portc_val, 0, bitRead(pattern, 1)); // Seg G (bit 1) -> PC0 (Pino 14)
  bitWrite(portc_val, 1, bitRead(pattern, 0)); // Seg DP (bit 0) -> PC1 (Pino 15)

  // (DISPLAY2_PORTB & 0b11000000) -> Lê o PORTB e mantém (zera o resto)
  // (... | portb_val) -> Combina com os novos valores (bits 0-5)
  DISPLAY2_PORTB = (DISPLAY2_PORTB & 0b11000000) | portb_val;

  // (DISPLAY2_PORTC & 0b11111100) -> Lê o PORTC e mantém (zera o resto)
  // (... | portc_val) -> Combina com os novos valores (bits 0-1)
  DISPLAY2_PORTC = (DISPLAY2_PORTC & 0b11111100) | portc_val;
}