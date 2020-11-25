#ifndef __UIENTITYTIME_H__
#define __UIENTITYTIME_H__
#include "../BaseAVR/Time/TimeSpan.h"
#include "UIEntity.hpp"

using namespace BaseAVR::Time;

namespace EAClock {
	namespace UI {
		class UIEntityTime : public UIEntity {
			
			protected:
			
			TimeSpan _timeValue;
			
			UIEntityTime(const l_t& state, Button* up, Button* down) : UIEntity(state, up, down)
			{ }
			
			UIEntityTime(const l_t& state) : UIEntityTime(state, nullptr, nullptr)
			{ }
			
			UIEntityTime() : UIEntityTime(FALSE)
			{ }
			
			void SetTimeValue(const TimeSpan& time) {
				_timeValue = time;
			}
			
			public:
			
			virtual u16_t GetValue() const {
				return 0;
			}
			
			virtual TimeSpan GetTimeValue() const {
				return _timeValue;
			}
			
			virtual l_t IsTimeEntity() const {
				return TRUE;
			}
			
			virtual void OnUpdate(const TimeSpan&) = 0;
		};
		
		typedef UIEntityTime* pui_entitytime;
	}
}


#endif //__UIENTITYTIME_H__
