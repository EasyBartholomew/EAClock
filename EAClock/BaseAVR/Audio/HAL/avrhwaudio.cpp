#include "avrhwaudio.h"
#include "avrhwAudioConfig.h"
#include "../../Time/HAL/avrhwtimer2.h"
#include <math.h>

using namespace BaseAVR::Time::HAL;

namespace BaseAVR{
	namespace Audio{
		namespace HAL{
			
			tu_t avrhwaudio::next = 0;
			tu_t avrhwaudio::interval = AVRHWAUDIO_DEFAULT_INTERVAL;
			l_t avrhwaudio::isInited = FALSE;
			
			l_t avrhwaudio::IsStarted(){
				return avrhwaudio::next != 0;
			}
			
			void avrhwaudio::Init(){
				if(!isInited){
					avrhwtimer2::Init();
					avrhwtimer2::SetHandler(avrhwaudio::hwTimerCallBack);
					
					_SETH(TARGET_DDR, TARGET_PIN);
					SET_TL
					
					isInited = TRUE;
				}
			}
			
			void avrhwaudio::SetFrequence(const freq_t& f){
				avrhwaudio::interval = (tu_t)round(1000000.0 / f);
			}
			
			void avrhwaudio::Start(const freq_t& f){
				avrhwaudio::SetFrequence(f);
				avrhwaudio::Start();
			}
			
			void avrhwaudio::Update(){
				avrhwaudio::next = avrhwtimer2::GetCurrentTiks() + avrhwaudio::interval;
			}
			
			void avrhwaudio::Start(){
				avrhwaudio::Update();
			}
			
			void avrhwaudio::Stop(){
				avrhwaudio::next = 0;
			}
			
			void avrhwaudio::hwTimerCallBack(const tu_t& us){
				if(avrhwaudio::IsStarted() && (avrhwaudio::next <= us)){
					SET_TI
					avrhwaudio::Update();
				}
			}
		}
	}
}
