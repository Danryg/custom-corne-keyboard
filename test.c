#include <avr/io.h>

int main(void) {
  DDRD |= (1 << PD1);  // Set pin 2 as an output (PD1 corresponds to digital pin
                       // 2 on ATmega32U4)
  PORTD |= (1 << PD1); // Set pin 2 HIGH

  while (1) {
    // Loop indefinitely
  }
}
