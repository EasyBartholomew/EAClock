#ifndef _LCD8_H_
#define _LCD8_H_
#include "../globaldef.h"
#include "../Time/TimeSpan.h"
#include "../Time/Timer.h"

using namespace BaseAVR::Time;

#define LCD8_MAXDIG 4

#define _ISDIG(VAL) ((VAL >= '0') && (VAL <= '9'))


namespace BaseAVR {
	namespace IO {

		/*Describes LCD8 position*/
		enum class lcd8position : u8_t {
			First = 3,
			Second = 2,
			Third = 1,
			Fourth = 0,
			NoPosition = 0xff
		};


		class lcd8 {

			private:
			
			static l_t isInited;
			static l_t useZeroAsDefaultDigit;
			
			static u8_t current[LCD8_MAXDIG];
			static Timer* updater;
			
			static u8_t pointFlags;
			
			static u16_t DefaultDigit();
			
			public:
			
			static void Init();
			static void UseZeroAsDefaultDigit(const l_t&);
			
			static void Write(const u8_t& digit, const lcd8position& pos, const l_t& point);
			static void Write(const u8_t& digit, const lcd8position& pos);
			static void Write(u8_t const*const digits, const lcd8position& pointPosition);
			static void Write(u8_t const*const digits);
			static void Write(u16_t number);
			static void Clear();
			
			static u8_t ReadAt(const lcd8position& pos);
			
			static l_t PointAt(const lcd8position& pos);
			static void PointAt(const lcd8position& pos, const l_t& status);
			
			static void Flush();
			
			static u8_t* GetBufferPtr();
			static u8_t const* GetConstBufferPtr();
			
			static const u16_t MaxValue;
			static const u16_t MinValue;
			static const u16_t NoDigit;
		};
	}
}

#endif // !_LCD8_H_