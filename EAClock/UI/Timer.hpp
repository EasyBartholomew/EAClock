#ifndef __TIMER_HPP__
#define __TIMER_HPP__
#include "UIEntityTime.hpp"
#include "Stopwatch.hpp"
#include "TemperatureSensor.hpp"

#include "../BaseAVR/Audio/HAL/avrhwaudio.h"

#define TIMER_RINGER_FREQ ((freq_t)1500)


using namespace BaseAVR::Audio::HAL;

namespace EAClock {
	namespace UI {
		
		class Timer : public UIEntityTime {
			
			private:
			
			static Timer instance;
			TimeSpan _userTimeLeft;
			
			_ButtonEventCallBack _upClickHandlerBackup;
			_ButtonEventCallBack _downClickHandlerBackup;
			_ButtonEventCallBack _upLongClickHandlerBackup;
			_ButtonEventCallBack _downLongClickHandlerBackup;
			
			Timer(
			const l_t& state,
			const TimeSpan& pivotValue,
			pbutton_t up,
			pbutton_t down) : UIEntityTime(state, down, down) {
				_userTimeLeft = pivotValue;
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
				instance.TransitTo(Stopwatch::GetInstance()->GetHandle());
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
				
			}
			
			static void OnUpLongClick(const Button& sender) {
				
			}
			
			void Ring() {
				
				instance.BackupLastButtons();
				
				if(!avrhwaudio::IsStarted()) {
					avrhwaudio::Start(TIMER_RINGER_FREQ);
				}
			}
			
			void StopRinging() {
				
				if(avrhwaudio::IsStarted()) {
					instance.RestoreLastButtons();
				}
				
				avrhwaudio::Stop();
			}
			
			public:
			
			void OnUpdate(const TimeSpan& delta) override {
				
				auto remain = UIEntityTime::GetTimeValue() - delta;
				
				if(remain == TimeSpan::Zero) {
					this->Stop();
					this->Ring();
				}
				
				if(remain.GetTotalMilliseconds() >= 0) {
					UIEntityTime::SetTimeValue(remain);
				}
			}
			
			void OnFocus() override {
				
			}
			
			void OnFocusLost() override {
				
			}
			
			
			static Timer* GetInstace() {
				return &instance;
			}
			
			static Timer* InitAndGetInstace(const l_t& state, const TimeSpan& pivotValue, pbutton_t down) {
				instance = Timer(state, pivotValue, down);
				return Timer::GetInstace();
			}
		};
		
		Timer Timer::instance(FALSE, TimeSpan::Zero, nullptr);
		
	}
}
#endif //__TIMER_HPP__
