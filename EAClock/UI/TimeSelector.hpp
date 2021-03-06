#ifndef __TIMESELECTOR_H__
#define __TIMESELECTOR_H__
#include "../BaseAVR/globaldef.h"
#include "UIEntityTime.hpp"

#define DEFAULT_BLINK_INTEVAL 750
#define DEFAULT_CLICK_CHANGE 1
#define DEFAUL_LONGCLICK_CHANGE 10


namespace EAClock {
	namespace UI {
		
		enum class SelectionPair : u8_t {
			None,
			High,
			Low
		};
		
		class TimeSelector : public UIEntityTime {
			
			private:
			
			static TimeSelector instance;
			
			SelectionPair _pair;
			l_t _showState;
			
			TimeSpan _blinkInterval;
			TimeSpan _timeToUpdate;
			
			handle_t _backHandle;
			
			u8_t * _GetBuffer() const override {
				
				const auto blinkSymbol = '_';
				auto base = UIEntityTime::_GetBuffer();
				
				if(_showState || !this->IsStarted()) {
					return base;
				}
				
				switch (_pair)
				{
					case SelectionPair::Low: {
						base[2] = base[3] = blinkSymbol;
						return base;
					}
					
					case SelectionPair::High: {
						base[0] = base[1] = blinkSymbol;
						return base;
					}
					
					default:
					return base;
				}
			}
			
			u8_t _ChangeCyclically(const s8_t& pivot, const s8_t& step, const s8_t& maxValue) {
				
				s8_t retVal = pivot + step;
				retVal += (retVal < 0 ? maxValue : (retVal >= maxValue ? -maxValue : 0));
				
				return retVal;
			}
			
			void ChangeOnValue(const tu_t& val) {
				
				_showState = TRUE;
				_timeToUpdate = TimeSpan::Zero;
				auto timeValue = this->GetTimeValue();
				
				switch (this->GetShowMode()) {
					case ShowMode::hh_mm: {
						switch (_pair) {
							
							case SelectionPair::Low: {
								
								auto pickValue = _ChangeCyclically(timeValue.GetMinutes(), (s8_t)val, 60);
								timeValue = TimeSpan(0,
								timeValue.GetHours(),
								pickValue,
								timeValue.GetSeconds(),
								timeValue.GetMilliseconds());
								
							}break;
							
							case SelectionPair::High: {
								
								auto pickValue = _ChangeCyclically(timeValue.GetHours(), (s8_t)val, 24);
								timeValue = TimeSpan(0,
								pickValue,
								timeValue.GetMinutes(),
								timeValue.GetSeconds(),
								timeValue.GetMilliseconds());
								
							}break;
							
							default:
							break;
						}
					}break;
					
					case ShowMode::mm_ss: {
						switch (_pair) {
							
							case SelectionPair::Low: {
								auto pickValue = _ChangeCyclically(timeValue.GetSeconds(), (s8_t)val, 60);
								timeValue = TimeSpan(0,
								timeValue.GetHours(),
								timeValue.GetMinutes(),
								pickValue,
								timeValue.GetMilliseconds());
								
							}break;
							
							case SelectionPair::High: {
								
								auto pickValue = _ChangeCyclically(timeValue.GetMinutes(), (s8_t)val, 60);
								timeValue = TimeSpan(0,
								timeValue.GetHours(),
								pickValue,
								timeValue.GetSeconds(),
								timeValue.GetMilliseconds());
								
							}break;
							
							default:
							break;
						}break;
					}
					
					break;
					default:
					break;
				}
				
				auto days = timeValue.GetDays();
				
				if(days > 0)
				timeValue.SubtractDays(days);
				
				this->SetTimeValue(timeValue);
			}
			
			void ChangeByButtonClick(const Button& sender, const tu_t& val) {
				s8_t sign = (&sender == _up) ? 1 : -1;
				this->ChangeOnValue(sign * val);
			}
			
			static void OnChangeButtonClick(const Button& sender) {
				instance.ChangeByButtonClick(sender, DEFAULT_CLICK_CHANGE);
			}
			
			static void OnChangeButtonLongClick(const Button& sender) {
				instance.ChangeByButtonClick(sender, DEFAUL_LONGCLICK_CHANGE);
			}
			
			void _reset() {
				_showState = FALSE;
				_backHandle = 0;
				_timeToUpdate = TimeSpan::Zero;
				_blinkInterval = TimeSpan(DEFAULT_BLINK_INTEVAL);
			}
			
			TimeSelector(
			pbutton_t up,
			pbutton_t down)
			: UIEntityTime(FALSE, up, down) {
				
				_reset();
				_pair = SelectionPair::Low;
			}
			
			public:
			
			TimeSpan GetBlinkInterval() const {
				return _blinkInterval;
			}
			
			void SetBlinkInterval(const TimeSpan& blinkInterval) {
				_blinkInterval = blinkInterval;
			}
			
			void Select(
			const TimeSpan& base,
			const SelectionPair& pair,
			const ShowMode& showMode,
			const handle_t& self) {
				_backHandle = self;
				_pair = pair;
				UIEntityTime::SetTimeValue(base);
				this->SetShowMode(showMode);
			}
			
			void OnShowModeChanged(const ShowMode& newMode) override {
				
				if(_pair == SelectionPair::Low){
					_pair = SelectionPair::High;
				}
				else if(_pair == SelectionPair::High) {
					_pair = SelectionPair::Low;
				}
			}
			
			void OnFocus() override {
				
				lcd8::PointAt(lcd8position::Second, TRUE);
				
				_up->SetClickHandler(OnChangeButtonClick);
				_down->SetClickHandler(OnChangeButtonClick);
				
				_up->SetLongClickHandler(OnChangeButtonLongClick);
				_down->SetLongClickHandler(OnChangeButtonLongClick);
				
				this->Start();
			}
			
			void OnFocusLost() override {
				
				lcd8::PointAt(lcd8position::Second, FALSE);
				
				this->Stop();
				
				_reset();
			}
			
			void OnUpdate(const TimeSpan& delta) override {
				
				_timeToUpdate = _timeToUpdate + delta;
				
				if(_timeToUpdate >= _blinkInterval) {
					_timeToUpdate = TimeSpan::Zero;
					
					_showState = !_showState;
				}
			}
			
			handle_t GetTransitionTarget() const override {
				return _backHandle;
			}
			
			static TimeSelector* GetInstance() {
				return &instance;
			}
			
			static TimeSelector* InitAndGetInstance(pbutton_t up, pbutton_t down) {
				instance = TimeSelector(up, down);
				return TimeSelector::GetInstance();
			}
		};
		
		TimeSelector TimeSelector::instance(nullptr, nullptr);
		
		typedef TimeSelector* pui_timeselector;
	}
}

#endif //__TIMESELECTOR_H__
