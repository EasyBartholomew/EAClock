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


using namespace BaseAVR;
using namespace BaseAVR::HAL;
using namespace BaseAVR::IO;
using namespace BaseAVR::Time;

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
			static const handle_t UI_MAIN_HANDLE = 0;
			
			static const tu_t TIME_ENTITY_UPDATE_INTERVAL = 25;
			static const tu_t UI_ENTITY_UPDATE_INTERVAL = 50;
			
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
			
			static void GoToUi(const handle_t& idx) {
				
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
				for(register handle_t i = 0; i < UIS_MAX; i++) {
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
			
			static void AddUI(pui_entity ui) {
				
				//Zero reserved for main ui
				for(register handle_t h = 1; h < UIManager::UIS_MAX; h++) {
					
					if(uis[h] == nullptr) {
						uis[h] = ui;
						ui->SetHandle(h);
						
						return;
					}
				}
			}
			
			static void RemoveUI(const handle_t& handle) {
				
				if(handle >= UIManager::UIS_MAX) {
					return;
				}
				
				uis[handle] = nullptr;
			}
			
			static void AddMainUI(pui_entity mainUI) {
				uis[UIManager::UI_MAIN_HANDLE] = mainUI;
				mainUI->SetHandle(UIManager::UI_MAIN_HANDLE);
			}
			
			//Make private
			static void SetButton(pbutton_t* target, pbutton_t btn) {
				*target = btn;
			}
			
			static void SetSelectButton(pbutton_t btn) {
				UIManager::SetButton(&select, btn);
				
				select->SetHandlerPriority(CallPriority::High);
				select->SetClickHandler(OnSelectClick);
				select->SetLongClickHandler(OnSelectLongClick);
			}
			
			static void SetUpButton(pbutton_t btn) {
				UIManager::SetButton(&up, btn);
			}
			
			static void SetDownButton(pbutton_t btn) {
				UIManager::SetButton(&down, btn);
			}
			
			static void Init() {
				
				for(register handle_t i = 0; i < UIS_MAX; i++) {
					uis[i] = nullptr;
				}
				
				updater = Timer::GetNextInstance((tu_t)UIManager::TIME_ENTITY_UPDATE_INTERVAL);
				updater->SetAutoReset(TRUE);
				updater->SubscribeHandler(OnUpdate);
				updater->SetHandlerPriority(CallPriority::High);
				
				uiUpdater = Timer::GetNextInstance((tu_t)UIManager::UI_ENTITY_UPDATE_INTERVAL);
				uiUpdater->SetAutoReset(TRUE);
				uiUpdater->SubscribeHandler(OnUiUpdate);
				uiUpdater->SetHandlerPriority(CallPriority::Normal);
			}
			
			static void Start() {
				GoToUi((handle_t)UIManager::UI_MAIN_HANDLE);
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
