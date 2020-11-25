#ifndef __BUTTON_H__
#define __BUTTON_H__
#include "../globaldef.h"
#include "../HAL/VLine.h"
#include "../Time/Timer.h"


namespace BaseAVR {
	namespace IO {
		
		enum class ButtonHWState : u8_t {
			Leaved,
			Pushed,
			Clicked,
			LongClicked
		};

		
		class Button;
		
		typedef void (*_ButtonEventCallBack)(const Button& sender);
		
		class Button {
			
			public:
			
			enum class CallPriority : u8_t {
				Normal,
				High
			};
			
			private:
			
			BaseAVR::HAL::VLine _vLine;
			
			tu_t _pushed4;
			tu_t _status;
			u8_t _currentState;
			l_t _eventActed;
			
			CallPriority _priority;
			
			l_t IsPushed();
			
			_ButtonEventCallBack _click;
			_ButtonEventCallBack _longClick;
			
			void SetEventHandler(_ButtonEventCallBack target, _ButtonEventCallBack value);
			
			Button();
			
			public:
			
			void SetClickHandler(_ButtonEventCallBack callBack);
			void SetLongClickHandler(_ButtonEventCallBack callBack);
			
			_ButtonEventCallBack GetClickHandler() const;
			_ButtonEventCallBack GetLongClickHandler() const;
			
			l_t HasClickHandler() const;
			l_t HasLongClickHandler() const;
			
			void ReferTo(const BaseAVR::HAL::VLine& hwLine);
			BaseAVR::HAL::VLine GetRef() const;
			l_t IsRefered() const;
			
			CallPriority GetHandlerPriority();
			void SetHandlerPriority(const CallPriority&);
			
			//Static
			public:
			static const fsize_t InstancesMax = 0x10;
			static void Init();
			
			static Button* GetInstance(const fsize_t&);
			static Button* GetNextInstance(const BaseAVR::HAL::VLine&);
			
			static void CallSubroutines();
			
			private:
			static BaseAVR::Time::Timer* initiator;
			static Button instances[Button::InstancesMax];
			static fsize_t current_instance;
			static l_t is_inited;
			
			static void Refresh();
			static void RaiseEvent(Button&);
		};
	}
}

#endif //__BUTTON_H__
