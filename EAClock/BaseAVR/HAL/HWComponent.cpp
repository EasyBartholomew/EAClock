#include "HWComponent.h"

namespace BaseAVR {
	namespace HAL {

		HWComponent::HWComponent(
			pcomponent_t hwPORT,
			pcomponent_t hwPIN,
			pcomponent_t hwDDR) {
			_hwPORT = hwPORT;
			_hwPIN = hwPIN;
			_hwDDR = hwDDR;
		}

		HWComponent::HWComponent(const HWComponent& other) {
			_hwPORT = other._hwPORT;
			_hwPIN = other._hwPIN;
			_hwDDR = other._hwDDR;
		}

		HWComponent::HWComponent() : HWComponent(nullptr, nullptr, nullptr)
		{ }

		pcomponent_t HWComponent::GetPORT() const {
			return _hwPORT;
		}

		pcomponent_t HWComponent::GetPIN() const {
			return _hwPIN;
		}

		pcomponent_t HWComponent::GetDDR() const {
			return _hwDDR;
		}

		l_t HWComponent::operator==(const HWComponent& other) const {
			return (this->_hwPORT == other._hwPORT)
				&& (this->_hwPIN == other._hwPIN)
				&& (this->_hwDDR == other._hwDDR);
		}

		l_t HWComponent::operator!=(const HWComponent& other) const {
			return !(*this == other);
		}

		const HWComponent HWComponent::Nothing(nullptr, nullptr, nullptr);
	}
}