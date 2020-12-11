#ifndef __APP_H__
#define __APP_H__

#include "BaseAVR/globaldef.h"
#include "BaseAVR/macrodef.h"
#include "BaseAVR/Audio/HAL/avrhwaudio.h"
#include "BaseAVR/Time/TimeSpan.h"
#include "BaseAVR/Time/Timer.h"
#include "BaseAVR/Time/await.h"
#include "BaseAVR/IO/lcd8.h"
#include "BaseAVR/IO/Button.h"
#include "BaseAVR/HAL/HWBase.h"

#include "UI/UIManager.hpp"
#include "UI/Clock.hpp"

#include "UI/Stopwatch.hpp"
#include "UI/Timer.hpp"
#include "UI/TimeSelector.hpp"
#include "UI/TemperatureSensor.hpp"


using namespace BaseAVR;
using namespace BaseAVR::IO;
using namespace BaseAVR::HAL;
using namespace BaseAVR::Time;
using namespace EAClock::UI;


namespace EAClock {
	
	class App {
		
		private:
		App();
		App(const App&);
		
		public:
		
		static void Init() {
			
			__CLI
			lcd8::Init();
			lcd8::UseZeroAsDefaultDigit(FALSE);
			Button::Init();
			__SEI
			
			
			pbutton_t select = Button::GetNextInstance(VLine(hwio_base::D, D0, IOMode::Input));
			pbutton_t up = Button::GetNextInstance(VLine(hwio_base::D, D1, IOMode::Input));
			pbutton_t down = Button::GetNextInstance(VLine(hwio_base::D, D2, IOMode::Input));
			
			
			UIManager::Init();
			UIManager::SetSelectButton(select);
			UIManager::SetUpButton(up);
			UIManager::SetDownButton(down);
			
			
			pui_entity ui_clock = Clock::InitAndGetInstance(
			TimeSpan(0, 12, 0, 0, 0),
			TimeSpan(0, 7, 0, 0, 0),
			TRUE,
			select,
			up,
			down);
			
			UIManager::AddMainUI(ui_clock);
			
			
			pui_entity ui_selector = TimeSelector::InitAndGetInstance(up, down);
			pui_entity ui_stopWatch = Stopwatch::InitAndGetInstance(TimeSpan::Zero, up, down);
			pui_entity ui_sensor = TemperatureSensor::InitAndGetInstance(FALSE, down, TemperatureUnits::Celsius);
			pui_entity ui_timer = UI::Timer::InitAndGetInstace(FALSE, TimeSpan(0, 0, 30, 0, 0), up, down);
			
			UIManager::AddUI(ui_selector);
			UIManager::AddUI(ui_stopWatch);
			UIManager::AddUI(ui_sensor);
			UIManager::AddUI(ui_timer);
			
			avrhwaudio::Init();
		}
		
		static void ShowSplashScreenFor(const TimeSpan& duration) {
			
			const u8_t splashScreen[] = "E-CL";
			
			lcd8::Write(splashScreen);
			Await::ForMs(duration.GetTotalMilliseconds());
			
			lcd8::Clear();
		}
		
		static void Start() {
			UIManager::Start();
		}
		
		static void EventLoop() {
			Time::Timer::CallSubroutines();
			Button::CallSubroutines();
		}
		
	};
};

#endif //__APP_H__
