#include "Timer.h"
#include "HAL/avrhwtimer0.h"

namespace BaseAVR {
	namespace Time {
		
		CallPriority Timer::GetHandlerPriority() const {
			return _priority;
		}
		
		void Timer::SetHandlerPriority(const CallPriority& priority) {
			_priority = priority;
		}

		l_t Timer::IsStarted() const {
			return _next != 0;
		}

		l_t Timer::GetAutoReset() const {
			return _autoReset;
		}

		l_t Timer::HasHandler() const {
			return _callback != nullptr;
		}

		void Timer::SetAutoReset(const l_t& val) {
			_autoReset = val;
		}

		TimeSpan Timer::GetInterval() const {
			return TimeSpan(_interval);
		}
		
		tu_t Timer::GetMsInterval() const {
			return this->_interval;
		}
		
		void Timer::SetInterval(const tu_t& ms) {
			this->SetInterval(TimeSpan(ms));
		}
		
		void Timer::SetInterval(const TimeSpan& interval) {

			if (interval.GetTotalMilliseconds() < 0) {
				_interval = DEFAULT_TIMER_INTERVAL.GetTotalMilliseconds();
				return;
			}

			_interval = interval.GetTotalMilliseconds();
		}

		void Timer::SubscribeHandler(_Timer_Handler_Callback callback) {
			_callback = callback;
		}

		void Timer::UnsubscribeHandler() {
			_callback = nullptr;
		}

		void Timer::Start() {
			_next = HAL::avrhwtimer0::GetCurrentTiks() + _interval;
		}

		void Timer::Stop() {
			_next = 0;
		}

		Timer::Timer() : Timer(TimeSpan(1000))
		{	}

		Timer::Timer(const TimeSpan& interval) {

			this->SetInterval(interval);
			this->SetAutoReset(0);
			_callback = nullptr;
			_next = 0;
			_elapsed = FALSE;
			_priority = CallPriority::Normal;
		}

		Timer::Timer(const tu_t& msInterval) : Timer(TimeSpan(msInterval))
		{	}

		Timer::Timer(const Timer& other) {
			this->_next = other._next;
			this->_autoReset = other._autoReset;
			this->_interval = other._interval;
			this->_callback = other._callback;
			this->_priority = other._priority;
			this->_elapsed = other._elapsed;
		}

		void Timer::hTimerCallback(const tu_t& current) {
			
			for (register fsize_t i = 0; i < TIMER_INST_MAX; i++) {
				Timer* currentInst = &Timer::instances[i];
				
				if (currentInst->IsStarted() && currentInst->_callback != nullptr) {
					
					if (currentInst->_next <= current) {
						
						if(currentInst->_priority == CallPriority::High) {
							currentInst->_callback();
						}
						else if(currentInst->_priority == CallPriority::Normal) {
							currentInst->_elapsed = TRUE;
						}
						
						if (currentInst->_autoReset)
						currentInst->_next += currentInst->_interval;
						else
						currentInst->Stop();
					}
				}
			}
		}

		void Timer::Init() {
			if(!isInited){
				HAL::avrhwtimer0::HandlerBlock();
				HAL::avrhwtimer0::SetHandler(Timer::hTimerCallback);
				HAL::avrhwtimer0::HandlerResort();
				HAL::avrhwtimer0::Init();
				
				for(register fsize_t i = 0; i < TIMER_INST_MAX; i++){
					Timer::instances[i] = Timer();
				}
				
				isInited = TRUE;
			}
		}

		Timer* const Timer::GetInstance(const fsize_t& idx) {
			if (idx >= TIMER_INST_MAX)
			return nullptr;

			return &Timer::instances[idx];
		}

		Timer* const Timer::GetNextInstance() {
			return Timer::GetInstance(current_instance++);
		}
		
		Timer* const Timer::GetNextInstance(const TimeSpan& span) {
			auto ret = Timer::GetNextInstance();
			ret->SetInterval(span);
			
			return ret;
		}
		
		Timer* const Timer::GetNextInstance(const tu_t& ms) {
			return Timer::GetNextInstance(TimeSpan(ms));
		}
		
		Timer* const Timer::GetFreeInstance() {
			for (register fsize_t i = 0; i < TIMER_INST_MAX; i++) {
				
				auto current = Timer::GetInstance(i);
				
				if(!current->HasHandler())
				return current;
			}
			
			return nullptr;
		}
		
		void Timer::CallSubroutines() {
			
			for(register fsize_t i = 0; i < TIMER_INST_MAX; i++) {
				auto current = Timer::GetInstance(i);
				
				if(current->_priority == CallPriority::Normal) {
					if(current->_elapsed) {
						current->_callback();
						current->_elapsed = FALSE;
					}
				}
			}
		}
		
		fsize_t Timer::current_instance = 0;
		l_t Timer::isInited = FALSE;
		Timer Timer::instances[TIMER_INST_MAX];
	}
}