#include "avrhwtimer2.h"
#include "../../macrodef.h"
#include <avr/io.h>
#include <avr/interrupt.h>

namespace BaseAVR {
	namespace Time {
		namespace HAL {
			
			tu_t avrhwtimer2::us = 0;
			l_t avrhwtimer2::handlerBlocker = FALSE;
			l_t avrhwtimer2::isInited = FALSE;
			_hwtimer_elapsed_callback avrhwtimer2::handler = nullptr;
			
			void avrhwtimer2::Block() {
				_SETL(TIMSK, OCIE2);
			}
			
			void avrhwtimer2::Resort() {
				_SETH(TIMSK, OCIE2);
			}
			
			void avrhwtimer2::HandlerBlock() {
				avrhwtimer2::handlerBlocker = FALSE;
			}
			
			void avrhwtimer2::HandlerResort() {
				avrhwtimer2::handlerBlocker = TRUE;
			}
			
			void avrhwtimer2::SetHandler(_hwtimer_elapsed_callback handler) {
				avrhwtimer2::Block();
				avrhwtimer2::handler = handler;
				avrhwtimer2::Resort();
			}

			void avrhwtimer2::ClearHandler() {
				avrhwtimer2::Block();
				handler = nullptr;
				avrhwtimer2::Resort();
			}

			l_t avrhwtimer2::HasHandler() {
				return handler != nullptr;
			}

			tu_t avrhwtimer2::GetCurrentTiks() {
				return us;
			}

			void avrhwtimer2::Raise() {
				
				us++;
				
				if(avrhwtimer2::handlerBlocker && (handler != nullptr))
				handler(us);
			}
			
			void avrhwtimer2::Init(){
				
				if(!avrhwtimer2::isInited) {
					
					avrhwtimer2::Block();
					
					//Setting compare value to 0x1 (1 us)
					_CL(TCNT2);
					_SETVAL(OCR2, 250);
					
					//Setting clock to F_CPU/8 and mode to CTC
					_CL(TCCR2);
					_SETH(TCCR2, WGM21);
					_SETH(TCCR2, CS21);
					
					//Setting interrupt flags to low value
					_SETL(TIFR, OCF2);
					_SETL(TIFR, TOV2);
					
					//Allowing interruption on CTC
					_SETL(TIMSK, TOIE2);
					_SETH(TIMSK, OCIE2);
					
					avrhwtimer2::isInited = TRUE;
				}
			}
		}
	}
}

ISR (TIMER2_COMP_vect){
	BaseAVR::Time::HAL::avrhwtimer2::Raise();
}