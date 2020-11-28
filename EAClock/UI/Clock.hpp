#ifndef __CLOCK_HPP__
#define __CLOCK_HPP__
#include "UIEntityTime.hpp"
#include "../BaseAVR/Audio/HAL/avrhwaudio.h"

using namespace BaseAVR::Audio::HAL;

#define ALARM_FREQ 1000
#define UI_STOPWATCH 1

namespace EAClock {
	namespace UI {

		class Clock : public UIEntityTime {
			
			private:
			
			static Clock instance;
			
			l_t _alarmOn;
			TimeSpan _alarmTime;
			
			fsize_t _transition;
			
			pbutton_t _select;
			
			Clock(
			const TimeSpan& initTime,
			const l_t& state,
			pbutton_t select,
			pbutton_t up,
			pbutton_t down)
			: UIEntityTime(state, up, down) {
				
				this->SetTimeValue(initTime);
				_alarmTime = TimeSpan(0);
				_alarmOn = FALSE;
				_transition = 0;
				
				_select = select;
				
			}
			
			void GiveControlTo(const fsize_t& target) {
				_transition = target;
			}
			
			void RestoreHandlers() {
				_up->SetClickHandler(OnUpClick);
				_down->SetClickHandler(OnDownClick);
				_select->SetLongClickHandler(OnSelectLongClick);
				
				_up->SetLongClickHandler(OnUpLongClick);
			}
			
			static void OnSelectLongClick(const Button& sender) {
				instance.GiveControlTo(UI_STOPWATCH);
			}
			
			static void OnUpClick(const Button& sender) {
				
			}
			
			static void OnUpLongClick(const Button& sender) {
				
			}
			
			static void OnDownClick(const Button& sender) {
				
			}
			
			static void OnAlarmStopRinging(const Button& sender) {
				avrhwaudio::Stop();
				
				//Add restoring handlers here
				instance.RestoreHandlers();
			}
			
			public:
			
			void OnUpdate(const TimeSpan& delta) override {
				
				if(_alarmOn) {
					if(_alarmTime == this->GetTimeValue()) {
						
						//ring code here
						
						_up->SetLongClickHandler(nullptr);
						_down->SetLongClickHandler(nullptr);
						_select->SetLongClickHandler(nullptr);
						_up->SetClickHandler(Clock::OnAlarmStopRinging);
						_down->SetClickHandler(Clock::OnAlarmStopRinging);
						
						
						avrhwaudio::Start(ALARM_FREQ);
					}
				}
				
				UIEntityTime::OnUpdate(delta);
				
				auto days = _timeValue.GetDays();
				
				if(days > 0) {
					_timeValue.SubtractDays(days);
				}
			}
			
			void OnFocus() override {
				
				this->RestoreHandlers();
				UIEntityTime::OnFocus();
			}
			
			void OnFocusLost() override {
				
				UIEntityTime::OnFocusLost();
			}
			
			static Clock* GetInstance(
			const TimeSpan& initTime,
			const l_t& state,
			pbutton_t select,
			Button* up,
			Button* down) {
				
				instance = Clock(initTime, state, select, up, down);
				return &instance;
			}
			
			l_t IsMainUIEntity() const override {
				return TRUE;
			}
			
			l_t IsTransitionTarget() const override {
				return _transition != 0;
			}
			
			fsize_t GetTransitionTarget() const override {
				return _transition;
			}
			
		};
		
		Clock Clock::instance(TimeSpan(0), FALSE, nullptr, nullptr, nullptr);
	}
}

#endif //__CLOCK_H__
