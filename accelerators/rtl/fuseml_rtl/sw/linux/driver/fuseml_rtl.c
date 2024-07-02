// Copyright (c) 2011-2024 Columbia University, System Level Design Group
// SPDX-License-Identifier: Apache-2.0
#include <linux/of_device.h>
#include <linux/mm.h>

#include <asm/io.h>

#include <esp_accelerator.h>
#include <esp.h>

#include "fuseml_rtl.h"

#define DRV_NAME	"fuseml_rtl"

/* <<--regs-->> */
#define FUSEML_REG5_REG 0x64
#define FUSEML_REG4_REG 0x60
#define FUSEML_REG7_REG 0x5c
#define FUSEML_REG6_REG 0x58
#define FUSEML_REG1_REG 0x54
#define FUSEML_REG3_REG 0x50
#define FUSEML_REG2_REG 0x4c
#define FUSEML_REG9_REG 0x48
#define FUSEML_REG8_REG 0x44
#define FUSEML_REG10_REG 0x40

struct fuseml_rtl_device {
	struct esp_device esp;
};

static struct esp_driver fuseml_driver;

static struct of_device_id fuseml_device_ids[] = {
	{
		.name = "SLD_FUSEML_RTL",
	},
	{
		.name = "eb_04a",
	},
	{
		.compatible = "sld,fuseml_rtl",
	},
	{ },
};

static int fuseml_devs;

static inline struct fuseml_rtl_device *to_fuseml(struct esp_device *esp)
{
	return container_of(esp, struct fuseml_rtl_device, esp);
}

static void fuseml_prep_xfer(struct esp_device *esp, void *arg)
{
	struct fuseml_rtl_access *a = arg;

	/* <<--regs-config-->> */
	iowrite32be(a->reg5, esp->iomem + FUSEML_REG5_REG);
	iowrite32be(a->reg4, esp->iomem + FUSEML_REG4_REG);
	iowrite32be(a->reg7, esp->iomem + FUSEML_REG7_REG);
	iowrite32be(a->reg6, esp->iomem + FUSEML_REG6_REG);
	iowrite32be(a->reg1, esp->iomem + FUSEML_REG1_REG);
	iowrite32be(a->reg3, esp->iomem + FUSEML_REG3_REG);
	iowrite32be(a->reg2, esp->iomem + FUSEML_REG2_REG);
	iowrite32be(a->reg9, esp->iomem + FUSEML_REG9_REG);
	iowrite32be(a->reg8, esp->iomem + FUSEML_REG8_REG);
	iowrite32be(a->reg10, esp->iomem + FUSEML_REG10_REG);
	iowrite32be(a->src_offset, esp->iomem + SRC_OFFSET_REG);
	iowrite32be(a->dst_offset, esp->iomem + DST_OFFSET_REG);

}

static bool fuseml_xfer_input_ok(struct esp_device *esp, void *arg)
{
	/* struct fuseml_rtl_device *fuseml = to_fuseml(esp); */
	/* struct fuseml_rtl_access *a = arg; */

	return true;
}

static int fuseml_probe(struct platform_device *pdev)
{
	struct fuseml_rtl_device *fuseml;
	struct esp_device *esp;
	int rc;

	fuseml = kzalloc(sizeof(*fuseml), GFP_KERNEL);
	if (fuseml == NULL)
		return -ENOMEM;
	esp = &fuseml->esp;
	esp->module = THIS_MODULE;
	esp->number = fuseml_devs;
	esp->driver = &fuseml_driver;
	rc = esp_device_register(esp, pdev);
	if (rc)
		goto err;

	fuseml_devs++;
	return 0;
 err:
	kfree(fuseml);
	return rc;
}

static int __exit fuseml_remove(struct platform_device *pdev)
{
	struct esp_device *esp = platform_get_drvdata(pdev);
	struct fuseml_rtl_device *fuseml = to_fuseml(esp);

	esp_device_unregister(esp);
	kfree(fuseml);
	return 0;
}

static struct esp_driver fuseml_driver = {
	.plat = {
		.probe		= fuseml_probe,
		.remove		= fuseml_remove,
		.driver		= {
			.name = DRV_NAME,
			.owner = THIS_MODULE,
			.of_match_table = fuseml_device_ids,
		},
	},
	.xfer_input_ok	= fuseml_xfer_input_ok,
	.prep_xfer	= fuseml_prep_xfer,
	.ioctl_cm	= FUSEML_RTL_IOC_ACCESS,
	.arg_size	= sizeof(struct fuseml_rtl_access),
};

static int __init fuseml_init(void)
{
	return esp_driver_register(&fuseml_driver);
}

static void __exit fuseml_exit(void)
{
	esp_driver_unregister(&fuseml_driver);
}

module_init(fuseml_init)
module_exit(fuseml_exit)

MODULE_DEVICE_TABLE(of, fuseml_device_ids);

MODULE_AUTHOR("Emilio G. Cota <cota@braap.org>");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("fuseml_rtl driver");
