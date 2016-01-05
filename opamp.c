#include <stdint.h>
#include <stdbool.h>

#include <libopencm3/cm3/common.h>
#include <libopencm3/stm32/memorymap.h>

#include "opamp.h"
#include "clock.h"


/** Lock the opamp config. No way to unlock until restart. */
void opamp_lock(uint32_t opamp)
{
	OPAMP_CSR(opamp) |= OPAMP_CSR_LOCK;
}


/**
 * Enable OPAMP
 * Must also enable RCC_SYSCFG_COMP in RCC
 */
void opamp_enable(uint32_t opamp)
{
	OPAMP_CSR(opamp) |= OPAMP_CSR_OPAMPEN;
}


/** Disable OPAMP */
void opamp_disable(uint32_t opamp)
{
	OPAMP_CSR(opamp) &= ~OPAMP_CSR_OPAMPEN;
}


/** Send reference to ADC input channel */
void opamp_ref_to_adc(uint32_t opamp, bool yes)
{
	if (yes) {
		OPAMP_CSR(opamp) |= OPAMP_CSR_TSTREF;
	} else {
		OPAMP_CSR(opamp) &= ~OPAMP_CSR_TSTREF;
	}
}


/** Send reference to non-inverting input */
void opamp_ref_to_vp(uint32_t opamp, bool yes)
{
	if (yes) {
		OPAMP_CSR(opamp) |= OPAMP_CSR_FORCE_VP;
	} else {
		OPAMP_CSR(opamp) &= ~OPAMP_CSR_FORCE_VP;
	}
}


/** Set PGA gain. To use PGA, set VM to PGA mode. */
void opamp_pga_set_gain(uint32_t opamp, enum OPAMP_PGA_GAIN gain)
{
	OPAMP_CSR(opamp) &= ~OPAMP_CSR_PGA_GAIN_MSK;
	OPAMP_CSR(opamp) |= gain;
}


/** Set PGA midpoint routing */
void opamp_pga_set_midpoint(uint32_t opamp, enum OPAMP_PGA_MIDPOINT midpoint)
{
	OPAMP_CSR(opamp) &= ~OPAMP_CSR_PGA_MIDPOINT_MSK;
	OPAMP_CSR(opamp) |= midpoint;
}


/** Connect VM pin (can be used to select follower and PGA mode) */
void opamp_vm_select(uint32_t opamp, enum OPAMP_VM_SEL vm)
{
	OPAMP_CSR(opamp) &= ~OPAMP_CSR_VM_SEL_MSK;
	OPAMP_CSR(opamp) |= vm;
}


/** Connect the VP pin */
void opamp_vp_select(uint32_t opamp, enum OPAMP_VP_SEL vp)
{
	OPAMP_CSR(opamp) &= ~OPAMP_CSR_VP_SEL_MSK;
	OPAMP_CSR(opamp) |= vp;
}


/**
 * Set opamp to PGA mode (shortcut function)
 * Midpoint can be connected to VM pins with opamp_pga_set_midpoint()
 */
void opamp_mode_pga(uint32_t opamp)
{
	opamp_vm_select(opamp, OPAMP_VM_SEL_PGA);
}


/**
 * Set opamp to follower mode  (shortcut function).
 * VM is not connected to GPIO in this mode.
 */
void opamp_mode_follower(uint32_t opamp)
{
	opamp_vm_select(opamp, OPAMP1_VM_SEL_FLW);
}


/** Select reference value to use */
void opamp_ref_select(uint32_t opamp, enum OPAMP_CALSEL ref)
{
	OPAMP_CSR(opamp) &= ~OPAMP_CSR_CALSEL_MSK;
	OPAMP_CSR(opamp) |= ref;
}


/** Set opamp trim offset N */
static void set_trimoffset_n(uint32_t opamp, uint8_t offset)
{
	OPAMP_CSR(opamp) &= ~OPAMP_CSR_TRIMOFFSETN_MSK;
	OPAMP_CSR(opamp) |= ((offset & 0x1F) << OPAMP_CSR_TRIMOFFSETN_SHIFT);
}


/** Set opamp trim offset P */
static void set_trimoffset_p(uint32_t opamp, uint8_t offset)
{
	OPAMP_CSR(opamp) &= ~OPAMP_CSR_TRIMOFFSETP_MSK;
	OPAMP_CSR(opamp) |= ((offset & 0x1F) << OPAMP_CSR_TRIMOFFSETP_SHIFT);
}


/** Enter calibration mode */
static void calib_start(uint32_t opamp)
{
	OPAMP_CSR(opamp) |= OPAMP_CSR_USER_TRIM; // enable user trimming
	OPAMP_CSR(opamp) |= OPAMP_CSR_CALON; // enable calibration mode (connect VP,VM to ref)
}


/** Leave calibration mode, disable user calibration constants */
static void calib_abort(uint32_t opamp)
{
	OPAMP_CSR(opamp) &= ~OPAMP_CSR_USER_TRIM; // disable user trim
	OPAMP_CSR(opamp) &= ~OPAMP_CSR_CALON; // leave calibration mode
}


/** Leave calibration mode */
static void calib_end(uint32_t opamp)
{
	OPAMP_CSR(opamp) &= ~OPAMP_CSR_CALON; // leave calibration mode
}


/** Calibrate N-MOS block. Returns false failure */
static bool calib_do_nmos(uint32_t opamp)
{
	uint8_t trimoffset;
	opamp_ref_select(opamp, OPAMP_CALSEL_90);

	OPAMP_CSR(opamp) &= ~OPAMP_CSR_TRIMOFFSETN_MSK;

	for (trimoffset = 0; trimoffset <= 0x1F; trimoffset++) {
		set_trimoffset_n(opamp, trimoffset);
		delay_ms(2);

		if ((OPAMP_CSR(opamp) & OPAMP_CSR_OUTCAL) == 0) {
			return true; // Calibration complete.
		}
	}

	return false;
}



/** Calibrate P-MOS block. Returns false failure */
static bool calib_do_pmos(uint32_t opamp)
{
	uint8_t trimoffset;
	opamp_ref_select(opamp, OPAMP_CALSEL_10);

	OPAMP_CSR(opamp) &= ~OPAMP_CSR_TRIMOFFSETP_MSK;

	for (trimoffset = 0; trimoffset <= 0x1F; trimoffset++) {
		set_trimoffset_p(opamp, trimoffset);
		delay_ms(2);

		if ((OPAMP_CSR(opamp) & OPAMP_CSR_OUTCAL) == 0) {
			return true; // Calibration complete.
		}
	}

	return false;
}


/** Calibrate an OpAmp */
bool opamp_calibrate(uint32_t opamp)
{
	opamp_enable(opamp); // make sure it's enabled
	calib_start(opamp);

	if (!calib_do_nmos(opamp)) {
		calib_abort(opamp);
		return false;
	}

	if (!calib_do_pmos(opamp)) {
		calib_abort(opamp);
		return false;
	}

	calib_end(opamp);
	return true;
}
