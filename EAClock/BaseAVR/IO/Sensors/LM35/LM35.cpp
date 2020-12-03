#include "LM35.h"
#include "lm35config.h"
#include "../../../Time/await.h"
#include <math.h>

using namespace BaseAVR;

namespace BaseAVR {
	namespace IO {
		namespace Sensors {
			
			l_t LM35::is_inited = FALSE;
			
			void LM35::Init() {
				
				if(LM35::is_inited)
				return;
				
				_CL(ADCSRA);
				
				//Enable the ADC
				_SETH(ADCSRA, ADEN);
				_SETH(ADCSRA, ADPS0);
				_SETH(ADCSRA, ADPS1);
				_SETH(ADCSRA, ADPS2);
				
				//Setting line to input
				_SETL(LM35_DDR, LM35_LINE);
				
				//Clear the data registers
				_CL(ADCH);
				_CL(ADCL);
				
				//Setting IDC ref as AVCC pin i.e. Vcc 5 V
				_SETH(ADMUX, REFS0);
				_SETL(ADMUX, REFS1);
				
				//Setting right shift
				_SETL(ADMUX, ADLAR);
				
				//Setting line to check
				ADMUX |= 0x0f & LM35_LINE;
				
				is_inited = TRUE;
			}
			
			u16_t LM35::MeasureAndGetCValue() {
				
				//Starting convert
				_SETH(ADCSRA, ADSC);
				
				//Await for convert to end
				while(_ISL(ADCSRA, ADIF));
				
				u16_t adcCode = ADCL;
				adcCode |= (ADCH << 8);
				
				return LM35::_dataconvert(adcCode);
			}
			
			u16_t LM35::_dataconvert(const u16_t& code) {
				constexpr auto mul = 150.0/306.9 * 10.0;
				return (u16_t)(round(mul*code));
			}
		}
	}
}