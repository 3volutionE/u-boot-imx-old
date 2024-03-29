/*
 * Copyright (C) 2016 Amarula Solutions B.V.
 * Copyright (C) 2016 Engicam S.r.l.
 *
 * Configuration settings for the Engicam i.MX6 SOM Starter Kits.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __IMX6_ENGICAM_CONFIG_H
#define __IMX6_ENGICAM_CONFIG_H

#include <linux/sizes.h>
#include "mx6_common.h"

/* Size of malloc() pool */
#define CONFIG_SYS_MALLOC_LEN		(16 * SZ_1M)

/* Total Size of Environment Sector */
#define CONFIG_ENV_SIZE			SZ_128K

/* Allow to overwrite serial and ethaddr */
#define CONFIG_ENV_OVERWRITE

/* Environment */
#ifndef CONFIG_ENV_IS_NOWHERE
/* Environment in MMC */
# if defined(CONFIG_ENV_IS_IN_MMC)
#  define CONFIG_ENV_OFFSET		0x100000
/* Environment in NAND */
# elif defined(CONFIG_ENV_IS_IN_NAND)
#  define CONFIG_ENV_OFFSET		0x400000
#  define CONFIG_ENV_SECT_SIZE		CONFIG_ENV_SIZE
# endif
#endif

/* Default environment */
#define CONFIG_EXTRA_ENV_SETTINGS \
	"script=boot.scr\0" \
	"splashpos=m,m\0" \
	"image=uImage\0" \
	"fit_image=fit.itb\0" \
	"fdt_high=0xffffffff\0" \
	"fdt_addr=" FDT_ADDR "\0" \
	"boot_fdt=try\0" \
	"mmcpart=1\0" \
	"nandroot=ubi0:rootfs rootfstype=ubifs\0" \
	"mmcautodetect=yes\0" \
	"mmcargs=setenv bootargs console=${console},${baudrate} " \
		"root=${mmcroot}\0" \
	"ubiargs=setenv bootargs console=${console},${baudrate} " \
		"ubi.mtd=5 root=${nandroot} ${mtdparts}\0" \
	"loadbootscript=" \
		"fatload mmc ${mmcdev}:${mmcpart} ${loadaddr} ${script};\0" \
	"bootscript=echo Running bootscript from mmc ...; " \
		"source\0" \
	"loadimage=fatload mmc ${mmcdev}:${mmcpart} ${loadaddr} ${image}\0" \
	"loadfdt=fatload mmc ${mmcdev}:${mmcpart} ${fdt_addr} ${fdt_file}\0" \
	"loadfit=fatload mmc ${mmcdev}:${mmcpart} ${loadaddr} ${fit_image}\0" \
	"fitboot=echo Booting FIT image from mmc ...; " \
		"run mmcargs; " \
		"bootm ${loadaddr}\0" \
	"_mmcboot=run mmcargs; " \
		"run mmcargs; " \
		"if test ${boot_fdt} = yes || test ${boot_fdt} = try; then " \
			"if run loadfdt; then " \
				"bootm ${loadaddr} - ${fdt_addr}; " \
			"else " \
				"if test ${boot_fdt} = try; then " \
					"bootm; " \
				"else " \
					"echo WARN: Cannot load the DT; " \
				"fi; " \
			"fi; " \
		"else " \
			"bootm; " \
		"fi\0" \
	"mmcboot=echo Booting from mmc ...; " \
		"if mmc rescan; then " \
			"if run loadbootscript; then " \
				"run bootscript; " \
			"else " \
				"if run loadfit; then " \
					"run fitboot; " \
				"else " \
					"if run loadimage; then " \
						"run _mmcboot; " \
					"fi; " \
				"fi; " \
			"fi; " \
		"fi\0" \
	"nandboot=echo Booting from nand ...; " \
		"if mtdparts; then " \
			"echo Starting nand boot ...; " \
		"else " \
			"mtdparts default; " \
		"fi; " \
		"run ubiargs; " \
		"nand read ${loadaddr} kernel 0x800000; " \
		"nand read ${fdt_addr} dtb 0x100000; " \
		"bootm ${loadaddr} - ${fdt_addr}\0"

#define CONFIG_BOOTCOMMAND		"run $modeboot"

/* Miscellaneous configurable options */
#define CONFIG_SYS_MEMTEST_START	0x80000000
#define CONFIG_SYS_MEMTEST_END		(CONFIG_SYS_MEMTEST_START + 0x8000000)

#define CONFIG_SYS_LOAD_ADDR		CONFIG_LOADADDR
#define CONFIG_SYS_HZ			1000

