/*
 * Copyright (c) 2017, NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @file
 * @brief Board configuration macros for the nxp_lpc54xxx platform
 *
 * This header file is used to specify and describe board-level aspects for the
 * 'nxp_lpc54xxx' platform.
 */

#ifndef _SOC__H_
#define _SOC__H_

#ifndef _ASMLANGUAGE
#include <sys/util.h>

#endif /* !_ASMLANGUAGE */


/* User Manual Pages
   5411x: 150
   546xx: 190 */

/* Common Defs */
#define IOCON_PIO_FUNC0                0x00u
#define IOCON_PIO_FUNC1                0x01u
#define IOCON_PIO_FUNC2                0x02u
#define IOCON_PIO_FUNC3                0x03u
#define IOCON_PIO_FUNC4                0x04u
#define IOCON_PIO_FUNC5                0x05u
#define IOCON_PIO_FUNC6                0x06u
#define IOCON_PIO_FUNC7                0x07u

#define IOCON_PIO_MODE_INACT           0x00u
#define IOCON_PIO_INV_DI               0x00u
#define IOCON_PIO_OPENDRAIN_DI         0x00u
#define IOCON_PIO_SLEW_STANDARD        0x00u
#define IOCON_PIO_INPFILT_ON           0x00u

#define IOCON_PIO_I2CDRIVE_LOW         0x00u
#define IOCON_PIO_I2CFILTER_EN         0x00u
#define IOCON_PIO_I2CSLEW_I2C          0x00u
/* End Common Defs */


#if defined(CONFIG_SOC_LPC54628)
#define IOCON_PIO_MODE_PULLDOWN        0x10u
#define IOCON_PIO_MODE_PULLUP          0x20u
#define IOCON_PIO_INV_EN               0x80u
#define IOCON_PIO_DIGITAL_EN           0x100u
#define IOCON_PIO_INPFILT_OFF          0x200u
#define IOCON_PIO_SLEW_FAST            0x400u
#define IOCON_PIO_OPENDRAIN_EN         0x800u

#define IOCON_PIO_I2CSLEW_GPIO         0x40u
#define IOCON_PIO_I2CDRIVE_HIGH        0x400u
#define IOCON_PIO_I2CFILTER_DI         0x800u

#endif /* SOC_LPC54628 */ 

#if defined(CONFIG_SOC_LPC54114_M4) || defined(CONFIG_SOC_LPC54114_M0)
#define IOCON_PIO_MODE_PULLDOWN        0x08u
#define IOCON_PIO_MODE_PULLUP          0x10u
#define IOCON_PIO_INV_EN               0x40u
#define IOCON_PIO_DIGITAL_EN           0x80u
#define IOCON_PIO_INPFILT_OFF          0x100u
#define IOCON_PIO_SLEW_FAST            0x200u
#define IOCON_PIO_OPENDRAIN_EN         0x400u

#define IOCON_PIO_I2CSLEW_GPIO         0x20u
#define IOCON_PIO_I2CDRIVE_HIGH        0x200u
#define IOCON_PIO_I2CFILTER_DI         0x400u

#endif /* SOC_LPC54114_M4_M0 */ 

#ifndef _ASMLANGUAGE
#include <fsl_common.h>


#endif /* !_ASMLANGUAGE */

#endif /* _SOC__H_ */
