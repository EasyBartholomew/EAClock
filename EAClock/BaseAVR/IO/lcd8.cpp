#include "lcd8.h"
#include "../macrodef.h"
#include "lcd8config.h"


namespace BaseAVR {
	namespace IO {

		u16_t lcd8::DefaultDigit(){
			return TOBYTE(dig2lcd(lcd8::useZeroAsDefaultDigit ? 0 : lcd8::NoDigit));
		}
		
		void lcd8::Init() {
			
			if(lcd8::isInited)
			return;
			
			//Current position
			_SETH(DDRLCD8POS, D0);
			_SETH(DDRLCD8POS, D1);
			_SETH(DDRLCD8POS, D2);
			_SETH(DDRLCD8POS, D3);
			
			_SETL(LCD8POS, D0);
			_SETL(LCD8POS, D1);
			_SETL(LCD8POS, D2);
			_SETL(LCD8POS, D3);
			
			//Current value
			_CL(LCD8CUR);
			_SETVAL(DDRLCD8CUR, DDR_OUT);
			
			for(register u8_t i = 0; i < LCD8_MAXDIG; i++)
			current[i] = lcd8::DefaultDigit();
			
			//Timer init
			Timer::Init();
			lcd8::updater = Timer::GetNextInstance();
			updater->SetInterval(TimeSpan(LCD8_UPDATE_INTERVAL));
			updater->SetAutoReset(1);
			updater->SubscribeHandler(lcd8::Flush);
			updater->Start();
			
			lcd8::isInited = TRUE;
		}
		
		void lcd8::UseZeroAsDefaultDigit(const l_t& val){
			lcd8::useZeroAsDefaultDigit = val;
		}
		
		void lcd8::Write(const u8_t& digit, const lcd8position& pos, const l_t& point){
			
			if(pos == lcd8position::NoPosition)
			return;
			
			u8_t s_code = TOBYTE(dig2lcd(digit));
			
			if(point || lcd8::PointAt(pos))
			s_code |= TOBYTE(lcd8symbol::Point);
			
			lcd8::current[TOBYTE(pos)] = s_code;
		}
		
		void lcd8::Write(const u8_t& digit, const lcd8position& pos){
			lcd8::Write(digit, pos, FALSE);
		}
		
		void lcd8::Write(u8_t const*const digits, const lcd8position& pointPosition){
			for(register u8_t i = 0; i < LCD8_MAXDIG; i++) {
				
				if(digits[i] == 'G') {
					return;
				}
				
				lcd8::Write(digits[i], (lcd8position)i, TOBYTE(pointPosition) == i);
			}
		}
		
		void lcd8::Write(u8_t const*const digits) {
			lcd8::Write(digits, lcd8position::NoPosition);
		}
		
		void lcd8::Write(u16_t number){
			
			for(register u8_t i = 0; i < LCD8_MAXDIG; i++){
				
				if(!number){
					current[i] = DefaultDigit();
					continue;
				}
				
				lcd8::Write(number % 10, (lcd8position)i);
				number /= 10;
			}
		}
		
		void lcd8::Clear() {
			
			for(register fsize_t i = 0; i < LCD8_MAXDIG; i++) {
				current[i] = DefaultDigit();
			}
		}
		
		void lcd8::Flush() {
			static u8_t current_position = 0;
			
			//This part is not optimized!
			_SETL(LCD8POS, D0);
			_SETL(LCD8POS, D1);
			_SETL(LCD8POS, D2);
			_SETL(LCD8POS, D3);
			
			_SETH(LCD8POS, current_position);
			_SETVAL(LCD8CUR, lcd8::current[current_position]);
			current_position++;
			
			if(!(current_position < LCD8_MAXDIG))
			current_position = 0;
		}
		
		l_t lcd8::PointAt(const lcd8position& pos) {
			
			if(pos == lcd8position::NoPosition) {
				return FALSE;
			}
			
			return _ISH(lcd8::pointFlags, TOBYTE(pos));
		}
		
		void lcd8::PointAt(const lcd8position& pos, const l_t& status) {
			
			if(pos == lcd8position::NoPosition) {
				return;
			}
			
			_SETLVL(lcd8::pointFlags, TOBYTE(pos), status);
			
			if(status)
			lcd8::current[TOBYTE(pos)] |= TOBYTE(lcd8symbol::Point);
			else
			lcd8::current[TOBYTE(pos)] &= ~TOBYTE(lcd8symbol::Point);
		}
		
		u8_t lcd8::ReadAt(const lcd8position& pos) {
			
			if(pos == lcd8position::NoPosition) {
				return lcd2char(lcd8symbol::None);
			}
			
			return lcd2char((lcd8symbol)current[TOBYTE(pos)]);
		}
		
		u8_t* lcd8::GetBufferPtr() {
			return lcd8::current;
		}
		
		u8_t const* lcd8::GetConstBufferPtr() {
			return lcd8::current;
		}
		
		u8_t lcd8::current[LCD8_MAXDIG];
		l_t lcd8::isInited = FALSE;
		l_t lcd8::useZeroAsDefaultDigit = FALSE;
		Timer* lcd8::updater = nullptr;
		u8_t lcd8::pointFlags = 0;
		
		const u16_t lcd8::MinValue = 0;
		const u16_t lcd8::MaxValue = 9999;
		const u16_t lcd8::NoDigit = 0x00ff;
	}
}