#ifndef __CLOCK_HPP__
#define __CLOCK_HPP__
#include "UIEntityTime.hpp"
#include "../BaseAVR/Audio/HAL/avrhwaudio.h"

#include "Stopwatch.hpp"
#include "TimeSelector.hpp"

using namespace BaseAVR::Audio::HAL;

#define ALARM_FREQ 1000

namespace EAClock {
	namespace UI {

		enum class SelectionTarget : u8_t {
			None,
			Time,
			AlarmTime
		};

		class Clock : public UIEntityTime {
			
			private:
			
			static Clock instance;
			
			l_t _alarmOn;
			TimeSpan _alarmTime;
			fsize_t _transition;
			SelectionTarget _selectionTarget;
			
			pbutton_t _select;
			
			Clock(
			const TimeSpan& initTime,
			const TimeSpan& alarmTime,
			const l_t& state,
			pbutton_t select,
			pbutton_t up,
			pbutton_t down)
			: UIEntityTime(state, up, down) {
				
				this->SetTimeValue(initTime);
				_alarmTime = alarmTime;
				_alarmOn = FALSE;
				_transition = 0;
				_selectionTarget = SelectionTarget::None;
				_select = select;
				
			}
			
			void GiveControlTo(const fsize_t& target) {
				_transition = target;
			}
			
			void RestoreHandlers() {
				_up->SetClickHandler(OnUpClick);
				_down->SetClickHandler(OnDownClick);
				
				_up->SetLongClickHandler(OnUpLongClick);
				
				_select->SetLongClickHandler(OnSelectLongClick);
			}
			
			void TurnAlarm(const l_t& status) {
				_alarmOn = status;
			}
			
			void BlinkCentralPoint() {
				lcd8::PointAt(lcd8position::Second, _alarmOn ? TRUE : _timeValue.GetSeconds() & 1);
			}
			
			l_t IsSelectionPreformed() {
				return _selectionTarget != SelectionTarget::None;
			}
			
			void Select(const SelectionTarget& target) {
				
				auto selector = TimeSelector::GetInstance();
				
				TimeSpan pivot, temp;
				
				switch (target)	{
					case SelectionTarget::Time:
					temp = this->GetTimeValue();
					break;
					
					case  SelectionTarget::AlarmTime:
					temp = _alarmTime;
					break;
					
					default:
					return;
					break;
				}
				
				pivot.AddHours(temp.GetHours())
				.AddMinutes(temp.GetMinutes());
				
				selector->Select(
				pivot,
				SelectionPair::Low,
				ShowMode::hh_mm,
				instance.GetHandle());
				
				instance.GiveControlTo(selector->GetHandle());
				instance._selectionTarget = target;
				this->Stop();
			}
			
			void OnSelectionEnd() {
				
				auto selector = TimeSelector::GetInstance();
				auto selectionResult = selector->GetTimeValue();
				
				switch (_selectionTarget) {
					case SelectionTarget::Time:
					UIEntityTime::SetTimeValue(selectionResult);
					break;
					
					case SelectionTarget::AlarmTime:
					_alarmTime = selectionResult;
					break;
					
					default:
					break;
				}
				
				_selectionTarget = SelectionTarget::None;
				this->Start();
			}
			
			static void OnSelectLongClick(const Button& sender) {
				instance.GiveControlTo(Stopwatch::GetInstance()->GetHandle());
			}
			
			static void OnUpClick(const Button& sender) {
				instance.TurnAlarm(!instance._alarmOn);
			}
			
			static void OnUpLongClick(const Button& sender) {
				instance.Select(SelectionTarget::AlarmTime);
			}
			
			static void OnDownClick(const Button& sender) {
				instance.Select(SelectionTarget::Time);
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
			
			void OnUiUpdate() override {
				this->BlinkCentralPoint();
			}
			
			void OnFocus() override {
				
				if(this->IsSelectionPreformed()) {
					this->OnSelectionEnd();
				}
				
				this->RestoreHandlers();
				UIEntityTime::SetShowMode(ShowMode::hh_mm);
			}
			
			void OnFocusLost() override {
				
				lcd8::PointAt(lcd8position::Second, FALSE);
				this->GiveControlTo(0);
			}
			
			static Clock* GetInstance(
			const TimeSpan& initTime,
			const TimeSpan& alarmTime,
			const l_t& state,
			pbutton_t select,
			Button* up,
			Button* down) {
				
				instance = Clock(initTime, alarmTime, state, select, up, down);
				return &instance;
			}
			
			static Clock* GetInstance() {
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
		
		Clock Clock::instance(TimeSpan(0), TimeSpan(0), FALSE, nullptr, nullptr, nullptr);
	}
}

#endif //__CLOCK_H__
