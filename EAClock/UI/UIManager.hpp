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

namespace EAClock {
	namespace UI {
		
		class UIManager {
			
			private:
			
			static const fsize_t UIS_MAX = 4;
			
			static pbutton_t select;
			static pbutton_t up;
			static pbutton_t down;
			static Timer* updater;
			static Timer* uiUpdater;
			
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
				cui->OnFocusLost();
				
				current_ui = idx;
				
				UIManager::BlockButtons();
				
				cui = UIManager::GetCurrentUI();
				cui->OnFocus();
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
			
			static void OnSelectClick(const Button& sender){
				UIManager::ChangeShowMode();
			}
			
			static void OnSelectLongClick(const Button& sender) {
				
				auto cui = UIManager::GetCurrentUI();
				GoToUi(cui->GetTransitionTarget());
			}
			
			static void OnUpdate() {
				for(register fsize_t i = 0; i < UIS_MAX; i++) {
					pui_entity cui = uis[i];
					
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
					
					select->SetLongClickHandler(OnSelectLongClick);
					UIManager::GoToUi(cui->GetTransitionTarget());
					return;
				}
				
				lcd8::Write(cui->GetConstBufferPtr());
				cui->OnUiUpdate();
			}
			
			public:
			
			static void Init() {
				
				for(register fsize_t i = 0; i < UIS_MAX; i++) {
					uis[i] = nullptr;
				}
				
				uis[UI_MAIN] = Clock::GetInstance(TimeSpan(0,12,15,16,0), TimeSpan(0,12,20,0,0), TRUE, select, up, down);
				uis[UI_STOPWATCH] = Stopwatch::GetInstance(TimeSpan::Zero, up);
				uis[UI_SELECTOR] = TimeSelector::InitAndGetInstance(up, down);
				
				for(register fsize_t i = 0; i < UIS_MAX; i++) {
					if(uis[i] != nullptr)
					uis[i]->SetHandle(i);
				}
				
				select = Button::GetNextInstance(VLine(hwio_base::D, D0, IOMode::Input));
				up = Button::GetNextInstance(VLine(hwio_base::D, D1, IOMode::Input));
				down = Button::GetNextInstance(VLine(hwio_base::D, D2, IOMode::Input));
				
				select->SetHandlerPriority(Button::CallPriority::High);
				select->SetClickHandler(OnSelectClick);
				select->SetLongClickHandler(OnSelectLongClick);
				
				updater = Timer::GetNextInstance(ENTITY_UPDATE_INTERVAL);
				updater->SetAutoReset(TRUE);
				updater->SubscribeHandler(OnUpdate);
				
				uiUpdater = Timer::GetNextInstance(UI_UPDATE_INTERVAL);
				uiUpdater->SetAutoReset(TRUE);
				uiUpdater->SubscribeHandler(OnUiUpdate);
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
		
		Timer* UIManager::updater = nullptr;
		Timer* UIManager::uiUpdater = nullptr;
	}
}

#endif //__UIMANAGER_H__
