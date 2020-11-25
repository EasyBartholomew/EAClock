#include "BaseAVR/globaldef.h"
#include "BaseAVR/macrodef.h"
#include "BaseAVR/Time/TimeSpan.h"
#include "BaseAVR/Time/await.h"
#include "BaseAVR/IO/lcd8.h"
#include "BaseAVR/IO/Button.h"
#include "UI/UIManager.hpp"

using namespace BaseAVR;
using namespace BaseAVR::IO;
using namespace BaseAVR::Time;


int main() {
	
	__CLI
	lcd8::Init();
	lcd8::UseZeroAsDefaultDigit(FALSE);
	Button::Init();
	__SEI
	
	EAClock::UI::UIManager::Init();
	
	while(1){
		Button::CallSubroutines();
	}
}