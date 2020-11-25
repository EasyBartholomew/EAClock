#ifndef __VLINE_H__
#define __VLINE_H__
#include "../globaldef.h"
#include "../macrodef.h"
#include "HWComponent.h"

typedef l_t lstate_type;
typedef archb_type line_t;

namespace BaseAVR {
	namespace HAL {

		enum class IOMode : u8_t {
			Input,
			Output
		};

		class VLine {
			private:
			HWComponent _hwComponent;
			line_t _lineNumber;
			IOMode _currentMode;

			pcomponent_t GetTatget() const;

			public:

			VLine(const HWComponent& hwcmp, const line_t& line, const IOMode& mode);
			VLine(const HWComponent& hwcmp, const line_t& line);
			explicit VLine(const HWComponent& hwcmp);
			explicit VLine(const IOMode& mode);
			VLine();

			HWComponent GetComponent() const;
			line_t GetLine() const;
			lstate_type GetState() const;
			l_t IsTargeted() const;
			IOMode GetMode() const;

			void SetMode(const IOMode&);
			void SetComponent(const HWComponent& hwcmp);
			void SetLine(const line_t& line);

			void SetState(const lstate_type&);
			void High();
			void Low();

			static const line_t LineNotDefined;
			static const IOMode DefaultMode;
		};
	}
}
#endif //__VLINE_H__
