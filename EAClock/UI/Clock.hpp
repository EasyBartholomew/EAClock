#ifndef __CLOCK_HPP__
#define __CLOCK_HPP__
#include "UIEntityTime.hpp"

#include "ButtonHelper.hpp"
#include "Stopwatch.hpp"
#include "TimeSelector.hpp"
#include "TemperatureSensor.hpp"


using namespace BaseAVR::Audio::HAL;


#define ALARM_FREQ (freq_t)1000


namespace EAClock {
	namespace UI {

		//Represents a clock selection target
		enum class SelectionTarget : u8_t {
			
			//Represents no target
			None,
			
			//Represents clock time
			Time,
			
			//Represents alarm time
			AlarmTime
		};
		
		//Represents Clock UI and logic
		class Clock : public UIEntityTime {
			
			private:
			
			static Clock instance;
			
			l_t _alarmOn;
			TimeSpan _alarmTime;
			SelectionTarget _selectionTarget;
			
			pbutton_t _select;
			
			void BackupLastButtons() {
				ButtonHelper::_upClickHandlerBackup = UIEntityTime::_up->GetClickHandler();
				ButtonHelper::_downClickHandlerBackup = UIEntityTime::_down->GetClickHandler();
				
				ButtonHelper::_upLongClickHandlerBackup = UIEntityTime::_up->GetLongClickHandler();
				ButtonHelper::_downLongClickHandlerBackup = UIEntityTime::_down->GetLongClickHandler();
			}
			
			void RestoreLastButtons() {
				UIEntityTime::_up->SetClickHandler(ButtonHelper::_upClickHandlerBackup);
				UIEntityTime::_down->SetClickHandler(ButtonHelper::_downClickHandlerBackup);
				
				UIEntityTime::_up->SetLongClickHandler(ButtonHelper::_upLongClickHandlerBackup);
				UIEntityTime::_down->SetLongClickHandler(ButtonHelper::_downLongClickHandlerBackup);
			}
			
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
				_selectionTarget = SelectionTarget::None;
				_select = select;
			}
			
			void TurnAlarm(const l_t& status) {
				_alarmOn = status;
			}
			
			void BlinkCentralPoint() {
				lcd8::PointAt(lcd8position::Second, _alarmOn ? TRUE : this->GetTimeValue().GetSeconds() & 1);
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
				
				instance.TransitTo(selector->GetHandle());
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
			
			static void OnSelectClick(const Button& sender) {
				instance.TransitTo(Stopwatch::GetInstance()->GetHandle());
			}
			
			static void OnUpClick(const Button& sender) {
				instance.TurnAlarm(!instance._alarmOn);
			}
			
			static void OnUpLongClick(const Button& sender) {
				instance.Select(SelectionTarget::AlarmTime);
			}
			
			static void OnDownClick(const Button& sender) {
				instance.TransitTo(TemperatureSensor::GetInstance()->GetHandle());
			}
			
			static void OnDownLongClick(const Button& sender) {
				instance.Select(SelectionTarget::Time);
			}
			
			void RestoreHandlers() {
				_up->SetClickHandler(OnUpClick);
				_up->SetLongClickHandler(OnUpLongClick);
				
				_down->SetClickHandler(OnDownClick);
				_down->SetLongClickHandler(OnDownLongClick);
				
				_select->SetClickHandler(OnSelectClick);
			}
			
			static void OnAlarmStopRinging(const Button& sender) {
				
				if(avrhwaudio::IsStarted()) {
					avrhwaudio::Stop();
					
					//Add restoring handlers here
					instance.RestoreLastButtons();
				}
			}
			
			public:
			
			void OnUpdate(const TimeSpan& delta) override {
				
				if(_alarmOn) {
					if(_alarmTime == this->GetTimeValue()) {
						
						if(!avrhwaudio::IsStarted()) {
							//ring code here
							BackupLastButtons();
							
							_up->SetLongClickHandler(nullptr);
							_down->SetLongClickHandler(nullptr);
							_up->SetClickHandler(Clock::OnAlarmStopRinging);
							_down->SetClickHandler(Clock::OnAlarmStopRinging);
							
							avrhwaudio::Start(ALARM_FREQ);
						}
					}
				}
				
				UIEntityTime::OnUpdate(delta);
				
				auto timeVal = this->GetTimeValue();
				auto days = timeVal.GetDays();
				
				if(days > 0) {
					this->SetTimeValue(timeVal.SubtractDays(days));
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
			}
			
			static Clock* InitAndGetInstance(
			const TimeSpan& initTime,
			const TimeSpan& alarmTime,
			const l_t& state,
			pbutton_t select,
			pbutton_t up,
			pbutton_t down) {
				
				instance = Clock(initTime, alarmTime, state, select, up, down);
				return &instance;
			}
			
			static Clock* GetInstance() {
				return &instance;
			}
		};
		
		Clock Clock::instance(TimeSpan(0), TimeSpan(0), FALSE, nullptr, nullptr, nullptr);
	}
}

#endif //__CLOCK_H__
