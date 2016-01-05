#pragma once

#include <libopencm3/cm3/common.h>
#include <libopencm3/stm32/f3/memorymap.h>


// Opamp base address (just one register)
#define OPAMP1_BASE   OPAMP_BASE + 0x38
#define OPAMP2_BASE   OPAMP_BASE + 0x3C
#define OPAMP3_BASE   OPAMP_BASE + 0x40
#define OPAMP4_BASE   OPAMP_BASE + 0x44

// opamp constants
#define OPAMP1   OPAMP1_BASE
#define OPAMP2   OPAMP2_BASE
#define OPAMP3   OPAMP3_BASE
#define OPAMP4   OPAMP4_BASE

#define OPAMP_CSR(opamp_base) MMIO32(opamp_base + 0)


#define OPAMP_CSR_LOCK    (1 << 31)


#define OPAMP_CSR_OUTCAL  (1 << 30)


#define OPAMP_CSR_TSTREF  (1 << 29)


// trim registers
#define OPAMP_CSR_TRIMOFFSETN_SHIFT 24
#define OPAMP_CSR_TRIMOFFSETP_SHIFT 19

#define OPAMP_CSR_TRIMOFFSETN_MSK (0x1F << OPAMP_CSR_TRIMOFFSETN_SHIFT)
#define OPAMP_CSR_TRIMOFFSETP_MSK (0x1F << OPAMP_CSR_TRIMOFFSETP_SHIFT)


// user trim
#define OPAMP_CSR_USER_TRIM  (1 << 18)



// PGA gain
#define OPAMP_CSR_PGA_GAIN_SHIFT 14
#define OPAMP_CSR_PGA_GAIN_MSK  (0b0011 << OPAMP_CSR_PGA_GAIN_SHIFT)
#define OPAMP_CSR_PGA_MIDPOINT_MSK  (0b1100 << OPAMP_CSR_PGA_GAIN_SHIFT)


// lower two bits
enum OPAMP_PGA_GAIN {
	OPAMP_PGA_GAIN_2 = (0 << OPAMP_CSR_PGA_GAIN_SHIFT),
	OPAMP_PGA_GAIN_4 = (1 << OPAMP_CSR_PGA_GAIN_SHIFT),
	OPAMP_PGA_GAIN_8 = (2 << OPAMP_CSR_PGA_GAIN_SHIFT),
	OPAMP_PGA_GAIN_16 = (3 << OPAMP_CSR_PGA_GAIN_SHIFT)
};

// Masks that can be used to adjust PGA value
enum OPAMP_PGA_MIDPOINT {
	OPAMP_PGA_MIDPOINT_NONE = 0,
	OPAMP_PGA_MIDPOINT_VM0 = (0b1000 << OPAMP_CSR_PGA_GAIN_SHIFT),
	OPAMP_PGA_MIDPOINT_VM1 = (0b1100 << OPAMP_CSR_PGA_GAIN_SHIFT)
};


// Calibration select
#define OPAMP_CSR_CALSEL_SHIFT 12
#define OPAMP_CSR_CALSEL_MSK  (0b11 << OPAMP_CSR_CALSEL_SHIFT)

enum OPAMP_CALSEL {
	OPAMP_CALSEL_3P3 = (0 << OPAMP_CSR_CALSEL_SHIFT),
	OPAMP_CALSEL_10 = (1 << OPAMP_CSR_CALSEL_SHIFT),
	OPAMP_CALSEL_50 = (2 << OPAMP_CSR_CALSEL_SHIFT),
	OPAMP_CALSEL_90 = (3 << OPAMP_CSR_CALSEL_SHIFT),
};



// Cal On
#define OPAMP_CSR_CALON  (1 << 11)



// Non-inverting input, secondary
#define OPAMP_CSR_VPS_SEL_SHIFT 9
#define OPAMP_CSR_VPS_SEL_MSK  (0b11 << OPAMP_CSR_VPS_SEL_SHIFT)

enum OPAMP_VPS_SEL {
	OPAMP_VPS_SEL_VP0 = (0 << OPAMP_CSR_VPS_SEL_SHIFT),
	OPAMP_VPS_SEL_VP1 = (1 << OPAMP_CSR_VPS_SEL_SHIFT),
	OPAMP_VPS_SEL_VP2 = (2 << OPAMP_CSR_VPS_SEL_SHIFT),
	OPAMP_VPS_SEL_VP3 = (3 << OPAMP_CSR_VPS_SEL_SHIFT),

