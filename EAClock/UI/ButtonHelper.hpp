#ifndef __BUTTONHELPER_HPP__
#define __BUTTONHELPER_HPP__
#include "../BaseAVR/IO/Button.h"

using namespace BaseAVR::IO;

namespace EAClock {
	namespace UI {
		
		class ButtonHelper {
			
			private:
			ButtonHelper();
			ButtonHelper(const ButtonHelper&);
			
			public:
			
			static _ButtonEventCallBack _upClickHandlerBackup;
			static _ButtonEventCallBack _downClickHandlerBackup;
			static _ButtonEventCallBack _upLongClickHandlerBackup;
			static _ButtonEventCallBack _downLongClickHandlerBackup;
		};
		
		_ButtonEventCallBack ButtonHelper::_upClickHandlerBackup = nullptr;
		_ButtonEventCallBack ButtonHelper::_downClickHandlerBackup = nullptr;
		_ButtonEventCallBack ButtonHelper::_upLongClickHandlerBackup = nullptr;
		_ButtonEventCallBack ButtonHelper::_downLongClickHandlerBackup = nullptr;
	}
}
#endif //__BUTTONHELPER_H__
