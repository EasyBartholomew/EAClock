#pragma once
#ifndef _TIMESPAN_H_
#define _TIMESPAN_H_
#include "../globaldef.h"

namespace BaseAVR {
	namespace Time {

		class TimeSpan {

			private:

			tu_t _total;

			public:

			tu_t GetMilliseconds() const;

			tu_t GetSeconds() const;

			tu_t GetMinutes() const;

			tu_t GetHours() const;

			tu_t GetDays() const;

			tu_t GetTotalMilliseconds() const;
			
			tu_t GetTotalSeconds() const;
			
			tu_t GetTotalMinutes() const;
			
			tu_t GetTotalHours() const;

			explicit TimeSpan(const tu_t& ms);

			TimeSpan();

			TimeSpan(
			const tu_t& days,
			const tu_t& hours,
			const tu_t& minutes,
			const tu_t& seconds,
			const tu_t& milliseconds
			);

			TimeSpan& AddMilliseconds(const tu_t&);
			TimeSpan& AddSeconds(const tu_t&);
			TimeSpan& AddMinutes(const tu_t&);
			TimeSpan& AddHours(const tu_t&);
			TimeSpan& AddDays(const tu_t&);

			TimeSpan& SubtractMilliseconds(const tu_t&);
			TimeSpan& SubtractSeconds(const tu_t&);
			TimeSpan& SubtractMinutes(const tu_t&);
			TimeSpan& SubtractHours(const tu_t&);
			TimeSpan& SubtractDays(const tu_t&);
			
			TimeSpan GetCopy() const;
			
			TimeSpan operator+(const TimeSpan&) const;
			TimeSpan operator-(const TimeSpan&) const;
			TimeSpan operator-() const;
			
			l_t operator==(const TimeSpan&) const;
			l_t operator!=(const TimeSpan&) const;
			l_t operator>(const TimeSpan&) const;
			l_t operator<(const TimeSpan&) const;
			l_t operator>=(const TimeSpan&) const;
			l_t operator<=(const TimeSpan&) const;
			
			static const TimeSpan Zero;
			static const TimeSpan MaxValue;
			static const TimeSpan MinValue;
			
			static TimeSpan FromSeconds(const tu_t&);
			static TimeSpan FromMinutes(const tu_t&);
			static TimeSpan FromHours(const tu_t&);
			static TimeSpan FromDays(const tu_t&);
		};
	}
}

#endif // !_TIMESPAN_H_