	// The following are just aliases for convenience, mapped for F303. 
	// probably should not be included in the library, as they're too specific.
	// same for the following enums.
	OPAMP1_VPS_SEL_PA7 = OPAMP_VPS_SEL_VP0,
	OPAMP1_VPS_SEL_PA5 = OPAMP_VPS_SEL_VP1,
	OPAMP1_VPS_SEL_PA3 = OPAMP_VPS_SEL_VP2,
	OPAMP1_VPS_SEL_PA1 = OPAMP_VPS_SEL_VP3,

	OPAMP2_VPS_SEL_PD14 = OPAMP_VPS_SEL_VP0,
	OPAMP2_VPS_SEL_PB14 = OPAMP_VPS_SEL_VP1,
	OPAMP2_VPS_SEL_PB0 = OPAMP_VPS_SEL_VP2,
	OPAMP2_VPS_SEL_PA7 = OPAMP_VPS_SEL_VP3,

	OPAMP3_VPS_SEL_PB13 = OPAMP_VPS_SEL_VP0,
	OPAMP3_VPS_SEL_PA5 = OPAMP_VPS_SEL_VP1,
	OPAMP3_VPS_SEL_PA1 = OPAMP_VPS_SEL_VP2,
	OPAMP3_VPS_SEL_PB0 = OPAMP_VPS_SEL_VP3,

	OPAMP4_VPS_SEL_PD11 = OPAMP_VPS_SEL_VP0,
	OPAMP4_VPS_SEL_PB11 = OPAMP_VPS_SEL_VP1,
	OPAMP4_VPS_SEL_PA4 = OPAMP_VPS_SEL_VP2,
	OPAMP4_VPS_SEL_PB13 = OPAMP_VPS_SEL_VP3
};



// Inverting input, secondary
#define OPAMP_CSR_VMS_SEL_SHIFT 8
#define OPAMP_CSR_VMS_SEL_MSK  (1 << OPAMP_CSR_VMS_SEL_SHIFT)

enum OPAMP_VMS_SEL {
	OPAMP_VMS_SEL_VM0 = (0 << OPAMP_CSR_VMS_SEL_SHIFT),
	OPAMP_VMS_SEL_VM1 = (1 << OPAMP_CSR_VMS_SEL_SHIFT),

	OPAMP1_VMS_SEL_PC5 = OPAMP_VMS_SEL_VM0,
	OPAMP1_VMS_SEL_PA3 = OPAMP_VMS_SEL_VM1,

	OPAMP2_VMS_SEL_PC5 = OPAMP_VMS_SEL_VM0,
	OPAMP2_VMS_SEL_PA5 = OPAMP_VMS_SEL_VM1,

	OPAMP3_VMS_SEL_PB10 = OPAMP_VMS_SEL_VM0,
	OPAMP3_VMS_SEL_PB2 = OPAMP_VMS_SEL_VM1,

	OPAMP4_VMS_SEL_PB10 = OPAMP_VMS_SEL_VM0,
	OPAMP4_VMS_SEL_PD8 = OPAMP_VMS_SEL_VM1,
};



// Cal On
#define OPAMP_CSR_TCM_EN  (1 << 7)



// Inverting input, primary
#define OPAMP_CSR_VM_SEL_SHIFT 5
#define OPAMP_CSR_VM_SEL_MSK  (0b11 << OPAMP_CSR_VM_SEL_SHIFT)

enum OPAMP_VM_SEL {
	OPAMP_VM_SEL_VM0 = (0 << OPAMP_CSR_VM_SEL_SHIFT),
	OPAMP_VM_SEL_VM1 = (1 << OPAMP_CSR_VM_SEL_SHIFT),
	OPAMP_VM_SEL_PGA = (2 << OPAMP_CSR_VM_SEL_SHIFT),
	OPAMP_VM_SEL_FLW = (3 << OPAMP_CSR_VM_SEL_SHIFT),

	OPAMP1_VM_SEL_PC5 = OPAMP_VM_SEL_VM0,
	OPAMP1_VM_SEL_PA3 = OPAMP_VM_SEL_VM1,
	OPAMP1_VM_SEL_PGA = OPAMP_VM_SEL_PGA,
	OPAMP1_VM_SEL_FLW = OPAMP_VM_SEL_FLW,

	OPAMP2_VM_SEL_PC5 = OPAMP_VM_SEL_VM0,
	OPAMP2_VM_SEL_PA5 = OPAMP_VM_SEL_VM1,
	OPAMP2_VM_SEL_PGA = OPAMP_VM_SEL_PGA,
	OPAMP2_VM_SEL_FLW = OPAMP_VM_SEL_FLW,

	OPAMP3_VM_SEL_PB10 = OPAMP_VM_SEL_VM0,
	OPAMP3_VM_SEL_PB2 = OPAMP_VM_SEL_VM1,
	OPAMP3_VM_SEL_PGA = OPAMP_VM_SEL_PGA,
	OPAMP3_VM_SEL_FLW = OPAMP_VM_SEL_FLW,

