#ifndef __TIMER_HPP__
#define __TIMER_HPP__
#include "UIEntityTime.hpp"
#include "TimeSelector.hpp"

#include "../BaseAVR/Audio/HAL/avrhwaudio.h"

#define TIMER_RINGER_FREQ ((freq_t)1500)


using namespace BaseAVR::Audio::HAL;

namespace EAClock {
	namespace UI {
		
		class Timer : public UIEntityTime {
			
			private:
			
			static Timer instance;
			TimeSpan _userTimeLeft;
			l_t _isSelectionPerformed;
			
			_ButtonEventCallBack _upClickHandlerBackup;
			_ButtonEventCallBack _downClickHandlerBackup;
			_ButtonEventCallBack _upLongClickHandlerBackup;
			_ButtonEventCallBack _downLongClickHandlerBackup;
			
			
			Timer(
			const l_t& state,
			const TimeSpan& pivotValue,
			pbutton_t up,
			pbutton_t down) : UIEntityTime(state, up, down) {
				_userTimeLeft = pivotValue;
				_isSelectionPerformed = FALSE;
				this->BackupLastButtons();
			}
			
			void Reset() {
				UIEntityTime::SetTimeValue(_userTimeLeft);
			}
			
			void BackupLastButtons() {
				_upClickHandlerBackup = UIEntityTime::_up->GetClickHandler();
				_downClickHandlerBackup = UIEntityTime::_down->GetClickHandler();
				
				_upLongClickHandlerBackup = UIEntityTime::_up->GetLongClickHandler();
				_downLongClickHandlerBackup = UIEntityTime::_down->GetLongClickHandler();
			}
			
			void RestoreLastButtons() {
				UIEntityTime::_up->SetClickHandler(_upClickHandlerBackup);
				UIEntityTime::_down->SetClickHandler(_downClickHandlerBackup);
				
				UIEntityTime::_up->SetLongClickHandler(_upLongClickHandlerBackup);
				UIEntityTime::_down->SetLongClickHandler(_downLongClickHandlerBackup);
			}
			
			static void OnUpClick(const Button& sender) {
				instance.TransitTo(instance.GetReturnTarget());
			}
			
			static void OnDownClick(const Button& sender) {
				
				if(instance.IsStarted()) {
					instance.Stop();
				}
				else {
					instance.Start();
				}
			}
			
			static void OnDownLongClick(const Button& sender) {
				instance.Stop();
				TimeSelector::GetInstance()->
				Select(instance._userTimeLeft, SelectionPair::Low, ShowMode::hh_mm, instance.GetHandle());
				instance._isSelectionPerformed = TRUE;
				instance.TransitTo(TimeSelector::GetInstance()->GetHandle());
			}
			
			static void OnUpLongClick(const Button& sender) {
				instance.Reset();
			}
			
			void Ring() {
				
				if(!avrhwaudio::IsStarted()) {
					instance.BackupLastButtons();
					UIEntityTime::_up->SetClickHandler(OnStopRinging);
					UIEntityTime::_down->SetClickHandler(OnStopRinging);
					avrhwaudio::Start(TIMER_RINGER_FREQ);
				}
			}
			
			void StopRinging() {
				
				if(avrhwaudio::IsStarted()) {
					instance.RestoreLastButtons();
					avrhwaudio::Stop();
				}
				
				UIEntityTime::SetTimeValue(_userTimeLeft);
			}
			
			static void OnStopRinging(const Button& sender) {
				instance.StopRinging();
			}
			
			void OnSelectionEnd() {
				
				_userTimeLeft = TimeSelector::GetInstance()->GetTimeValue();
				UIEntityTime::SetTimeValue(_userTimeLeft);
				_isSelectionPerformed = FALSE;
			}
			
			public:
			
			void OnUpdate(const TimeSpan& delta) override {
				
				auto remain = UIEntityTime::GetTimeValue() - delta;
				
				if(remain <= TimeSpan::Zero) {
					this->Stop();
					this->Ring();
				}
				
				if(remain.GetTotalMilliseconds() >= 0) {
					UIEntityTime::SetTimeValue(remain);
				}
			}
			
			void OnUiUpdate() override {
				
				auto span = this->GetTimeValue();
				
				this->SetShowMode(ShowMode::hh_mm);
				
				if(span.GetHours() == 0) {
					this->SetShowMode(ShowMode::mm_ss);
				}
				
				if(span.GetMinutes() == 0) {
					this->SetShowMode(ShowMode::ss_th);
				}
			}
			
			void OnFocus() override {
				
				if(!this->IsStarted()) {
					instance.SetTimeValue(instance._userTimeLeft);
				}
				
				if(_isSelectionPerformed) {
					this->OnSelectionEnd();
				}
				
				lcd8::PointAt(lcd8position::Second, TRUE);
				UIEntityTime::_down->SetClickHandler(OnDownClick);
				UIEntityTime::_down->SetLongClickHandler(OnDownLongClick);
				UIEntityTime::_up->SetClickHandler(OnUpClick);
				UIEntityTime::_up->SetLongClickHandler(OnUpLongClick);
			}
			
			void OnFocusLost() override {
				
				lcd8::PointAt(lcd8position::Second, FALSE);
			}
			
			
			static Timer* GetInstace() {
				return &instance;
			}
			
			static Timer* InitAndGetInstace(
			const l_t& state,
			const TimeSpan& pivotValue,
			pbutton_t up,
			pbutton_t down) {
				instance = Timer(state, pivotValue, up, down);
				return Timer::GetInstace();
			}
		};
		
		Timer Timer::instance(FALSE, TimeSpan::Zero, nullptr, nullptr);
		
	}
}
#endif //__TIMER_HPP__
