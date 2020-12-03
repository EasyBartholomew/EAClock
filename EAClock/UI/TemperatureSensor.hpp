#ifndef __TEMPERATURESENSOR_HPP__
#define __TEMPERATURESENSOR_HPP__
#include "UIEntity.hpp"
#include "../BaseAVR/IO/Sensors/LM35/LM35.h"
#include <math.h>

#define DEFAULT_DATAUPDATE_INTERVAL 3000U

using namespace BaseAVR::IO::Sensors;

namespace EAClock {
	namespace UI {
		
		enum class TemperatureUnits : u8_t {
			Celsius,
			Fahrenheit
		};
		
		class TemperatureSensor : public UIEntity {
			
			private:
			
			static TemperatureSensor instance;
			
			u16_t _currentValue;
			
			TemperatureUnits _currentUnits;
			
			Timer* _dataUpdater;
			
			
			TemperatureSensor(const l_t& state, pbutton_t down, const TemperatureUnits& units) : UIEntity(state, nullptr, down) {
				_currentUnits = units;
				_currentValue = 0;
				
				_dataUpdater = Timer::GetNextInstance(DEFAULT_DATAUPDATE_INTERVAL);
				_dataUpdater->SetAutoReset(TRUE);
				_dataUpdater->SubscribeHandler(TemperatureSensor::OnDataUpdate);
				_dataUpdater->SetHandlerPriority(CallPriority::Normal);
				
				LM35::Init();
			}
			
			void UpdateData() {
				_currentValue = LM35::MeasureAndGetCValue();
			}
			
			static void OnDataUpdate() {
				instance.UpdateData();
			}
			
			void SetPoint() {
				lcd8::PointAt(lcd8position::Second, _currentUnits == TemperatureUnits::Celsius);
			}
			
			void SwapCurrentUnits() {
				
				if (_currentUnits == TemperatureUnits::Celsius) {
					_currentUnits = TemperatureUnits::Fahrenheit;
				}
				else if(_currentUnits == TemperatureUnits::Fahrenheit) {
					_currentUnits = TemperatureUnits::Celsius;
				}
				
				SetPoint();
			}
			
			static u16_t ConvertToFahrenheit(u16_t celsius) {
				return (u16_t)round(celsius/10.0 * 1.8 + 32);
			}
			
			u8_t* _GetBuffer() const override {
				auto buff = UIEntity::_GetBuffer();
				auto value = _currentValue;
				
				if (value > 999) {
					value = 999;
				}
				
				if(_currentUnits == TemperatureUnits::Celsius) {
					buff[3] = 'C';
					
					value &= 0x0FFF;
				}
				else if(_currentUnits == TemperatureUnits::Fahrenheit) {
					buff[3] = 'F';
					
					value = TemperatureSensor::ConvertToFahrenheit(value);
				}

				buff[2] = value % 10;
				value /= 10;
				buff[1] = value % 10;
				value /= 10;
				buff[0] = value;
				
				return UIEntity::_GetBuffer();
			}
			
			static void OnDownClick(const Button& sender) {
				instance.SwapCurrentUnits();
			}
			
			public:
			
			void OnFocus() override {
				_dataUpdater->Start();
				_down->SetClickHandler(TemperatureSensor::OnDownClick);
				SetPoint();
				UpdateData();
			}
			
			void OnFocusLost() override {
				_dataUpdater->Stop();
				lcd8::PointAt(lcd8position::Second, FALSE);
			}
			
			static TemperatureSensor* GetInstance() {
				return &instance;
			}
			
			static TemperatureSensor* InitAndGetInstance(const l_t& state, pbutton_t down, const TemperatureUnits& units) {
				
				instance = TemperatureSensor(state, down, units);
				
				return TemperatureSensor::GetInstance();
			}
			
		};
		
		TemperatureSensor TemperatureSensor::instance(FALSE, nullptr, TemperatureUnits::Celsius);
		
	}
}
#endif //__TEMPERATURESENSOR_HPP__
