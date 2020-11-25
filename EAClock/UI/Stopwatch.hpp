#ifndef __STOPWATCH_H__
#define __STOPWATCH_H__
#include "UIEntityTime.hpp"


namespace EAClock {
	namespace UI {
		
		class Stopwatch	: public UIEntityTime {
			
			private:
			
			static Stopwatch instance;
			
			l_t firstStart;
			
			void Reset() {
				UIEntityTime::SetTimeValue(TimeSpan(0));
				UIEntityTime::SetShowMode(ShowMode::ss_th);
			}
			
			static void OnUpLongClick(const Button& sender) {
				instance.Reset();
			}
			
			static void OnUpClick(const Button& sender) {
				
				if(instance.IsStarted()) {
					instance.Stop();
				}
				else {
					instance.Start();
				}
			}
			
			Stopwatch(const TimeSpan& startVal, pbutton_t up) : UIEntityTime(FALSE, up, nullptr) {
				UIEntityTime::SetTimeValue(startVal);
				firstStart = TRUE;
			}
			
			public:
			
			void OnFirstStart() {
				this->Reset();
			}
			
			void OnFocus() {
				_up->SetClickHandler((_ButtonEventCallBack)Stopwatch::OnUpClick);
				_up->SetLongClickHandler((_ButtonEventCallBack)Stopwatch::OnUpLongClick);
				
				if(firstStart) {
					this->OnFirstStart();
					firstStart = FALSE;
				}
				
				UIEntityTime::OnFocus();
			}
			
			static Stopwatch* GetInstance(const TimeSpan& startVal, pbutton_t up) {
				instance = Stopwatch(startVal, up);
				
				return &instance;
			}
		};
		
		Stopwatch Stopwatch::instance(TimeSpan(0), nullptr);
	}
}

#endif //__STOPWATCH_H__
