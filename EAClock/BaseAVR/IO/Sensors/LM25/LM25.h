#ifndef __LM25_H__
#define __LM25_H__
#include "../../../globaldef.h"

namespace BaseAVR {
	namespace IO {
		namespace Sensors {
			class LM25 {
				
				private:
				l_t is_inited;
				
				LM25();
				LM25(const LM25&);
				
				static u16_t _dataconvert(const u16_t& code);
				
				public:
				
				static void Init();
				static u16_t MeasureAndGetCValue();
			};
		}
	}
}

#endif //__LM25_H__
