#ifndef __UIENTITY_H__
#define __UIENTITY_H__
#include "../BaseAVR/globaldef.h"
#include "../BaseAVR/IO/Button.h"

using namespace BaseAVR::IO;

namespace EAClock {
	namespace UI {
		
		typedef Button* pbutton_t;
		
		class UIEntity	{
			
			public:
			
			static const fsize_t BufferMax = 0x4;
			
			protected:
			
			l_t _state;
			l_t _focus;
			
			mutable u8_t _buffer[UIEntity::BufferMax];
			
			pbutton_t _up;
			pbutton_t _down;
			
			UIEntity(const l_t& state, Button* up, Button* down) {
				_state = state;
				_up = up;
				_down = down;
				
				_focus = FALSE;
			}
			
			UIEntity(const l_t& state) : UIEntity(state, nullptr, nullptr)
			{ }
			
			UIEntity() : UIEntity(FALSE)
			{ }
			
			public:
			
			virtual l_t IsFocused() {
				return _focus;
			}
			
			virtual void OnFocus() {
				_focus = TRUE;
			}
			
			virtual void OnFocusLost() {
				_focus = FALSE;
			}
			
			virtual u8_t* GetBufferPtr() {
				return _buffer;
			}
			
			virtual u8_t const* GetConstBufferPtr() const {
				return _buffer;
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
			
			virtual l_t IsTransitionTarget() const {
				return 0;
			}
			
			virtual fsize_t GetTransitionTarget() const {
				return 0;
			}
			
			virtual void Start() {
				_state = TRUE;
			}
			
			virtual void Stop() {
				_state = FALSE;
			}
		};
		
		typedef UIEntity* pui_entity;
	}
}

#endif //__UIENTITY_H__
