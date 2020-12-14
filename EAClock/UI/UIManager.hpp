#ifndef __UIMANAGER_H__
#define __UIMANAGER_H__
#include "../BaseAVR/globaldef.h"
#include "../BaseAVR/macrodef.h"
#include "../BaseAVR/Time/TimeSpan.h"
#include "../BaseAVR/Time/Timer.h"
#include "../BaseAVR/IO/lcd8.h"
#include "../BaseAVR/IO/Button.h"
#include "../BaseAVR/HAL/VLine.h"
#include "../BaseAVR/HAL/HWBase.h"

#include "UIEntity.hpp"
#include "UIEntityTime.hpp"
#include "Stopwatch.hpp"
#include "Clock.hpp"
#include "TimeSelector.hpp"
#include "Timer.hpp"

using namespace BaseAVR;
using namespace BaseAVR::HAL;
using namespace BaseAVR::IO;
using namespace BaseAVR::Time;

#define ENTITY_UPDATE_INTERVAL 25
#define UI_UPDATE_INTERVAL 50

#define UI_MAIN 0
#define UI_STOPWATCH 1
#define UI_SELECTOR 2
#define UI_TIMER 3
#define UI_SENSOR 4

namespace EAClock {
	namespace UI {
		
		class UIManager {
			
			private:
			
			static const fsize_t UIS_MAX = 6;
			
			static pbutton_t select;
			static pbutton_t up;
			static pbutton_t down;
			static BaseAVR::Time::Timer* updater;
			static BaseAVR::Time::Timer* uiUpdater;
			
			static pui_entity uis[UIManager::UIS_MAX];
			static fsize_t current_ui;
			
			static pui_entity GetCurrentUI() {
				return uis[current_ui];
			}
			
			static void BlockButtons() {
				up->SetClickHandler(nullptr);
				up->SetLongClickHandler(nullptr);
				
				down->SetClickHandler(nullptr);
				down->SetLongClickHandler(nullptr);
			}
			
			static void GoToUi(const fsize_t& idx) {
				
				auto cui = UIManager::GetCurrentUI();
				cui->LoseFocus();
				
				current_ui = idx;
				
				UIManager::BlockButtons();
				
				cui = UIManager::GetCurrentUI();
				cui->GetFocus();
			}
			
			static void ChangeShowMode() {
				
				auto cui = uis[current_ui];
				
				if(!cui->IsTimeEntity())
				{ return; }
				
				auto tui = (pui_entitytime)cui;
				auto currentMode = tui->GetShowMode();
				
				if(currentMode == ShowMode::ss_th) {
					currentMode = (ShowMode)0U;
				}
				else {
					currentMode = (ShowMode)(TOBYTE(currentMode) + 1);
				}
				
				tui->OnShowModeChanged(currentMode);
			}
			
			static void OnSelectLongClick(const Button& sender){
				UIManager::ChangeShowMode();
			}
			
			static void OnSelectClick(const Button& sender) {
				
				auto cui = UIManager::GetCurrentUI();
				GoToUi(cui->GetTransitionTarget());
			}
			
			static void OnUpdate() {
				for(register fsize_t i = 0; i < UIS_MAX; i++) {
					auto cui = uis[i];
					
					if(cui == nullptr)
					continue;
					
					if(!cui->IsStarted())
					continue;
					
					if(cui->IsTimeEntity()) {
						auto tui = (pui_entitytime)cui;
						tui->OnUpdate(updater->GetInterval());
					}
				}
			}
			
			static void OnUiUpdate() {
				auto cui = UIManager::GetCurrentUI();
				
				if(cui == nullptr)
				return;
				
				if(cui->IsTransitionTarget()) {
					
					select->SetClickHandler(OnSelectClick);
					UIManager::GoToUi(cui->GetTransitionTarget());
					cui->TransitTo(0);
					return;
				}
				
				lcd8::Write(cui->GetConstBufferPtr());
				cui->OnUiUpdate();
			}
			
			public:
			
			static void Init() {
				
				select = Button::GetNextInstance(VLine(hwio_base::D, D0, IOMode::Input));
				up = Button::GetNextInstance(VLine(hwio_base::D, D1, IOMode::Input));
				down = Button::GetNextInstance(VLine(hwio_base::D, D2, IOMode::Input));
				
				for(register fsize_t i = 0; i < UIS_MAX; i++) {
					uis[i] = nullptr;
				}
				
				uis[UI_MAIN] = Clock::InitAndGetInstance(TimeSpan(0,12,0,0,0), TimeSpan(0,12,0,0,0), TRUE, select, up, down);
				uis[UI_STOPWATCH] = Stopwatch::InitAndGetInstance(TimeSpan::Zero, up, down);
				uis[UI_SELECTOR] = TimeSelector::InitAndGetInstance(up, down);
				uis[UI_SENSOR] = TemperatureSensor::InitAndGetInstance(FALSE, down, TemperatureUnits::Celsius);
				uis[UI_TIMER] = UI::Timer::InitAndGetInstace(FALSE, TimeSpan::FromMinutes(10), up, down);
				
				for(register fsize_t i = 0; i < UIS_MAX; i++) {
					if(uis[i] != nullptr)
					uis[i]->SetHandle(i);
				}
				
				select->SetHandlerPriority(CallPriority::High);
				select->SetClickHandler(OnSelectClick);
				select->SetLongClickHandler(OnSelectLongClick);
				
				updater = BaseAVR::Time::Timer::GetNextInstance(ENTITY_UPDATE_INTERVAL);
				updater->SetAutoReset(TRUE);
				updater->SubscribeHandler(OnUpdate);
				updater->SetHandlerPriority(CallPriority::High);
				
				uiUpdater = BaseAVR::Time::Timer::GetNextInstance(UI_UPDATE_INTERVAL);
				uiUpdater->SetAutoReset(TRUE);
				uiUpdater->SubscribeHandler(OnUiUpdate);
				uiUpdater->SetHandlerPriority(CallPriority::Normal);
				
				avrhwaudio::Init();
			}
			
			static void Start() {
				GoToUi(UI_MAIN);
				updater->Start();
				uiUpdater->Start();
			}
		};
		
		
		pui_entity UIManager::uis[UIManager::UIS_MAX];
		fsize_t UIManager::current_ui = 0;
		
		pbutton_t UIManager::select = nullptr;
		pbutton_t UIManager::up = nullptr;
		pbutton_t UIManager::down = nullptr;
		
		BaseAVR::Time::Timer* UIManager::updater = nullptr;
		BaseAVR::Time::Timer* UIManager::uiUpdater = nullptr;
	}
}

#endif //__UIMANAGER_H__
