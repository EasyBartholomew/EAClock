#ifndef __UIENTITY_H__
#define __UIENTITY_H__
#include "../BaseAVR/globaldef.h"
#include "../BaseAVR/IO/Button.h"

using namespace BaseAVR::IO;

namespace EAClock {
	namespace UI {
		
		typedef Button* pbutton_t;
		
		class UIEntity	{
			
			protected:
			
			l_t _state;
			u16_t _value;
			
			pbutton_t _up;
			pbutton_t _down;
			
			UIEntity(const l_t& state, Button* up, Button* down) {
				_state = state;
				_up = up;
				_down = down;
			}
			
			UIEntity(const l_t& state) : UIEntity(state, nullptr, nullptr)
			{ }
			
			UIEntity() : UIEntity(FALSE)
			{ }
			
			void SetValue(const u16_t& value) {
				_value = value;
			}
			
			public:
			
			virtual u16_t GetValue() const {
				return _value;
			}
			
			virtual l_t IsStarted() const {
				return _state;
			}
			
			virtual l_t IsTimeEntity() const {
				return FALSE;
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
