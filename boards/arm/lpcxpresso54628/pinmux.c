/*
 * Copyright (c) 2017,  NXP
 * SPDX-License-Identifier: Apache-2.0
 */

#include <init.h>
#include <drivers/pinmux.h>
#include <fsl_common.h>
#include <fsl_iocon.h>
#include <soc.h>

#define IOCON_PIO_FUNC6 0x06u 

static int lpcxpresso_54628_pinmux_init(const struct device *dev)
{
	ARG_UNUSED(dev);

#if DT_NODE_HAS_STATUS(DT_NODELABEL(pio0), okay)
	__unused const struct device *port0 =
		DEVICE_DT_GET(DT_NODELABEL(pio0));
	__ASSERT_NO_MSG(device_is_ready(port0));
#endif

#if DT_NODE_HAS_STATUS(DT_NODELABEL(pio1), okay)
	__unused const struct device *port1 =
		DEVICE_DT_GET(DT_NODELABEL(pio1));
	__ASSERT_NO_MSG(device_is_ready(port1));
#endif

#if DT_NODE_HAS_STATUS(DT_NODELABEL(pio2), okay)
	__unused const struct device *port2 =
		DEVICE_DT_GET(DT_NODELABEL(pio2));
	__ASSERT_NO_MSG(device_is_ready(port2));
#endif

#if DT_NODE_HAS_STATUS(DT_NODELABEL(pio3), okay)
	__unused const struct device *port3 =
		DEVICE_DT_GET(DT_NODELABEL(pio3));
	__ASSERT_NO_MSG(device_is_ready(port3));
#endif

    const uint32_t port0_pin10_config = (/* Pin is configured as SWO */
                                         IOCON_PIO_FUNC6 |
                                         /* No addition pin function */
                                         IOCON_PIO_MODE_INACT |
                                         /* Input function is not inverted */
                                         IOCON_PIO_INV_DI |
                                         /* Enables digital function */
                                         IOCON_PIO_DIGITAL_EN |
                                         /* Input filter disabled */
                                         IOCON_PIO_INPFILT_OFF |
                                         /* Open drain is disabled */
                                         IOCON_PIO_OPENDRAIN_DI);
    /* PORT0 PIN10 (coords: P2) is configured as SWO */
    pinmux_pin_set(port0, 10, port0_pin10_config);

#if DT_NODE_HAS_COMPAT_STATUS(DT_NODELABEL(flexcomm0), nxp_lpc_usart, okay) && CONFIG_SERIAL
	/* USART0 RX,  TX */
	uint32_t port0_pin29_config = (
			IOCON_PIO_FUNC1 |
			IOCON_PIO_MODE_INACT |
			IOCON_PIO_INV_DI |
			IOCON_PIO_DIGITAL_EN |
			IOCON_PIO_INPFILT_OFF |
			IOCON_PIO_SLEW_STANDARD |
			IOCON_PIO_OPENDRAIN_DI
			);

	uint32_t port0_pin30_config = (
			IOCON_PIO_FUNC1 |
			IOCON_PIO_MODE_INACT |
			IOCON_PIO_INV_DI |
			IOCON_PIO_DIGITAL_EN |
			IOCON_PIO_INPFILT_OFF |
			IOCON_PIO_SLEW_STANDARD |
			IOCON_PIO_OPENDRAIN_DI
			);

	pinmux_pin_set(port0, 29, port0_pin29_config);
	pinmux_pin_set(port0, 30, port0_pin30_config);

#endif

#if DT_NODE_HAS_COMPAT_STATUS(DT_NODELABEL(flexcomm2), nxp_lpc_i2c, okay) && CONFIG_I2C
	uint32_t port3_pin23_config = (
			IOCON_PIO_FUNC1 |
			IOCON_PIO_I2CFILTER_DI |
			IOCON_PIO_I2CDRIVE_LOW |
			IOCON_PIO_INPFILT_OFF |
			IOCON_PIO_DIGITAL_EN |
			IOCON_PIO_INV_DI |
			IOCON_PIO_I2CSLEW_I2C
			);

	uint32_t port3_pin24_config = (
			IOCON_PIO_FUNC1 |
			IOCON_PIO_I2CFILTER_DI |
			IOCON_PIO_I2CDRIVE_LOW |
			IOCON_PIO_INPFILT_OFF |
			IOCON_PIO_DIGITAL_EN |
			IOCON_PIO_INV_DI |
			IOCON_PIO_I2CSLEW_I2C
			);


	pinmux_pin_set(port3, 23, port3_pin23_config);
	pinmux_pin_set(port3, 24, port3_pin24_config);
#endif

	return 0;
}

SYS_INIT(lpcxpresso_54628_pinmux_init,  PRE_KERNEL_1,
	 CONFIG_PINMUX_INIT_PRIORITY);
