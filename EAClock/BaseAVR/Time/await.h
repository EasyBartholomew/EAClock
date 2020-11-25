#ifndef AWAIT_H_
#define AWAIT_H_
#include "../globaldef.h"

namespace BaseAVR {
	
	namespace Await {
		
		void While(const l_t&);
		void WhileNot(const l_t&);
		void ForMs(const tu_t&);
		void ForUs(const tu_t&);
	}
}

#endif /* AWAIT_H_ */