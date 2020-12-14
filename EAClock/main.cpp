#include "App.hpp"


using namespace EAClock;
using namespace BaseAVR::Time;


int main() {
	
	//Initialization of usable modules
	App::Init();
	
	//Showing splash screen for 2 seconds
	App::ShowSplashScreenFor(TimeSpan::FromSeconds(2));
	
	//Starting main program
	App::Start();
	
	
	while(1) {
		App::EventLoop();
	}
}