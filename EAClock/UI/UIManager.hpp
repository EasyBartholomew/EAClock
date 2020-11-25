#ifndef __UIMANAGER_H__
#define __UIMANAGER_H__
#include "../BaseAVR/globaldef.h"
#include "../BaseAVR/macrodef.h"
#include "../BaseAVR/Time/TimeSpan.h"
#include "../BaseAVR/Time/Timer.h"
#include "../BaseAVR/IO/lcd8.h"
#include "../BaseAVR/IO/Button.h"
#include "UIEntity.hpp"
#include "UIEntityTime.hpp"


using namespace BaseAVR;
using namespace BaseAVR::IO;
using namespace BaseAVR::Time;


namespace EAClock {
	namespace UI {
		
		enum class ShowMode : u8_t {
			hh_mm,
			mm_ss,
			ss_th
		};
		
		enum class SPosition : u8_t {
			First,
			Second
		};
		
		class UIManager {
			
			private:
			
			static const fsize_t UIS_MAX = 4;
			
			static Button* select;
			static Button* up;
			static Button* down;
			static Timer* updater;
			static ShowMode currentMode;
			
			static pui_entity uis[UIManager::UIS_MAX];
			static fsize_t current_ui;
			
			static void WriteFormatedSpan(const TimeSpan& span) {
				switch(currentMode) {
					case ShowMode::hh_mm: {
						lcd8::Write(span.GetHours() / 10, lcd8position::First);
						lcd8::Write(span.GetHours() % 10, lcd8position::Second);
						lcd8::Write(span.GetMinutes() / 10, lcd8position::Third);
						lcd8::Write(span.GetMinutes() % 10, lcd8position::Fourth);
					}break;
					
					case ShowMode::mm_ss: {
						lcd8::Write(span.GetMinutes() / 10, lcd8position::First);
						lcd8::Write(span.GetMinutes() % 10, lcd8position::Second);
						lcd8::Write(span.GetSeconds() / 10, lcd8position::Third);
						lcd8::Write(span.GetSeconds() % 10, lcd8position::Fourth);
						
					}break;
					
					case ShowMode::ss_th: {
						lcd8::Write(span.GetSeconds() / 10, lcd8position::First);
						lcd8::Write(span.GetSeconds() % 10, lcd8position::Second);
						lcd8::Write(span.GetMilliseconds() / 100, lcd8position::Third);
						lcd8::Write(span.GetMilliseconds() / 10, lcd8position::Fourth);
					}break;
				}
			}
			
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
				
				if(cui->IsTimeEntity()) {
					
					auto tui = (pui_entitytime)cui;
					
					UIManager::WriteFormatedSpan(tui->GetTimeValue());
				}
				
			}
			
			static void GoNextUI() {
				
				if(current_ui == (UIManager::UIS_MAX - 1)) {
					current_ui = 0;
					return;
				}
				
				current_ui++;
			}
			
			static void ChangeShowMode() {
				
				if(currentMode == ShowMode::ss_th) {
					currentMode = (ShowMode)0U;
					return;
				}
				
				currentMode = (ShowMode)(TOBYTE(currentMode) + 1);
			}
			
			public:
			
			static void Init() {
				
			}
			
		};
		
		pui_entity UIManager::uis[UIManager::UIS_MAX];
		fsize_t UIManager::current_ui = 0;
		
		Button* UIManager::select = nullptr;
		Button* UIManager::up = nullptr;
		Button* UIManager::down = nullptr;
	}
}

#endif //__UIMANAGER_H__
