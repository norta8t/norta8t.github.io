/*-------------------------------------------------------------------------*\
| Datei:        main.c
| Version:      1.0
| Projekt:      Einfuehrung in das Programmieren des Atmega16
| Beschreibung: Grundgeruest fuer die Anzeigebibliothek
| Schaltung:    MEGACARD V5.5
| Autor:
| Erstellung:
|
| Aenderung:
\*-------------------------------------------------------------------------*/
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "display.h"

volatile uint8_t CTC_val = 122;
volatile uint16_t current_capture = 0;

// ISR when rising edge is detected on ICP1
ISR(TIMER1_CAPT_vect){
	
	uint8_t sreg = SREG; // save global interrupt flag
	
	current_capture = ICR1;  // Read ICR1 (16 bit register)
	TCNT1 = 0;   // set counter value to 0
	
	SREG = sreg; // restore global interrupt flag
}

// ISR to generate output frequency on PB0
ISR(TIMER0_COMP_vect){
	PORTB ^= (1 << PB0); // Toggle PB0 to generate clock signal
}

void init (void)
{
   // Grundinitialisierungen
   DDRB |= (1 << PB0) | (1 << PB2); // Set Pin PB0 and PB2 as output
   
   DDRD &= ~(1 << PD6); // Set pin PD6 as input (ICP, timer1)
}

int main (void)
{
   init ();        // Aufruf der Grundinitialisierungen
   display_init(); // Initialisierung der Anzeige
   
   // Timer 0 initialisieren
   TCCR0 |= (1 << CS00) | (1 << CS01) | (1 << WGM01);  // set prescaler to 64 and CTC mode
   OCR0 = CTC_val; // assign global variable CTC_val to OCR0 register
   TIMSK |= (1 << OCIE0); // Enable timer0 compare match interrupt 
  
   // Timer 1 initialisieren
   TCCR1A &= ~((1 << WGM10) | (1 << WGM11));
   TCCR1B &= ~((1 << WGM12) | (1 << WGM13));
   TCCR1B |= (1 << ICES1) | (1 << CS10);  // ICP rising edge and no prescaler (1)
   
   TIMSK |= (1 << TICIE1);
   
   double time_ms = 1.832;
   display_printf_pos (0, 4, "Float: %.2f", time_ms);

   sei();
   
   float test_flt; // 32bit (4 byte)
   double test_dbl; // 32bit (4 byte)
   
   while (1)
   {
      // Hauptschleife
	  
	  test = 3.0 / 2.0;
	  
	  cli();
	  display_clear();
	  display_printf_pos (0, 4, "Float: %i", sizeof(float));
	  sei();

	  _delay_ms(1000);	  
   }

   return 0;
}