	OPAMP4_VM_SEL_PB10 = OPAMP_VM_SEL_VM0,
	OPAMP4_VM_SEL_PD8 = OPAMP_VM_SEL_VM1,
	OPAMP4_VM_SEL_PGA = OPAMP_VM_SEL_PGA,
	OPAMP4_VM_SEL_FLW = OPAMP_VM_SEL_FLW,
};



// Non-inverting input, primary
#define OPAMP_CSR_VP_SEL_SHIFT 2
#define OPAMP_CSR_VP_SEL_MSK  (0b11 << OPAMP_CSR_VP_SEL_SHIFT)

enum OPAMP_VP_SEL {
	OPAMP_VP_SEL_VP0 = (0 << OPAMP_CSR_VP_SEL_SHIFT),
	OPAMP_VP_SEL_VP1 = (1 << OPAMP_CSR_VP_SEL_SHIFT),
	OPAMP_VP_SEL_VP2 = (2 << OPAMP_CSR_VP_SEL_SHIFT),
	OPAMP_VP_SEL_VP3 = (3 << OPAMP_CSR_VP_SEL_SHIFT),

	OPAMP1_VP_SEL_PA7 = OPAMP_VP_SEL_VP0,
	OPAMP1_VP_SEL_PA5 = OPAMP_VP_SEL_VP1,
	OPAMP1_VP_SEL_PA3 = OPAMP_VP_SEL_VP2,
	OPAMP1_VP_SEL_PA1 = OPAMP_VP_SEL_VP3,

	OPAMP2_VP_SEL_PD14 = OPAMP_VP_SEL_VP0,
	OPAMP2_VP_SEL_PB14 = OPAMP_VP_SEL_VP1,
	OPAMP2_VP_SEL_PB0 = OPAMP_VP_SEL_VP2,
	OPAMP2_VP_SEL_PA7 = OPAMP_VP_SEL_VP3,

	OPAMP3_VP_SEL_PB13 = OPAMP_VP_SEL_VP0,
	OPAMP3_VP_SEL_PA5 = OPAMP_VP_SEL_VP1,
	OPAMP3_VP_SEL_PA1 = OPAMP_VP_SEL_VP2,
	OPAMP3_VP_SEL_PB0 = OPAMP_VP_SEL_VP3,

	OPAMP4_VP_SEL_PD11 = OPAMP_VP_SEL_VP0,
	OPAMP4_VP_SEL_PB11 = OPAMP_VP_SEL_VP1,
	OPAMP4_VP_SEL_PA4 = OPAMP_VP_SEL_VP2,
	OPAMP4_VP_SEL_PB13 = OPAMP_VP_SEL_VP3
};



// Forve VP - VP connected to calibration reference
#define OPAMP_CSR_FORCE_VP  (1 << 1)



// Enable opamp
#define OPAMP_CSR_OPAMPEN  (1 << 0)


// -------------- Functions ----------------

/** Lock the opamp config. No way to unlock until restart. */
void opamp_lock(uint32_t opamp);


/**
 * Enable OPAMP
 * Must also enable RCC_SYSCFG_COMP in RCC
 */
void opamp_enable(uint32_t opamp);


/** Disable OPAMP */
void opamp_disable(uint32_t opamp);


/** Send reference to ADC input channel */
void opamp_ref_to_adc(uint32_t opamp, bool yes);


/** Send reference to non-inverting input */
void opamp_ref_to_vp(uint32_t opamp, bool yes);


/**
 * Set PGA gain.
 * To use PGA, call opamp_mode_pga() first.
 */
void opamp_pga_set_gain(uint32_t opamp, enum OPAMP_PGA_GAIN gain);


/** Set PGA midpoint routing */
void opamp_pga_set_midpoint(uint32_t opamp, enum OPAMP_PGA_MIDPOINT midpoint);


/** Connect VM pin (can be used to select follower and PGA mode) */
void opamp_vm_select(uint32_t opamp, enum OPAMP_VM_SEL vm);


/** Connect the VP pin */
void opamp_vp_select(uint32_t opamp, enum OPAMP_VP_SEL vp);


/**
 * Set opamp to PGA mode (shortcut function)
 * Midpoint can be connected to VM pins with opamp_pga_set_midpoint()
 */
void opamp_mode_pga(uint32_t opamp);


/**
 * Set opamp to follower mode  (shortcut function).
 * VM is not connected to GPIO in this mode.
 */
void opamp_mode_follower(uint32_t opamp);


/** Select reference value to use */
void opamp_ref_select(uint32_t opamp, enum OPAMP_CALSEL ref);


/** Calibrate an OpAmp */
bool opamp_calibrate(uint32_t opamp);
