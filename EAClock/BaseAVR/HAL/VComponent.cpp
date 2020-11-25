#include "VComponent.h"
#include <string.h>


namespace BaseAVR {
	namespace HAL {

		VComponent::VComponent(
			VLine* const lines,
			const fsize_t& lsize,
			const IOMode& mode,
			const l_t& alwaysFlush,
			const l_t& alwaysUpdate
		) {
			memcpy(&_lines, lines, lsize);
			_virtual = this->Resort();
			this->SetAlwaysFlush(alwaysFlush);
			this->SetVirtualUpdate(alwaysUpdate);
			this->SetMode(mode);
		}

		VComponent::VComponent(
			const IOMode& lineMode,
			const l_t& alwaysFlush,
			const l_t& alwaysUpdate) {

			this->SetAlwaysFlush(alwaysFlush);
			this->SetVirtualUpdate(alwaysUpdate);

			for (register fsize_t i = 0; i < ARCH_MAX; i++)
				_lines[i] = VLine();

			this->SetMode(lineMode);
			_virtual = this->Resort();
		}

		VComponent::VComponent(const l_t& alwaysFlush, const l_t& alwaysUpdate) :
			VComponent(IOMode::Output, alwaysFlush, alwaysUpdate)
		{ }

		VComponent::VComponent() : VComponent(FALSE)
		{ }

		void VComponent::ResortVirtual() const {
			_virtual = this->Resort();
		}

		void VComponent::ResortVirtualIf() const {
			if (_alwaysUpdate)
				this->ResortVirtual();
		}

		void VComponent::RefreshVirtual() {
			this->ResortVirtual();
		}

		l_t VComponent::GetVirtualUpdate() const {
			return _alwaysUpdate;
		}

		void VComponent::SetVirtualUpdate(const l_t& state) {
			_alwaysUpdate = state;
		}

		IOMode VComponent::GetMode() const {
			return _mode;
		}

		void VComponent::SetMode(const IOMode& mode) {
			_mode = mode;

			for (register fsize_t i = 0; i < ARCH_MAX; i++)
				_lines[i].SetMode(_mode);
		}

		l_t VComponent::GetAlwaysFlush() const {
			return _alwaysFlush;
		}

		void VComponent::SetAlwaysFlush(const l_t& state) {
			_alwaysFlush = state;
		}

		l_t VComponent::FlushOne(const archb_type& num) {
			if (_lines[num].IsTargeted()) {
				_lines[num].SetState(_ISH(this->_virtual, num));
				return TRUE;
			}

			return FALSE;
		}

		void VComponent::Flush() {
			for (register fsize_t i = 0; i < ARCH_MAX; i++)
				this->FlushOne(i);
		}

		archb_type VComponent::Resort() const {

			archb_type ret = 0;

			for (register fsize_t i = 0; i < ARCH_MAX; i++) {
				auto current = _lines[i];
				_SETLVL(ret, i, (current.IsTargeted() ? current.GetState() : LOW_STATE));
			}

			return ret;
		}

		archb_type VComponent::GetVirtual() const {

			ResortVirtualIf();

			return _virtual;
		}

		archb_type VComponent::GetHardware() const {
			return this->Resort();
		}

		void VComponent::FillWith(const HWComponent& hwcmp,
			fsize_t localPosition,
			fsize_t hwcmpPosition,
			fsize_t cpySize
		) {

			if (localPosition >= ARCH_MAX)
				return;

			if ((localPosition + cpySize) > ARCH_MAX)
				cpySize = ARCH_MAX;
			else
				cpySize += localPosition;

			for (; localPosition < cpySize; localPosition++, hwcmpPosition++) {
				VLine* current = &_lines[localPosition];

				current->SetComponent(hwcmpPosition < ARCH_MAX ? hwcmp : HWComponent::Nothing);
				current->SetLine(hwcmpPosition);
			}

			this->ResortVirtual();
		}

