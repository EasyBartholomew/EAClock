#ifndef __UIENTITYTIME_HPP__
#define __UIENTITYTIME_HPP__
#include "../BaseAVR/Time/TimeSpan.h"
#include "UIEntity.hpp"


using namespace BaseAVR::Time;


namespace EAClock {
	namespace UI {
		
		//Represents time show mode
		enum class ShowMode : u8_t {
			
			//Represents hours and minutes show mode
			hh_mm,
			
			//Represents minutes and seconds show mode
			mm_ss,
			
			//Represents seconds and tenth hundredth parts of second show mode
			ss_th
		};
		
		
		//Represents base class for Time based UI entity
		class UIEntityTime : public UIEntity {
			
			private:
			
			ShowMode _mode;
			TimeSpan _timeValue;
			
			protected:
			
			
			UIEntityTime(const l_t& state, pbutton_t up, pbutton_t down) : UIEntity(state, up, down)
			{ }
			
			UIEntityTime(const l_t& state) : UIEntityTime(state, nullptr, nullptr)
			{ }
			
			UIEntityTime() : UIEntityTime(FALSE)
			{ }
			
			void SetTimeValue(const TimeSpan& time) {
				_timeValue = time;
			}
			
			u8_t * _GetBuffer() const override {
				switch(this->GetShowMode()) {
					
					case ShowMode::hh_mm: {
						_buffer[0] = _timeValue.GetHours() / 10;
						_buffer[1] = _timeValue.GetHours() % 10;
						_buffer[2] = _timeValue.GetMinutes() / 10;
						_buffer[3] = _timeValue.GetMinutes() % 10;
						
					}break;
					
					case ShowMode::mm_ss: {
						_buffer[0] = _timeValue.GetMinutes() / 10;
						_buffer[1] = _timeValue.GetMinutes() % 10;
						_buffer[2] = _timeValue.GetSeconds() / 10;
						_buffer[3] = _timeValue.GetSeconds() % 10;
						
					}break;
					
					case ShowMode::ss_th: {
						_buffer[0] = _timeValue.GetSeconds() / 10;
						_buffer[1] = _timeValue.GetSeconds() % 10;
						_buffer[2] = _timeValue.GetMilliseconds() / 100;
						_buffer[3] = (_timeValue.GetMilliseconds() / 10) % 10;
					}break;
					
				}
				
				return UIEntity::_GetBuffer();
			}
			
			public:
			
			void SetShowMode(const ShowMode& mode) {
				_mode = mode;
			}
			
			ShowMode GetShowMode() const {
				return _mode;
			}
			
			virtual TimeSpan GetTimeValue() const {
				return _timeValue;
			}
			
			virtual l_t IsTimeEntity() const {
				return TRUE;
			}
			
			virtual void OnUpdate(const TimeSpan& delta) {
				if(this->IsStarted()) {
					_timeValue = _timeValue + delta;
				}
			}
			
			virtual void OnShowModeChanged(const ShowMode& newMode) {
				this->SetShowMode(newMode);
			}
		};
		
		typedef UIEntityTime* pui_entitytime;
	}
}


#endif //__UIENTITYTIME_HPP__
