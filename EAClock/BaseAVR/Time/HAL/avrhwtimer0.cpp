#include "avrhwtimer0.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include "../../macrodef.h"

namespace BaseAVR {
	namespace Time {
		namespace HAL {

			tu_t avrhwtimer0::tiks = 0;
			l_t avrhwtimer0::handlerBlocker = TRUE;
			l_t avrhwtimer0::isInited = FALSE;
			_hwtimer_elapsed_callback avrhwtimer0::handler = nullptr;
			
			void avrhwtimer0::Block() {
				_SETL(TIMSK, OCIE0);
			}
			
			void avrhwtimer0::Resort() {
				_SETH(TIMSK, OCIE0);
			}
			
			void avrhwtimer0::HandlerBlock() {
				avrhwtimer0::handlerBlocker = FALSE;
			}
			
			void avrhwtimer0::HandlerResort() {
				avrhwtimer0::handlerBlocker = TRUE;
			}
			
			void avrhwtimer0::SetHandler(_hwtimer_elapsed_callback handler) {
				avrhwtimer0::Block();
				avrhwtimer0::handler = handler;
				avrhwtimer0::Resort();
			}

			void avrhwtimer0::ClearHandler() {
				avrhwtimer0::Block();
				handler = nullptr;
				avrhwtimer0::Resort();
			}

			l_t avrhwtimer0::HasHandler() {
				return handler != nullptr;
			}

			tu_t avrhwtimer0::GetCurrentTiks() {
				return tiks;
			}

			void avrhwtimer0::Raise() {
				
				tiks++;
				
				if (avrhwtimer0::handlerBlocker && (handler != nullptr))
				handler(tiks);
			}

			/*Init timer0 and start (~1ms)*/
			void avrhwtimer0::Init() {
				
				if(!avrhwtimer0::isInited) {
					
					avrhwtimer0::Block();
					
					//Setting compare value to 31 (992 us)
					_CL(TCNT0);
					_SETVAL(OCR0, 31);
					
					//Setting clock to F_CPU/256 and mode to CTC
					_CL(TCCR0);
					_SETH(TCCR0, WGM01);
					_SETH(TCCR0, CS02);
					
					//Setting interrupt flags to low value
					_SETL(TIFR, OCF0);
					_SETL(TIFR, TOV0);
					
					//Allowing interruption on CTC
					_SETL(TIMSK, TOIE0);
					_SETH(TIMSK, OCIE0);
					
					avrhwtimer0::isInited = TRUE;
				}
			}
		}
	}
}


ISR (TIMER0_COMP_vect) {
	BaseAVR::Time::HAL::avrhwtimer0::Raise();
}