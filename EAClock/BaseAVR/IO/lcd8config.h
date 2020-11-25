#ifndef LCD8CONFIG_H_
#define LCD8CONFIG_H_
#include "../globaldef.h"
#include "../macrodef.h"
#include <avr/io.h>

//Update interval in ms
#define LCD8_UPDATE_INTERVAL 4

//Current position
#define LCD8POS MKPORT(A)

//Current value
#define LCD8CUR MKPORT(C)

#define DDRLCD8POS MKDDR(A)
#define DDRLCD8CUR MKDDR(C)

namespace BaseAVR {
	namespace IO {
		
		/*Contains lcd8 digit code*/
		enum class lcd8symbol : u8_t {
			None = 0x00,
			Unknown = 0xff,
			Point = 0x80,
			Zero = 0b00111111,
			One = 0b00000110,
			Two = 0b01011011,
			Three = 0b01001111,
			Four = 0b01100110,
			Five = 0b01101101,
			Six = 0b01111101,
			Seven = 0b00000111,
			Eight = 0b01111111,
			Nine = 0b01101111,
			TopLine = 0b00000001,
			MiddleLine = 0b01000000,
			LowLine = 0b00001000,
			LetterA = 0b01110111,
			Letterb = 0b01111100,
			LetterC = 0b00111001,
			Letterd = 0b01011110,
			LetterE = 0b01111001,
			LetterF = 0b01110001,
			Letterh = 0b01110100,
			LetterJ = 0b00011110,
			LetterL = 0b00111000,
			LetterP = 0b01110011,
			Letterq = 0b01100111,
			LetterS = 0b01101101,
			Lettert = 0b01111000,
			LetterU = 0b00111110,
			LetterX = 0b01110110,
			LetterY = 0b01101110,
			LetterZ = 0b01011011
			
		};
		
		inline lcd8symbol dig2lcd(const u8_t& digit) {
			
			switch(digit) {
				
				case 0:
				return lcd8symbol::Zero;
				
				case 1:
				return lcd8symbol::One;
				
				case 2:
				return lcd8symbol::Two;
				
				case 3:
				return lcd8symbol::Three;
				
				case 4:
				return lcd8symbol::Four;
				
				case 5:
				return lcd8symbol::Five;
				
				case 6:
				return lcd8symbol::Six;
				
				case 7:
				return lcd8symbol::Seven;
				
				case 8:
				return lcd8symbol::Eight;
				
				case 9:
				return lcd8symbol::Nine;
				
				case '.':
				return lcd8symbol::Point;
				
				case '`':
				return lcd8symbol::TopLine;
				
				case '-':
				return lcd8symbol::MiddleLine;
				
				case '_':
				return lcd8symbol::LowLine;
				
				case 'A':
				return lcd8symbol::LetterA;
				
				case 'B':
				return lcd8symbol::Letterb;
				
				case 'C':
				return lcd8symbol::LetterC;
				
				case 'D':
				return lcd8symbol::Letterd;
				
				case 'E':
				return lcd8symbol::LetterE;
				
				case 'F':
				return lcd8symbol::LetterF;
				
				case 'H':
				return lcd8symbol::Letterh;
				
				case 'J':
				return lcd8symbol::LetterJ;
				
				case 'L':
				return lcd8symbol::LetterL;
				
				case 'P':
				return lcd8symbol::LetterP;
				
				case 'Q':
				return lcd8symbol::Letterq;
				
				case 'S':
				return lcd8symbol::LetterS;
				
				case 'T':
				return lcd8symbol::Lettert;
				
				case 'U':
				return lcd8symbol::LetterU;
				
				case 'X':
				return lcd8symbol::LetterX;
				
				case 'Y':
				return lcd8symbol::LetterY;
				
				case 'Z':
				return lcd8symbol::LetterZ;
				
				case 'a':
				return lcd8symbol::LetterA;
				
				case 'b':
				return lcd8symbol::Letterb;
				
				case 'c':
				return lcd8symbol::LetterC;
				
				case 'd':
				return lcd8symbol::Letterd;
				
				case 'e':
				return lcd8symbol::LetterE;
				
				case 'f':
				return lcd8symbol::LetterF;
				
				case 'h':
				return lcd8symbol::Letterh;
				
				case 'j':
				return lcd8symbol::LetterJ;
				
				case 'l':
				return lcd8symbol::LetterL;
				
				case 'p':
				return lcd8symbol::LetterP;
				
				case 'q':
				return lcd8symbol::Letterq;
				
				case 's':
				return lcd8symbol::LetterS;
				
				case 't':
				return lcd8symbol::Lettert;
				
				case 'u':
				return lcd8symbol::LetterU;
				
				case 'x':
				return lcd8symbol::LetterX;
				
				case 'y':
				return lcd8symbol::LetterY;
				
				case 'z':
				return lcd8symbol::LetterZ;
				
				case 0xff:
				return lcd8symbol::None;
				
				default:
				return lcd8symbol::Unknown;
			}
		}
		
		inline u8_t lcd2char(const lcd8symbol& symbol) {
			
			u8_t code = TOBYTE(symbol);
			
			if(code == TOBYTE(lcd8symbol::Point)) {
				return '.';
			}
			
			if(TOBYTE(code) & TOBYTE(lcd8symbol::Point)) {
				code &= ~TOBYTE(lcd8symbol::Point);
			}
			
			switch ((lcd8symbol)code) {
				
				case lcd8symbol::Zero:
				return 0;
				
				case lcd8symbol::One:
				return 1;
				
				case lcd8symbol::Two:
				return 2;
				
				case lcd8symbol::Three:
				return 3;
				
				case lcd8symbol::Four:
				return 4;
				
				case lcd8symbol::Five:
				return 5;
				
				case lcd8symbol::Six:
				return 6;
				
				case lcd8symbol::Seven:
				return 7;
				
				case lcd8symbol::Eight:
				return 8;
				
				case lcd8symbol::Nine:
				return 9;
				
				case lcd8symbol::None:
				return 0xff;
				
				case lcd8symbol::Unknown:
				default:
				return 0xfe;
			}
		}
	}
}

#endif /* LCD8CONFIG_H_ */