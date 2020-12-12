#ifndef __UIENTITY_HPP__
#define __UIENTITY_HPP__
#include "../BaseAVR/globaldef.h"
#include "../BaseAVR/IO/Button.h"

using namespace BaseAVR::IO;

#define INVALID_HANDLE 0xff

namespace EAClock {
	namespace UI {
		
		typedef Button* pbutton_t;
		typedef fsize_t handle_t;
		
		
		//Represents base class for any UI entity
		class UIEntity	{
			
			public:
			
			//Max length for data buffer
			static const fsize_t BufferMax = 0x4;
			
			private:
			
			handle_t _handle;
			handle_t _transitionTarget;
			handle_t _returnTarget;
			
			l_t _state;
			l_t _focus;
			
			protected:
			
			mutable u8_t _buffer[UIEntity::BufferMax];
			
			pbutton_t _up;
			pbutton_t _down;
			
			UIEntity(const l_t& state, pbutton_t up, pbutton_t down) {
				_state = state;
				_up = up;
				_down = down;
				_handle = INVALID_HANDLE;
				_returnTarget = 0;
				_focus = FALSE;
			}
			
			UIEntity(const l_t& state) : UIEntity(state, nullptr, nullptr)
			{ }
			
			UIEntity() : UIEntity(FALSE)
			{ }
			
			virtual u8_t * _GetBuffer() const {
				return _buffer;
			}
			
			public:
			
			handle_t GetHandle() {
				return _handle;
			}
			
			void SetHandle(const handle_t& handle) {
				_handle = handle;
			}
			
			virtual l_t IsFocused() {
				return _focus;
			}
			
			virtual void OnFocus()
			{ }
			
			virtual void OnFocusLost()
			{ }
			
			void GetFocus() {
				_focus = TRUE;
				OnFocus();
			}
			
			void LoseFocus() {
				_focus = FALSE;
				OnFocusLost();
			}
			
			virtual void OnStart()
			{ }
			
			virtual void OnStop()
			{ }
			
			void Start() {
				_state = TRUE;
				OnStart();
			}
			
			void Stop() {
				_state = FALSE;
				OnStop();
			}
			
			virtual void OnUiUpdate()
			{ }
			
			virtual u8_t* GetBufferPtr() {
				return _GetBuffer();
			}
			
			virtual u8_t const* GetConstBufferPtr() const {
				return _GetBuffer();
			}
			
			virtual l_t IsStarted() const {
				return _state;
			}
			
			virtual l_t IsTimeEntity() const {
				return FALSE;
			}
			
			virtual l_t IsMainUIEntity() const {
				return FALSE;
			}
			
			virtual void SetReturnTarget(const handle_t& target) {
				_returnTarget = target;
			}
			
			virtual handle_t GetReturnTarget() const {
				return _returnTarget;
			}
			
			virtual void TransitTo(const handle_t& handle) {
				_transitionTarget = handle;
			}
			
			virtual l_t IsTransitionTarget() const {
				return _transitionTarget != 0;
			}
			
			virtual handle_t GetTransitionTarget() const {
				return _transitionTarget;
			}
		};
		
		typedef UIEntity* pui_entity;
	}
}

#endif //__UIENTITY_HPP__
