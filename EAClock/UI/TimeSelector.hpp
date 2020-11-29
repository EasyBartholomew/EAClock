#ifndef __TIMESELECTOR_H__
#define __TIMESELECTOR_H__
#include "../BaseAVR/globaldef.h"
#include "UIEntityTime.hpp"

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
			
			Timer* _updater;
			SelectionPair _pair;
			l_t _showState;
			
			
			u8_t * _GetBuffer() const {
				if(_showState || !this->IsStarted()) {
					return _buffer;
				}
				
				switch (_pair)
				{
					case SelectionPair::Low: {
						_buffer[2] = _buffer[3] = '_';
						return _buffer;
					}
					
					case SelectionPair::High: {
						_buffer[0] = _buffer[1] = '_';
						return _buffer;
					}
					
					default:
					return _buffer;
				}
			}
			
			void ChangeOnValue(const l_t& sign, const tu_t& val) {
				
				switch (this->GetShowMode()) {
					case ShowMode::hh_mm: {
						switch (_pair) {
							
							case SelectionPair::Low: {
								_timeValue.AddMinutes(sign * val);
							}break;
							
							case SelectionPair::High: {
								_timeValue.AddHours(sign * val);
							}break;
							
							default:
							break;
						}
					}break;
					
					case ShowMode::mm_ss: {
						switch (_pair) {
							
							case SelectionPair::Low: {
								_timeValue.AddSeconds(sign * val);
							}break;
							
							case SelectionPair::High: {
								_timeValue.AddMinutes(sign * val);
							}break;
							
							default:
							break;
						}break;
					}
					
					break;
					default:
					break;
				}
				
				auto days = _timeValue.GetDays();
				
				if(days > 0)
				_timeValue.SubtractDays(days);
				
			}
			
			void ChangeByButtonClick(const Button& sender, const tu_t& val) {
				
				l_t sign = (&sender == _up) ? 1 : -1;
				this->ChangeOnValue(sign, val);
			}
			
			static void OnChangeButtonClick(const Button& sender) {
				instance.ChangeByButtonClick(sender, 1);
			}
			
			static void OnChangeButtonLongClick(const Button& sender) {
				instance.ChangeByButtonClick(sender, 10);
			}
			
			public:
			
			TimeSelector(
			const TimeSpan& pivot,
			pbutton_t up,
			pbutton_t down)
			: UIEntityTime(FALSE, up, down) {
				UIEntityTime::SetTimeValue(pivot);
			}
			
			u8_t const * GetConstBufferPtr() const override {
				return _GetBuffer();
			}
			
			u8_t * GetBufferPtr() override {
				return _GetBuffer();
			}
			
			void Select() {
				UIEntityTime::Start();
			}
			
			void OnShowModeChanged(const ShowMode& newMode) {
				
				auto mode = newMode;
				
				if(mode == ShowMode::ss_th) {
					mode = (ShowMode)0;
				}
				
				UIEntityTime::OnShowModeChanged(mode);
			}
		};
	}
}

#endif //__TIMESELECTOR_H__
