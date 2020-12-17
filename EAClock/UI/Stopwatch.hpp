#ifndef __STOPWATCH_H__
#define __STOPWATCH_H__
#include "UIEntityTime.hpp"
#include "Timer.hpp"

namespace EAClock {
	namespace UI {
		
		//Represents stopwatch UI and logic
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
			
			static void OnDownLongClick(const Button& sender) {
				instance.TransitTo(Timer::GetInstace()->GetHandle());
			}
			
			Stopwatch(const TimeSpan& startVal, pbutton_t up, pbutton_t down) : UIEntityTime(FALSE, up, down) {
				UIEntityTime::SetTimeValue(startVal);
				firstStart = TRUE;
			}
			
			public:
			
			void OnFirstStart() {
				this->Reset();
			}
			
			void OnFocus() override {
				
				_up->SetClickHandler(Stopwatch::OnUpClick);
				_up->SetLongClickHandler(Stopwatch::OnUpLongClick);
				_down->SetClickHandler(Stopwatch::OnDownLongClick);
				
				if(firstStart) {
					this->OnFirstStart();
					firstStart = FALSE;
				}
			}
			
			void OnFocusLost() override {
				this->Stop();
			}
			
			static Stopwatch* InitAndGetInstance(const TimeSpan& startVal, pbutton_t up, pbutton_t down) {
				instance = Stopwatch(startVal, up, down);
				
				return &instance;
			}
			
			static Stopwatch* GetInstance() {
				return &instance;
			}
		};
		
		Stopwatch Stopwatch::instance(TimeSpan(0), nullptr, nullptr);
	}
}

#endif //__STOPWATCH_H__