#ifdef CONFIG_MX6UL
# define DRAM_OFFSET(x)			0x87##x
# define FDT_ADDR			__stringify(DRAM_OFFSET(800000))
#else 
# define DRAM_OFFSET(x)			0x1##x
# define FDT_ADDR			__stringify(DRAM_OFFSET(8000000))
#endif

/* Physical Memory Map */
#define CONFIG_NR_DRAM_BANKS		1
#define PHYS_SDRAM			MMDC0_ARB_BASE_ADDR

#define CONFIG_SYS_SDRAM_BASE		PHYS_SDRAM
#define CONFIG_SYS_INIT_RAM_ADDR	IRAM_BASE_ADDR
#define CONFIG_SYS_INIT_RAM_SIZE	IRAM_SIZE

#define CONFIG_SYS_INIT_SP_OFFSET	(CONFIG_SYS_INIT_RAM_SIZE - \
					GENERATED_GBL_DATA_SIZE)
#define CONFIG_SYS_INIT_SP_ADDR		(CONFIG_SYS_INIT_RAM_ADDR + \
					CONFIG_SYS_INIT_SP_OFFSET)

/* FIT */
#ifdef CONFIG_FIT
# define CONFIG_HASH_VERIFY
# define CONFIG_IMAGE_FORMAT_LEGACY
#endif

/* UART */
#ifdef CONFIG_MXC_UART
# ifdef CONFIG_MX6UL
#  define CONFIG_MXC_UART_BASE		UART1_BASE
# else
#  define CONFIG_MXC_UART_BASE		UART4_BASE
# endif
#endif

/* MMC */
#ifdef CONFIG_FSL_USDHC
# define CONFIG_SYS_MMC_ENV_DEV		0
#endif

/* NAND */
#ifdef CONFIG_NAND_MXS
# define CONFIG_SYS_MAX_NAND_DEVICE	1
# define CONFIG_SYS_NAND_BASE		0x40000000
# define CONFIG_SYS_NAND_5_ADDR_CYCLE
# define CONFIG_SYS_NAND_ONFI_DETECTION
# define CONFIG_SYS_NAND_U_BOOT_START	CONFIG_SYS_TEXT_BASE
# define CONFIG_SYS_NAND_U_BOOT_OFFS	0x200000

/* MTD device */
# define CONFIG_MTD_DEVICE
# define CONFIG_CMD_MTDPARTS
# define CONFIG_MTD_PARTITIONS
# define MTDIDS_DEFAULT			"nand0=gpmi-nand"
# define MTDPARTS_DEFAULT		"mtdparts=gpmi-nand:2m(spl),2m(uboot)," \
					"1m(env),8m(kernel),1m(dtb),-(rootfs)"

/* UBI */
# define CONFIG_CMD_UBIFS
# define CONFIG_RBTREE
# define CONFIG_LZO

# define CONFIG_APBH_DMA
# define CONFIG_APBH_DMA_BURST
# define CONFIG_APBH_DMA_BURST8
#endif

/* Ethernet */
#ifdef CONFIG_FEC_MXC
# ifdef CONFIG_TARGET_MX6Q_ICORE_RQS
#  define CONFIG_FEC_MXC_PHYADDR	3
#  define CONFIG_FEC_XCV_TYPE		RGMII
# else
#  define CONFIG_FEC_MXC_PHYADDR	0
#  define CONFIG_FEC_XCV_TYPE		RMII
# endif

# define CONFIG_MII
#endif

/* Framebuffer */
#ifdef CONFIG_VIDEO_IPUV3
# define CONFIG_IPUV3_CLK		260000000
# define CONFIG_IMX_VIDEO_SKIP

# define CONFIG_SPLASH_SCREEN
# define CONFIG_SPLASH_SCREEN_ALIGN
# define CONFIG_BMP_16BPP
# define CONFIG_VIDEO_BMP_RLE8
# define CONFIG_VIDEO_LOGO
# define CONFIG_VIDEO_BMP_LOGO
#endif

/* SPL */
#ifdef CONFIG_SPL
# ifdef CONFIG_NAND_MXS
#  define CONFIG_SPL_NAND_SUPPORT
# else
#  define CONFIG_SPL_MMC_SUPPORT
# endif

# include "imx6_spl.h"
# ifdef CONFIG_SPL_BUILD
#  if defined(CONFIG_TARGET_MX6Q_ICORE_RQS) || defined(CONFIG_TARGET_MX6UL_ISIOT)
#   define CONFIG_SYS_FSL_USDHC_NUM	2
#  else
#   define CONFIG_SYS_FSL_USDHC_NUM	1
#  endif

#  define CONFIG_SYS_FSL_ESDHC_ADDR	0
#  undef CONFIG_DM_GPIO
#  undef CONFIG_DM_MMC
# endif
#endif

#endif /* __IMX6_ENGICAM_CONFIG_H */
