#include "HWBase.h"
#include <avr/io.h>

namespace BaseAVR {
	namespace HAL {
		namespace hwio_base {
			
			HWComponent	A(&PORTA, &PINA, &DDRA);
			HWComponent	B(&PORTB, &PINB, &DDRB);
			HWComponent	C(&PORTC, &PINC, &DDRC);
			HWComponent	D(&PORTD, &PIND, &DDRD);
			
		}
	}
}