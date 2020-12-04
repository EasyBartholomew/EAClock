#ifndef AVRHWAUDIO_H_
#define AVRHWAUDIO_H_
#include "../../globaldef.h"

#define AVRHWAUDIO_DEFAULT_INTERVAL 1000

namespace BaseAVR{
	namespace Audio{
		namespace HAL{
			
			typedef d32_t freq_t;
			
			class avrhwaudio{
				private:
				avrhwaudio();
				avrhwaudio(const avrhwaudio&);
				
				static tu_t next;
				static tu_t interval;
				static l_t isInited;
				
				static void Update();
				static void hwTimerCallBack(const tu_t&);
				
				public:
				
				static l_t IsStarted();
				static void Init();
				static void SetFrequence(const freq_t&);
				static void Start(const freq_t&);
				static void Start();
				static void Stop();
			};
		}
	}
}

#endif /* AVRHWAUDIO_H_ */