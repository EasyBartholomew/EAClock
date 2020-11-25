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


using namespace BaseAVR;
using namespace BaseAVR::HAL;
using namespace BaseAVR::IO;
using namespace BaseAVR::Time;

#define UI_STOPWATCH 0

namespace EAClock {
	namespace UI {
		
		class UIManager {
			
			private:
			
			static const fsize_t UIS_MAX = 1;
			
			static pbutton_t select;
			static pbutton_t up;
			static pbutton_t down;
			static Timer* updater;
			static Timer* uiUpdater;
			
			static pui_entity uis[UIManager::UIS_MAX];
			static fsize_t current_ui;
			
			static void OnUpdate() {
				for(register fsize_t i = 0; i < UIS_MAX; i++) {
					pui_entity cui = uis[i];
					
					if(cui->IsTimeEntity()) {
						auto tui = (pui_entitytime)cui;
						tui->OnUpdate(updater->GetInterval());
					}
				}
			}
			
			static void OnUiUpdate() {
				auto cui = uis[current_ui];
				lcd8::Write(cui->GetConstBufferPtr());
			}
			
			static void GoToUi(const fsize_t& idx) {
				auto cui = uis[current_ui];
				cui->OnFocusLost();
				
				current_ui = idx;
				
				cui = uis[current_ui];
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
			
			static void OnSelectLongClick(const Button& sender){
				UIManager::GoToUi(0);
			}
			
			public:
			
			static void Init() {
				
				uis[UI_STOPWATCH] = Stopwatch::GetInstance(TimeSpan(0), up);
				
				select = Button::GetNextInstance(VLine(hwio_base::D, D0, IOMode::Input));
				up = Button::GetNextInstance(VLine(hwio_base::D, D1, IOMode::Input));
				down = Button::GetNextInstance(VLine(hwio_base::D, D2, IOMode::Input));
				
				select->SetHandlerPriority(Button::CallPriority::High);
				select->SetClickHandler(OnSelectClick);
				select->SetLongClickHandler(OnSelectLongClick);
				
				updater = Timer::GetNextInstance(10);
				updater->SetAutoReset(TRUE);
				updater->SubscribeHandler(OnUpdate);
				
				uiUpdater = Timer::GetNextInstance(20);
				uiUpdater->SetAutoReset(TRUE);
				uiUpdater->SubscribeHandler(OnUiUpdate);
				
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
