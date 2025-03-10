/*
 * Copyright (c) 2021 Piotr Mienkowski
 * SPDX-License-Identifier: Apache-2.0
 */

/** @file
 * @brief DAC driver for Atmel SAM MCU family.
 *
 * Remarks:
 * Only SAME70, SAMV71 series devices are currently supported. Please submit a
 * patch.
 */

#define DT_DRV_COMPAT atmel_sam_dac

#include <errno.h>
#include <sys/__assert.h>
#include <soc.h>
#include <device.h>
#include <drivers/dac.h>

#include <logging/log.h>
LOG_MODULE_REGISTER(dac_sam, CONFIG_DAC_LOG_LEVEL);

BUILD_ASSERT(IS_ENABLED(CONFIG_SOC_SERIES_SAME70) ||
	     IS_ENABLED(CONFIG_SOC_SERIES_SAMV71),
	     "Only SAME70, SAMV71 series devices are currently supported.");

#define DAC_CHANNEL_NO  2

/* Device constant configuration parameters */
struct dac_sam_dev_cfg {
	Dacc *regs;
	void (*irq_config)(void);
	uint8_t irq_id;
	uint8_t periph_id;
	uint8_t prescaler;
};

struct dac_channel {
	struct k_sem sem;
};

/* Device run time data */
struct dac_sam_dev_data {
	struct dac_channel dac_channels[DAC_CHANNEL_NO];
};

static void dac_sam_isr(void *arg)
{
	const struct device *dev = (const struct device *)arg;
	const struct dac_sam_dev_cfg *const dev_cfg = dev->config;
	struct dac_sam_dev_data *const dev_data = dev->data;
	Dacc *const dac = dev_cfg->regs;
	uint32_t int_stat;

	/* Retrieve interrupt status */
	int_stat = dac->DACC_ISR & dac->DACC_IMR;

	if ((int_stat & DACC_ISR_TXRDY0) != 0) {
		/* Disable Transmit Ready Interrupt */
		dac->DACC_IDR = DACC_IDR_TXRDY0;
		k_sem_give(&dev_data->dac_channels[0].sem);
	}
	if ((int_stat & DACC_ISR_TXRDY1) != 0) {
		/* Disable Transmit Ready Interrupt */
		dac->DACC_IDR = DACC_IDR_TXRDY1;
		k_sem_give(&dev_data->dac_channels[1].sem);
	}
}

static int dac_sam_channel_setup(const struct device *dev,
				 const struct dac_channel_cfg *channel_cfg)
{
	const struct dac_sam_dev_cfg *const dev_cfg = dev->config;
	Dacc *const dac = dev_cfg->regs;

	if (channel_cfg->channel_id >= DAC_CHANNEL_NO) {
		return -EINVAL;
	}
	if (channel_cfg->resolution != 12) {
		return -ENOTSUP;
	}

	/* Enable Channel */
	dac->DACC_CHER = DACC_CHER_CH0 << channel_cfg->channel_id;

	return 0;
}

static int dac_sam_write_value(const struct device *dev, uint8_t channel,
			       uint32_t value)
{
	struct dac_sam_dev_data *const dev_data = dev->data;
	const struct dac_sam_dev_cfg *const dev_cfg = dev->config;
	Dacc *const dac = dev_cfg->regs;

	if (channel >= DAC_CHANNEL_NO) {
		return -EINVAL;
	}

	if (dac->DACC_IMR & (DACC_IMR_TXRDY0 << channel)) {
		/* Attempting to send data on channel that's already in use */
		return -EINVAL;
	}

	k_sem_take(&dev_data->dac_channels[channel].sem, K_FOREVER);

	/* Trigger conversion */
	dac->DACC_CDR[channel] = DACC_CDR_DATA0(value);

	/* Enable Transmit Ready Interrupt */
	dac->DACC_IER = DACC_IER_TXRDY0 << channel;

	return 0;
}

static int dac_sam_init(const struct device *dev)
{
	const struct dac_sam_dev_cfg *const dev_cfg = dev->config;
	struct dac_sam_dev_data *const dev_data = dev->data;
	Dacc *const dac = dev_cfg->regs;

	/* Configure interrupts */
	dev_cfg->irq_config();

	/* Initialize semaphores */
	for (int i = 0; i < ARRAY_SIZE(dev_data->dac_channels); i++) {
		k_sem_init(&dev_data->dac_channels[i].sem, 1, 1);
	}

	/* Enable DAC clock in PMC */
	soc_pmc_peripheral_enable(dev_cfg->periph_id);

	/* Set Mode Register */
	dac->DACC_MR = DACC_MR_PRESCALER(dev_cfg->prescaler);

	/* Enable module's IRQ */
	irq_enable(dev_cfg->irq_id);

	LOG_INF("Device %s initialized", dev->name);

	return 0;
}

static const struct dac_driver_api dac_sam_driver_api = {
	.channel_setup = dac_sam_channel_setup,
	.write_value = dac_sam_write_value,
};

/* DACC */

static void dacc_irq_config(void)
{
	IRQ_CONNECT(DT_INST_IRQN(0), DT_INST_IRQ(0, priority), dac_sam_isr,
		    DEVICE_DT_INST_GET(0), 0);
}

static const struct dac_sam_dev_cfg dacc_sam_config = {
	.regs = (Dacc *)DT_INST_REG_ADDR(0),
	.irq_id = DT_INST_IRQN(0),
	.irq_config = dacc_irq_config,
	.periph_id = DT_INST_PROP(0, peripheral_id),
	.prescaler = DT_INST_PROP(0, prescaler),
};

static struct dac_sam_dev_data dacc_sam_data;

DEVICE_DT_INST_DEFINE(0, dac_sam_init, NULL, &dacc_sam_data, &dacc_sam_config,
		      POST_KERNEL, CONFIG_DAC_INIT_PRIORITY,
		      &dac_sam_driver_api);
