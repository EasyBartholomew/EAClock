#ifndef __STOPWATCH_H__
#define __STOPWATCH_H__
#include "UIEntityTime.hpp"


namespace EAClock {
	namespace UI {
		
		class Stopwatch	: public UIEntityTime {
			
			private:
			
			void Reset() {
				UIEntityTime::SetTimeValue(TimeSpan(0));
			}
			
			void OnUpLongClick(const Button& sender) {
				Reset();
			}
			
			void OnUpClick(const Button& sender) {
				
				if(this->IsStarted()) {
					this->Stop();
				}
				else {
					this->Start();
				}
			}
			
			public:
			
			Stopwatch(const TimeSpan& startVal, pbutton_t up) : UIEntityTime(FALSE, up, nullptr) {
				UIEntityTime::SetTimeValue(startVal);
				
				_up->SetClickHandler(OnUpClick);
				_up->SetLongClickHandler(OnUpLongClick);
				
			}
			
			void OnUpdate(const TimeSpan& delta) {
				
				if(this->IsStarted()) {
					_timeValue = _timeValue + delta;
				}
			}
		};
	}
}

#endif //__STOPWATCH_H__