		void VComponent::FillWith(const HWComponent& hwcmp,
			fsize_t localPosition,
			fsize_t hwcmpPosition
		) {

			this->FillWith(hwcmp, localPosition, hwcmpPosition, ARCH_MAX);
		}

		VLine& VComponent::operator[](const fsize_t& idx) {
			if (idx < ARCH_MAX)
				return this->_lines[idx];

			return _lines[ARCH_MAX - 1];
		}

		const VLine& VComponent::operator[](const fsize_t& idx) const {
			if (idx < ARCH_MAX)
				return this->_lines[idx];

			return _lines[ARCH_MAX - 1];
		}

		l_t VComponent::operator!() const {
			this->ResortVirtualIf();

			return !this->Resort();
		}

		archb_type VComponent::GetInverse() const {
			this->ResortVirtualIf();
			return ~(this->Resort());
		}

		VComponent::operator archb_type() const {
			this->ResortVirtualIf();
			return this->Resort();
		}

		archb_type VComponent::operator=(const archb_type& val) {

			this->ResortVirtualIf();

			_virtual = val;

			if (_alwaysFlush)
				this->Flush();

			return _virtual;
		}

		archb_type VComponent::operator+=(const archb_type& val) {

			this->ResortVirtualIf();

			_virtual += val;

			if (_alwaysFlush)
				this->Flush();

			return _virtual;
		}

		archb_type VComponent::operator-=(const archb_type& val) {

			this->ResortVirtualIf();

			_virtual -= val;

			if (_alwaysFlush)
				this->Flush();

			return _virtual;
		}

		archb_type VComponent::operator*=(const archb_type& val) {

			this->ResortVirtualIf();

			_virtual *= val;

			if (_alwaysFlush)
				this->Flush();

			return _virtual;
		}

		archb_type VComponent::operator/=(const archb_type& val) {

			this->ResortVirtualIf();

			_virtual /= val;

			if (_alwaysFlush)
				this->Flush();

			return _virtual;
		}

		archb_type VComponent::operator%=(const archb_type& val) {

			this->ResortVirtualIf();

			_virtual %= val;

			if (_alwaysFlush)
				this->Flush();

			return _virtual;
		}

		archb_type VComponent::operator|=(const archb_type& val) {

			this->ResortVirtualIf();

			_virtual |= val;

			if (_alwaysFlush)
				this->Flush();

			return _virtual;
		}

		archb_type VComponent::operator&=(const archb_type& val) {

			this->ResortVirtualIf();

			_virtual &= val;

			if (_alwaysFlush)
				this->Flush();

			return _virtual;
		}

		archb_type VComponent::operator^=(const archb_type& val) {

			this->ResortVirtualIf();

			_virtual ^= val;

			if (_alwaysFlush)
				this->Flush();

			return _virtual;
		}

		archb_type VComponent::operator<<=(const archb_type& val) {

			this->ResortVirtualIf();

			_virtual <<= val;

			if (_alwaysFlush)
				this->Flush();

			return _virtual;
		}

		archb_type VComponent::operator>>=(const archb_type& val) {

			this->ResortVirtualIf();

			_virtual >>= val;

			if (_alwaysFlush)
				this->Flush();

			return _virtual;
		}


		VComponent VComponent::FromHWComponentPart(
			const HWComponent& hwcmp,
			const fsize_t& start_idx,
			const l_t& alwaysFlush,
			const l_t& alwaysUpdate
		) {
			VComponent ret(alwaysFlush, alwaysUpdate);
			ret.FillWith(hwcmp, 0, start_idx);
			ret.ResortVirtual();

			return ret;
		}

		VComponent VComponent::FromHWComponent(
			const HWComponent& hwcmp,
			const l_t& alwaysFlush,
			const l_t& alwaysUpdate) {
			VComponent ret(alwaysFlush);

			return VComponent::FromHWComponentPart(hwcmp, 0, alwaysFlush, alwaysUpdate);
		}
	}
}