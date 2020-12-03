#ifndef __LM35_H__
#define __LM35_H__
#include "../../../globaldef.h"

namespace BaseAVR {
	namespace IO {
		namespace Sensors {
			class LM35 {
				
				private:
				static l_t is_inited;
				
				LM35();
				LM35(const LM35&);
				
				static u16_t _dataconvert(const u16_t& code);
				
				public:
				
				static void Init();
				static u16_t MeasureAndGetCValue();
			};
		}
	}
}

#endif //__LM35_H__
