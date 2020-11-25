#include "VLine.h"

namespace BaseAVR {
	namespace HAL {

		const line_t VLine::LineNotDefined = (line_t)0xffU;
		const IOMode VLine::DefaultMode = IOMode::Output;

		VLine::VLine(const HWComponent& hwcmp, const line_t& line, const IOMode& mode) {
			this->SetComponent(hwcmp);
			this->SetLine(line);
			this->SetMode(mode);
		}

		VLine::VLine(const HWComponent& hwcmp, const line_t& line)
			: VLine(hwcmp, line, VLine::DefaultMode)
		{	}

		VLine::VLine(const HWComponent& hwcmp)
			: VLine(hwcmp, VLine::LineNotDefined)
		{	}

		VLine::VLine(const IOMode& mode) {
			this->SetMode(mode);
		}

		VLine::VLine()
			: VLine(HWComponent::Nothing)
		{	}

		line_t VLine::GetLine() const {
			return _lineNumber;
		}

		void VLine::SetLine(const line_t& line) {

			if (line < ARCH_MAX)
				this->_lineNumber = line;
			else
				this->_lineNumber = VLine::LineNotDefined;
		}

		IOMode VLine::GetMode() const {
			return _currentMode;
		}

		void VLine::SetMode(const IOMode& mode) {
			_currentMode = mode;
		}

		HWComponent VLine::GetComponent() const {
			return _hwComponent;
		}

		void VLine::SetComponent(const HWComponent& hwcmp) {
			_hwComponent = hwcmp;
		}

		lstate_type VLine::GetState() const {
			if (!this->IsTargeted())
				return LOW_STATE;

			auto vval = *this->GetTatget();
			return _ISH(vval, _lineNumber) ? HIGH_STATE : LOW_STATE;
		}

		void VLine::SetState(const lstate_type& level) {
			if (!this->IsTargeted())
				return;

			_SETLVL(*this->GetTatget(), _lineNumber, level);
		}

		void VLine::High() {
			this->SetState(HIGH_STATE);
		}

		void VLine::Low() {
			this->SetState(LOW_STATE);
		}

		pcomponent_t VLine::GetTatget() const {
			switch (_currentMode)
			{
			case IOMode::Input: {
				_SETL(*_hwComponent.GetDDR(), _lineNumber);
				return _hwComponent.GetPIN();
			}

			case IOMode::Output: {
				_SETH(*_hwComponent.GetDDR(), _lineNumber);
				return _hwComponent.GetPORT();
			}
			}

			return nullptr;
		}

		l_t VLine::IsTargeted() const {
			return (_hwComponent != HWComponent::Nothing)
				&& (_lineNumber != VLine::LineNotDefined);
		}
	}
}
