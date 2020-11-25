#ifndef _AVR_HW_TIMER0_H_
#define _AVR_HW_TIMER0_H_
#include "../../globaldef.h"
#include "hwtypes.h"

namespace BaseAVR {
	namespace Time {
		namespace HAL {

			class avrhwtimer0 {
				private:
				avrhwtimer0();
				avrhwtimer0(const avrhwtimer0&);

				static tu_t tiks;
				static _hwtimer_elapsed_callback handler;
				static l_t handlerBlocker;
				
				static l_t isInited;
				
				public:

				static void Init();
				static tu_t GetCurrentTiks();
				static void SetHandler(_hwtimer_elapsed_callback);
				static void ClearHandler();
				static l_t HasHandler();
				static void Raise();
				
				static void Block();
				static void Resort();
				
				static void HandlerBlock();
				static void HandlerResort();
			};
		}
	}
}
#endif // !_AVR_HW_TIMER0_H_
