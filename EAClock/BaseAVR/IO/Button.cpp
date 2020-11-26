#include "Button.h"
#include "ButtonConfig.h"
#include "../macrodef.h"

using namespace BaseAVR::HAL;
using namespace BaseAVR::Time;


namespace BaseAVR {
	namespace IO {
		
		//static
		Timer* Button::initiator = nullptr;
		Button Button::instances[Button::InstancesMax];
		fsize_t Button::current_instance = 0;
		l_t Button::is_inited = FALSE;
		
		l_t Button::IsRefered() const {
			return _vLine.IsTargeted();
		}
		
		Button::Button() {
			_currentState = 0;
			_pushed4 = 0;
			_status = 0;
			_eventActed = FALSE;
			_vLine = VLine();
			_click = nullptr;
			_longClick = nullptr;
			_priority = CallPriority::Normal;
		}
		
		void Button::Refresh() {
			
			for(register fsize_t i = 0; i < Button::InstancesMax; i++) {
				
				auto currentInst = Button::GetInstance(i);
				auto currentState = currentInst->_currentState;
				
				if(!currentInst->IsRefered())
				continue;
				
				if(currentInst->IsPushed()) {
					currentInst->_pushed4++;
				}
				else {
					currentInst->_pushed4 = 0;
					_SETL(currentState, TOBYTE(ButtonHWState::Leaved));
				}
				
				if(currentInst->_pushed4 >= WAIT_BEFORE_MARK_PUSHED) {
					_SETH(currentState, TOBYTE(ButtonHWState::Pushed));
				}
				
				if(_ISH(currentState, TOBYTE(ButtonHWState::Pushed))) {
					
					if(currentInst->_pushed4) {
						currentInst->_status++;
					}
					else {
						_SETL(currentState, TOBYTE(ButtonHWState::Pushed));
						_SETH(currentState, TOBYTE(ButtonHWState::Leaved));
					}
					
					if(!currentInst->_eventActed &&
					(currentInst->_status >= WAIT_BEFORE_MARK_LONGCLICKED)) {
						_SETL(currentState, TOBYTE(ButtonHWState::Pushed));
						_SETH(currentState, TOBYTE(ButtonHWState::LongClicked));
						currentInst->_eventActed = TRUE;
					}
				}
				
				if(_ISH(currentState, TOBYTE(ButtonHWState::Leaved))) {
					
					if(!currentInst->_eventActed &&
					(currentInst->_status >= WAIT_BEFORE_MARK_CLICKED)) {
						_SETH(currentState, TOBYTE(ButtonHWState::Clicked));
					}
					
					currentInst->_status = 0;
					currentInst->_eventActed = FALSE;
					_SETL(currentState, TOBYTE(ButtonHWState::Leaved));
				}
				
				currentInst->_currentState = currentState;
				
				if(currentInst->GetHandlerPriority() == Button::CallPriority::High) {
					RaiseEvent(*currentInst);
				}
			}
		}
		
		static inline void CallIf(_ButtonEventCallBack callBack, const Button& sender) {
			if(callBack != nullptr)
			callBack(sender);
		}
		
		void Button::RaiseEvent(Button& btn) {
			switch (btn._currentState) {
				
				case (1 << TOBYTE(ButtonHWState::Clicked)): {
					CallIf(btn._click, btn);
					_SETL(btn._currentState, TOBYTE(ButtonHWState::Clicked));
				} break;
				
				case (1 << TOBYTE(ButtonHWState::LongClicked)): {
					CallIf(btn._longClick, btn);
					_SETL(btn._currentState, TOBYTE(ButtonHWState::LongClicked));
				} break;
				
				default:
				/* No action */
				break;
			}
		}
		
		Button::CallPriority Button::GetHandlerPriority() {
			return _priority;
		}
		
		void Button::SetHandlerPriority(const Button::CallPriority& priority) {
			_priority = priority;
		}
		
		void Button::CallSubroutines() {
			
			for (register fsize_t i = 0; i < Button::InstancesMax; i++)	{
				
				auto current = Button::GetInstance(i);
				
				if(current->GetHandlerPriority() == Button::CallPriority::Normal) {
					Button::RaiseEvent(*current);
				}
			}
		}
		
		void Button::Init() {
			
			if(is_inited)
			return;
			
			for(register fsize_t i = 0; i < Button::InstancesMax; i++) {
				Button::instances[i] = Button();
			}
			
			Timer::Init();
			initiator = Timer::GetNextInstance();
			initiator->SetInterval(BUTTON_CHECK_DELAY);
			initiator->SetAutoReset(TRUE);
			initiator->SubscribeHandler(Refresh);
			initiator->Start();
			
			is_inited = TRUE;
		}
		
		Button* Button::GetInstance(const fsize_t& idx) {
			if(idx < Button::InstancesMax)
			return &instances[idx];
			
			return nullptr;
		}
		
		Button* Button::GetNextInstance(const VLine& hwLine) {
			auto btn = Button::GetInstance(current_instance++);
			btn->ReferTo(hwLine);
			
			return btn;
		}
		
		l_t Button::IsPushed() {
			return _vLine.GetState() == LOW_STATE;
		}
		
		void Button::SetClickHandler(_ButtonEventCallBack callBack) {
			initiator->Stop();
			_click = callBack;
			initiator->Start();
		}
		
		void Button::SetLongClickHandler(_ButtonEventCallBack callBack) {
			initiator->Stop();
			_longClick = callBack;
			initiator->Start();
		}
		
		_ButtonEventCallBack Button::GetClickHandler() const {
			return _click;
		}
		
		_ButtonEventCallBack Button::GetLongClickHandler() const {
			return _longClick;
		}
		
		l_t Button::HasClickHandler() const {
			return _click != nullptr;
		}
		
		l_t Button::HasLongClickHandler() const {
			return _longClick != nullptr;
		}
		
		VLine Button::GetRef() const {
			return _vLine;
		}
		
		void Button::ReferTo(const VLine& hwLine) {
			_vLine = hwLine;
		}
	}
}