#include "await.h"
#include "../Time/HAL/avrhwtimer0.h"
#include "../Time/HAL/avrhwtimer2.h"

using namespace BaseAVR::Time::HAL;

namespace BaseAVR {
	
	namespace Await {
		
		void ForMs(const tu_t& ms) {
			avrhwtimer0::Init();
			
			tu_t endTiks = ms + avrhwtimer0::GetCurrentTiks();
			
			while(avrhwtimer0::GetCurrentTiks() < endTiks);
		}
		
		void ForUs(const tu_t& us) {
			avrhwtimer2::Init();
			
			tu_t endTiks = us + avrhwtimer2::GetCurrentTiks();
			
			while(avrhwtimer2::GetCurrentTiks() < endTiks);
		}
	}
}