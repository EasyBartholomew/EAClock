#ifndef _TIMER_H_
#define _TIMER_H_
#include "../globaldef.h"
#include "TimeSpan.h"

#define DEFAULT_TIMER_INTERVAL TimeSpan(1)
#define TIMER_INST_MAX 16

namespace BaseAVR {
	namespace Time {

		typedef void (*_Timer_Handler_Callback)();

		class Timer {
			private:
			tu_t _interval;
			l_t _autoReset;
			tu_t _next;

			_Timer_Handler_Callback _callback;

			Timer(const Timer&);
			Timer();
			Timer(const TimeSpan&);
			Timer(const tu_t&);

			static Timer instances[TIMER_INST_MAX];
			static l_t isInited;
			static fsize_t current_instance;
			static void hTimerCallback(const tu_t&);

			public:

			TimeSpan GetInterval() const;
			tu_t GetMsInterval() const;
			l_t GetAutoReset() const;
			l_t IsStarted() const;
			l_t HasHandler() const;

			void SetInterval(const TimeSpan&);
			void SetInterval(const tu_t&);
			void SetAutoReset(const l_t&);
			void SubscribeHandler(_Timer_Handler_Callback);
			void UnsubscribeHandler();

			static Timer* const GetInstance(const fsize_t&);
			static Timer* const GetNextInstance();
			static Timer* const GetNextInstance(const TimeSpan&);
			static Timer* const GetNextInstance(const tu_t&);
			static Timer* const GetFreeInstance();
			
			void Start();
			void Stop();

			static void Init();
		};
	}
}


#endif // !_TIMER_H_
