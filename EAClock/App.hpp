#ifndef __APP_HPP__
#define __APP_HPP__

//BaseAVR includes here
#include "BaseAVR/globaldef.h"
#include "BaseAVR/macrodef.h"
#include "BaseAVR/Audio/HAL/avrhwaudio.h"
#include "BaseAVR/Time/TimeSpan.h"
#include "BaseAVR/Time/Timer.h"
#include "BaseAVR/Time/await.h"
#include "BaseAVR/IO/lcd8.h"
#include "BaseAVR/IO/Button.h"
#include "BaseAVR/HAL/HWBase.h"

//UI includes here
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
	
	//Main EAClock class
	class App {
		
		private:
		App();
		App(const App&);
		
		public:
		
		//Initializes Application
		static void Init() {
			
			//BaseAVR components initialization
			__CLI
			lcd8::Init();
			lcd8::UseZeroAsDefaultDigit(FALSE);
			Button::Init();
			__SEI
			
			//Initialization of buttons
			pbutton_t select = Button::GetNextInstance(VLine(hwio_base::D, D0, IOMode::Input));
			pbutton_t up = Button::GetNextInstance(VLine(hwio_base::D, D1, IOMode::Input));
			pbutton_t down = Button::GetNextInstance(VLine(hwio_base::D, D2, IOMode::Input));
			
			//Initialization of UI and setting buttons
			UIManager::Init();
			UIManager::SetSelectButton(select);
			UIManager::SetUpButton(up);
			UIManager::SetDownButton(down);
			
			//Creation and getting main ui
			pui_entity ui_clock = Clock::InitAndGetInstance(
			TimeSpan(0, 12, 0, 0, 0), //Clock start time
			TimeSpan(0, 7, 0, 0, 0), //Default alarm time
			TRUE,					//Starting instantly
			select,
			up,
			down);
			
			//Setting Main UI
			UIManager::AddMainUI(ui_clock);
			
			//Creating other UIs here
			pui_entity ui_selector = TimeSelector::InitAndGetInstance(up, down);
			pui_entity ui_stopWatch = Stopwatch::InitAndGetInstance(TimeSpan::Zero, up, down);
			pui_entity ui_sensor = TemperatureSensor::InitAndGetInstance(FALSE, down, TemperatureUnits::Celsius);
			pui_entity ui_timer = UI::Timer::InitAndGetInstace(FALSE, TimeSpan(0, 0, 30, 0, 0), up, down);
			
			//Adding other UIs here
			UIManager::AddUI(ui_selector);
			UIManager::AddUI(ui_stopWatch);
			UIManager::AddUI(ui_sensor);
			UIManager::AddUI(ui_timer);
			
			//Initialization of BaseAVR Audio
			avrhwaudio::Init();
		}
		
		//Shows splash screen for specified time
		static void ShowSplashScreenFor(const TimeSpan& duration) {
			
			const u8_t splashScreen[] = "E-CL";
			
			lcd8::Write(splashScreen);
			Await::ForMs(duration.GetTotalMilliseconds());
			
			lcd8::Clear();
		}
		
		//Starts UI
		static void Start() {
			UIManager::Start();
		}
		
		//Calls subroutines (should be called as often as possible)
		static void EventLoop() {
			Time::Timer::CallSubroutines();
			Button::CallSubroutines();
		}
	};
};

#endif //__APP_HPP__
