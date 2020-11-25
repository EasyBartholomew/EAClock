#ifndef __HWCOMPONENT_H__
#define __HWCOMPONENT_H__
#include "../globaldef.h"

namespace BaseAVR {
	namespace HAL {

		typedef volatile archb_type* pcomponent_t;

		class HWComponent {

		private:
			pcomponent_t _hwPORT;
			pcomponent_t _hwPIN;
			pcomponent_t _hwDDR;

		public:

			HWComponent(pcomponent_t hwPORT, pcomponent_t hwPIN, pcomponent_t hwDDR);
			HWComponent(const HWComponent&);
			HWComponent();

			pcomponent_t GetPORT() const;
			pcomponent_t GetPIN() const;
			pcomponent_t GetDDR() const;

			l_t operator==(const HWComponent&) const;
			l_t operator!=(const HWComponent&) const;

			static const HWComponent Nothing;
		};
	}
}
#endif //__HWCOMPONENT_H__
