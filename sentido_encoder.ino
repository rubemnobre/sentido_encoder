#include<avr/interrupt.h>
#define PB4 0b00010000
#define PB3 0b00001000
#define PB2 0b00000100
#define PCIE 0b00100000
#define PCINT3 0b00001000
#define PCINT4 0b00010000
#define GIE 0b10000000

int main() {
  GIMSK |= PCIE; // Habilita pin change interrupt
  PCMSK |= PCINT3 | PCINT4; // Habilita PCINT para os pinos 2 e 3
  
  PORTB |= PB3 | PB4; // Seta pinos 2 e 3 como high antes de configurar como input para habilitar pull up
  DDRB |= PB2; // Configura o pino 7 como output e os outros como input

  SREG |= GIE; // Habilita interrupts globalmente

  while(1);
}

volatile uint8_t ab_ant = 0, ab = 0;
int a = 0;
volatile uint8_t dir = 0, dir_ant = 0;

ISR(PCINT0_vect){ // Interrupção de pin change
  ab = PINB & (PB3 | PB4);

  // dir = 0 => 00, 01, 11, 10, 00
  // dir = 1 => 00, 10, 11, 01, 00
  uint16_t x = (ab_ant >> 1)|(ab >> 3); //A'B'AB
  switch(x){ // Lógica para definição do sentido de rotação, baseada em (ab_anterior, ab_atual)
    case 0b0001: // (00, 01) => dir = 0
      dir = 0;
      break;
    case 0b0010: // (00, 10) => dir = 1
      dir = 1;
      break;
    case 0b0100: // (00, 10) => dir = 1
      dir = 1;
      break;
    case 0b0111: // (01, 11) => dir = 0
      dir = 0;
      break;
    case 0b1000: // (10, 00) => dir = 0
      dir = 0;
      break;
    case 0b1011: // (10, 11) => dir = 1
      dir = 1;
      break;
    case 0b1101: // (11, 01) => dir = 1
      dir = 1;
      break;
    case 0b1110: // (11, 10) => dir = 0
      dir = 0;
      break;
  }

  if(dir != dir_ant){ // Contar quatro bordas na mesma direção para desconsiderar pulsos incompletos
    a = 0;
  }else{
    a++;
    if(a >= 4){
      PORTB = dir?(PORTB & ~PB2):(PORTB | PB2); // Atribui o valor da direção ao pino PB2
    }
  }

  dir_ant = dir;
  ab_ant = ab;
}
