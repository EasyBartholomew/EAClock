#ifndef AVRHWTIMER2_H_
#define AVRHWTIMER2_H_

#include "../../globaldef.h"
#include "hwtypes.h"

namespace BaseAVR {
	namespace Time {
		namespace HAL {

			class avrhwtimer2 {
				private:
				avrhwtimer2();
				avrhwtimer2(const avrhwtimer2&);

				static tu_t us;
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

#endif /* AVRHWTIMER2_H_ */
