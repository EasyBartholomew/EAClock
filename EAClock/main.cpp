#include "App.hpp"


using namespace EAClock;
using namespace BaseAVR::Time;


int main() {
	
	//Init of usable modules
	App::Init();
	
	//Showing splash screen for 3 seconds
	App::ShowSplashScreenFor(TimeSpan::FromSeconds(1));
	
	//Starting main program
	App::Start();
	
	
	while(1) {
		App::EventLoop();
	}
}