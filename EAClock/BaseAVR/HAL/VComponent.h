#ifndef __VCOMPONENT_H__
#define __VCOMPONENT_H__
#include "../globaldef.h"
#include "../macrodef.h"
#include "VLine.h"


namespace BaseAVR {
	namespace HAL {

		class VComponent {

			private:
			VLine _lines[ARCH_MAX];
			IOMode _mode;

			mutable archb_type _virtual;
			l_t _alwaysFlush;
			l_t _alwaysUpdate;


			archb_type Resort() const;
			void ResortVirtual() const;
			void ResortVirtualIf() const;

			public:

			VComponent(
			VLine* const lines,
			const fsize_t& lsize,
			const IOMode& mode,
			const l_t& alwaysFlush,
			const l_t& alwaysUpdate);

			VComponent(const IOMode& lineMode, const l_t& alwaysFlush, const l_t& alwaysUpdate = FALSE);
			VComponent(const l_t& alwaysFlush, const l_t& alwaysUpdate = FALSE);
			VComponent();

			archb_type GetVirtual() const;
			archb_type GetHardware() const;

			void RefreshVirtual();
			//void SetStrictVirtualUpdate(const l_t&);


			virtual void Flush();
			virtual l_t FlushOne(const archb_type& num);

			virtual void FillWith(
			const HWComponent& hwcmp,
			fsize_t localPosition,
			fsize_t hwcmpPosition,
			fsize_t cpySize);

			virtual void FillWith(
			const HWComponent& hwcmp,
			fsize_t localPosition,
			fsize_t hwcmpPosition
			);

			virtual archb_type operator=(const archb_type&);
			virtual archb_type operator+=(const archb_type&);
			virtual archb_type operator-=(const archb_type&);
			virtual archb_type operator*=(const archb_type&);
			virtual archb_type operator/=(const archb_type&);
			virtual archb_type operator%=(const archb_type&);

			virtual archb_type operator|=(const archb_type&);
			virtual archb_type operator&=(const archb_type&);
			virtual archb_type operator^=(const archb_type&);
			virtual archb_type operator<<=(const archb_type&);
			virtual archb_type operator>>=(const archb_type&);

			virtual operator archb_type() const;
			virtual l_t operator!() const;
			virtual archb_type GetInverse() const;

			const VLine& operator[](const fsize_t&) const;
			VLine& operator[](const fsize_t&);

			IOMode GetMode() const;
			void SetMode(const IOMode&);

			l_t GetVirtualUpdate() const;
			void SetVirtualUpdate(const l_t&);

			l_t GetAlwaysFlush() const;
			void SetAlwaysFlush(const l_t&);

			static VComponent FromHWComponentPart(
			const HWComponent&,
			const fsize_t& start_idx,
			const l_t& alwaysFlush = FALSE,
			const l_t& alwaysUpdate = FALSE);

			static VComponent FromHWComponent(const HWComponent&,
			const l_t& alwaysFlush = FALSE,
			const l_t& alwaysUpdate = FALSE);
		};
	}
}
#endif //__VCOMPONENT_H__
