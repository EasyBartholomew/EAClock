#include "TimeSpan.h"
#include <stdint-gcc.h>

namespace BaseAVR {

	namespace Time {

		tu_t TimeSpan::GetMilliseconds() const {
			return _total % 1000;
		}

		tu_t TimeSpan::GetSeconds() const {
			return (_total / 1000) % 60;
		}

		tu_t TimeSpan::GetMinutes() const {
			return (this->GetTotalSeconds()/60) % 60;
		}

		tu_t TimeSpan::GetHours() const {
			return (this->GetTotalMinutes() / 60) % 24;
		}

		tu_t TimeSpan::GetDays() const {
			return this->GetTotalHours() / 24;
		}

		tu_t TimeSpan::GetTotalMilliseconds() const {
			return _total;
		}
		
		tu_t TimeSpan::GetTotalSeconds() const {
			return _total / 1000;
		}
		
		tu_t TimeSpan::GetTotalMinutes() const {
			return (_total / 1000) / 60;
		}

		tu_t TimeSpan::GetTotalHours() const {
			return ((_total / 1000) /60 /60) / 24;
		}
		
		
		TimeSpan& TimeSpan::AddMilliseconds(const tu_t& ms) {

			_total += ms;
			return *this;
		}

		TimeSpan& TimeSpan::AddSeconds(const tu_t& seconds) {
			return this->AddMilliseconds(seconds * 1000);
		}

		TimeSpan& TimeSpan::AddMinutes(const tu_t& minutes) {
			return this->AddSeconds(minutes * 60);
		}

		TimeSpan& TimeSpan::AddHours(const tu_t& hours) {
			return this->AddMinutes(hours * 60);
		}

		TimeSpan& TimeSpan::AddDays(const tu_t& days) {
			return this->AddHours(days * 24);
		}

		TimeSpan& TimeSpan::SubtractMilliseconds(const tu_t& ms) {
			return this->AddMilliseconds(-ms);
		}

		TimeSpan& TimeSpan::SubtractSeconds(const tu_t& seconds) {
			return this->AddSeconds(-seconds);
		}

		TimeSpan& TimeSpan::SubtractMinutes(const tu_t& minutes) {
			return this->AddMinutes(-minutes);
		}

		TimeSpan& TimeSpan::SubtractHours(const tu_t& hours) {
			return this->AddHours(-hours);
		}

		TimeSpan& TimeSpan::SubtractDays(const tu_t& days) {
			return this->AddDays(-days);
		}

		TimeSpan::TimeSpan(const tu_t& ms) {
			_total = ms;
		}
		
		TimeSpan TimeSpan::GetCopy() const{
			return TimeSpan(*this);
		}

		TimeSpan::TimeSpan(
		const tu_t& days,
		const tu_t& hours,
		const tu_t& minutes,
		const tu_t& seconds,
		const tu_t& milliseconds
		) : TimeSpan() {

			this->AddDays(days)
			.AddHours(hours)
			.AddMinutes(minutes)
			.AddSeconds(seconds)
			.AddMilliseconds(milliseconds);
		}

		TimeSpan::TimeSpan() : TimeSpan(0)
		{	}

		TimeSpan TimeSpan::operator+(const TimeSpan& _rght) const {
			return TimeSpan(this->GetTotalMilliseconds() + _rght.GetTotalMilliseconds());
		}

		TimeSpan TimeSpan::operator-() const {
			return TimeSpan(-this->GetTotalMilliseconds());
		}

		TimeSpan TimeSpan::operator-(const TimeSpan& _rght) const {
			return this->operator+(-_rght);
		}
		
		const TimeSpan TimeSpan::Zero(0);
		const TimeSpan TimeSpan::MinValue(INT64_MIN);
		const TimeSpan TimeSpan::MaxValue(INT64_MAX);
		
		
		TimeSpan TimeSpan::FromSeconds(const tu_t& seconds){
			return TimeSpan().AddSeconds(seconds);
		}
		
		TimeSpan TimeSpan::FromMinutes(const tu_t& minutes){
			return TimeSpan().AddMinutes(minutes);
		}
		
		TimeSpan TimeSpan::FromHours(const tu_t& hours){
			return TimeSpan().AddHours(hours);
		}
		
		TimeSpan TimeSpan::FromDays(const tu_t& days){
			return TimeSpan().AddDays(days);
		}
		
		l_t TimeSpan::operator==(const TimeSpan& other) const {
			return this->GetTotalMilliseconds() == other.GetTotalMilliseconds();
		}
		
		l_t TimeSpan::operator!=(const TimeSpan& other) const {
			return this->GetTotalMilliseconds() != other.GetTotalMilliseconds();
		}
		
		l_t TimeSpan::operator>(const TimeSpan& other) const{
			return this->GetTotalMilliseconds() > other.GetTotalMilliseconds();
		}
		
		l_t TimeSpan::operator<(const TimeSpan& other) const{
			return this->GetTotalMilliseconds() < other.GetTotalMilliseconds();
		}
		
		l_t TimeSpan::operator>=(const TimeSpan& other) const{
			return this->GetTotalMilliseconds() >= other.GetTotalMilliseconds();
		}
		
		l_t TimeSpan::operator<=(const TimeSpan& other) const{
			return this->GetTotalMilliseconds() <= other.GetTotalMilliseconds();
		}
	}
}