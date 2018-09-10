#ifndef __FLASH_H
#define __FLASH_H

#include "gd32f1x0.h"

#define ERASE_FAILED 9

//FMCµØÖ·ÑÚÂë
#define FMC_ADDR_MASK		  ((uint32_t)0x0800fc00)
//FMCÆðÊ¼µØÖ·
#define FMC_ADDR_BASE		  ADDR_FMC_SECTOR_0			/* FMCµÄÆðÊ¼µØÖ· */
//FMC´óÐ¡
#define FMC_ALL_SIZE		  ((uint8_t)64)				/* FMCµÄ´óÐ¡,µ¥Î»:Kbytes */
/* FMCÉÈÇø´óÐ¡ */
#define FMC_PAGE_SIZE         ((uint16_t)0x400)			/* Ã¿¸öÉÈÇø´óÐ¡, 1 Kbytes */

/* FLASH ÉÈÇøµÄÆðÊ¼µØÖ· */
#define ADDR_FMC_SECTOR_0     ((uint32_t)0x08000000)	/* ÉÈÇø0ÆðÊ¼µØÖ·, 1 Kbytes */
#define ADDR_FMC_SECTOR_1     ((uint32_t)0x08000400)	/* ÉÈÇø1ÆðÊ¼µØÖ·, 1 Kbytes */
#define ADDR_FMC_SECTOR_2     ((uint32_t)0x08000800)	/* ÉÈÇø2ÆðÊ¼µØÖ·, 1 Kbytes */
#define ADDR_FMC_SECTOR_3     ((uint32_t)0x08000c00)	/* ÉÈÇø3ÆðÊ¼µØÖ·, 1 Kbytes */
#define ADDR_FMC_SECTOR_4     ((uint32_t)0x08001000)	/* ÉÈÇø4ÆðÊ¼µØÖ·, 1 Kbytes */
#define ADDR_FMC_SECTOR_5     ((uint32_t)0x08001400)	/* ÉÈÇø5ÆðÊ¼µØÖ·, 1 Kbytes */
#define ADDR_FMC_SECTOR_6     ((uint32_t)0x08001800)	/* ÉÈÇø6ÆðÊ¼µØÖ·, 1 Kbytes */
#define ADDR_FMC_SECTOR_7     ((uint32_t)0x08001c00)	/* ÉÈÇø7ÆðÊ¼µØÖ·, 1 Kbytes */
#define ADDR_FMC_SECTOR_8     ((uint32_t)0x08002000)	/* ÉÈÇø8ÆðÊ¼µØÖ·, 1 Kbytes */
#define ADDR_FMC_SECTOR_9     ((uint32_t)0x08002400)	/* ÉÈÇø9ÆðÊ¼µØÖ·, 1 Kbytes */
#define ADDR_FMC_SECTOR_10    ((uint32_t)0x08002800)	/* ÉÈÇø10ÆðÊ¼µØÖ·, 1 Kbytes */
#define ADDR_FMC_SECTOR_11    ((uint32_t)0x08002c00)	/* ÉÈÇø11ÆðÊ¼µØÖ·, 1 Kbytes */
#define ADDR_FMC_SECTOR_12    ((uint32_t)0x08003000)	/* ÉÈÇø12ÆðÊ¼µØÖ·, 1 Kbytes */
#define ADDR_FMC_SECTOR_13    ((uint32_t)0x08003400)	/* ÉÈÇø13ÆðÊ¼µØÖ·, 1 Kbytes */
#define ADDR_FMC_SECTOR_14    ((uint32_t)0x08003800)	/* ÉÈÇø14ÆðÊ¼µØÖ·, 1 Kbytes */
#define ADDR_FMC_SECTOR_15    ((uint32_t)0x08003c00)	/* ÉÈÇø15ÆðÊ¼µØÖ·, 1 Kbytes */
#define ADDR_FMC_SECTOR_16    ((uint32_t)0x08004000)	/* ÉÈÇø16ÆðÊ¼µØÖ·, 1 Kbytes */
#define ADDR_FMC_SECTOR_17    ((uint32_t)0x08004400)	/* ÉÈÇø17ÆðÊ¼µØÖ·, 1 Kbytes */
#define ADDR_FMC_SECTOR_18    ((uint32_t)0x08004800)	/* ÉÈÇø18ÆðÊ¼µØÖ·, 1 Kbytes */
#define ADDR_FMC_SECTOR_19    ((uint32_t)0x08004c00)	/* ÉÈÇø19ÆðÊ¼µØÖ·, 1 Kbytes */
#define ADDR_FMC_SECTOR_20    ((uint32_t)0x08005000)	/* ÉÈÇø20ÆðÊ¼µØÖ·, 1 Kbytes */
#define ADDR_FMC_SECTOR_21    ((uint32_t)0x08005400)	/* ÉÈÇø21ÆðÊ¼µØÖ·, 1 Kbytes */
#define ADDR_FMC_SECTOR_22    ((uint32_t)0x08005800)	/* ÉÈÇø22ÆðÊ¼µØÖ·, 1 Kbytes */
#define ADDR_FMC_SECTOR_23    ((uint32_t)0x08005c00)	/* ÉÈÇø23ÆðÊ¼µØÖ·, 1 Kbytes */
#define ADDR_FMC_SECTOR_24    ((uint32_t)0x08006000)	/* ÉÈÇø24ÆðÊ¼µØÖ·, 1 Kbytes */
#define ADDR_FMC_SECTOR_25    ((uint32_t)0x08006400)	/* ÉÈÇø25ÆðÊ¼µØÖ·, 1 Kbytes */
#define ADDR_FMC_SECTOR_26    ((uint32_t)0x08006800)	/* ÉÈÇø26ÆðÊ¼µØÖ·, 1 Kbytes */
#define ADDR_FMC_SECTOR_27    ((uint32_t)0x08006c00)	/* ÉÈÇø27ÆðÊ¼µØÖ·, 1 Kbytes */
#define ADDR_FMC_SECTOR_28    ((uint32_t)0x08007000)	/* ÉÈÇø28ÆðÊ¼µØÖ·, 1 Kbytes */
#define ADDR_FMC_SECTOR_29    ((uint32_t)0x08007400)	/* ÉÈÇø29ÆðÊ¼µØÖ·, 1 Kbytes */
#define ADDR_FMC_SECTOR_30    ((uint32_t)0x08007800)	/* ÉÈÇø30ÆðÊ¼µØÖ·, 1 Kbytes */
#define ADDR_FMC_SECTOR_31    ((uint32_t)0x08007c00)	/* ÉÈÇø31ÆðÊ¼µØÖ·, 1 Kbytes */
#define ADDR_FMC_SECTOR_32    ((uint32_t)0x08008000)	/* ÉÈÇø32ÆðÊ¼µØÖ·, 1 Kbytes */
#define ADDR_FMC_SECTOR_33    ((uint32_t)0x08008400)	/* ÉÈÇø33ÆðÊ¼µØÖ·, 1 Kbytes */
#define ADDR_FMC_SECTOR_34    ((uint32_t)0x08008800)	/* ÉÈÇø34ÆðÊ¼µØÖ·, 1 Kbytes */
#define ADDR_FMC_SECTOR_35    ((uint32_t)0x08008c00)	/* ÉÈÇø35ÆðÊ¼µØÖ·, 1 Kbytes */
#define ADDR_FMC_SECTOR_36    ((uint32_t)0x08009000)	/* ÉÈÇø36ÆðÊ¼µØÖ·, 1 Kbytes */
#define ADDR_FMC_SECTOR_37    ((uint32_t)0x08009400)	/* ÉÈÇø37ÆðÊ¼µØÖ·, 1 Kbytes */
#define ADDR_FMC_SECTOR_38    ((uint32_t)0x08009800)	/* ÉÈÇø38ÆðÊ¼µØÖ·, 1 Kbytes */
#define ADDR_FMC_SECTOR_39    ((uint32_t)0x08009c00)	/* ÉÈÇø39ÆðÊ¼µØÖ·, 1 Kbytes */
#define ADDR_FMC_SECTOR_40    ((uint32_t)0x0800a000)	/* ÉÈÇø40ÆðÊ¼µØÖ·, 1 Kbytes */
#define ADDR_FMC_SECTOR_41    ((uint32_t)0x0800a400)	/* ÉÈÇø41ÆðÊ¼µØÖ·, 1 Kbytes */
#define ADDR_FMC_SECTOR_42    ((uint32_t)0x0800a800)	/* ÉÈÇø42ÆðÊ¼µØÖ·, 1 Kbytes */
#define ADDR_FMC_SECTOR_43    ((uint32_t)0x0800ac00)	/* ÉÈÇø43ÆðÊ¼µØÖ·, 1 Kbytes */
#define ADDR_FMC_SECTOR_44    ((uint32_t)0x0800b000)	/* ÉÈÇø44ÆðÊ¼µØÖ·, 1 Kbytes */
#define ADDR_FMC_SECTOR_45    ((uint32_t)0x0800b400)	/* ÉÈÇø45ÆðÊ¼µØÖ·, 1 Kbytes */
#define ADDR_FMC_SECTOR_46    ((uint32_t)0x0800b800)	/* ÉÈÇø46ÆðÊ¼µØÖ·, 1 Kbytes */
#define ADDR_FMC_SECTOR_47    ((uint32_t)0x0800bc00)	/* ÉÈÇø47ÆðÊ¼µØÖ·, 1 Kbytes */
#define ADDR_FMC_SECTOR_48    ((uint32_t)0x0800c000)	/* ÉÈÇø48ÆðÊ¼µØÖ·, 1 Kbytes */
#define ADDR_FMC_SECTOR_49    ((uint32_t)0x0800c400)	/* ÉÈÇø49ÆðÊ¼µØÖ·, 1 Kbytes */
#define ADDR_FMC_SECTOR_50    ((uint32_t)0x0800c800)	/* ÉÈÇø50ÆðÊ¼µØÖ·, 1 Kbytes */
#define ADDR_FMC_SECTOR_51    ((uint32_t)0x0800cc00)	/* ÉÈÇø51ÆðÊ¼µØÖ·, 1 Kbytes */
#define ADDR_FMC_SECTOR_52    ((uint32_t)0x0800d000)	/* ÉÈÇø52ÆðÊ¼µØÖ·, 1 Kbytes */
#define ADDR_FMC_SECTOR_53    ((uint32_t)0x0800d400)	/* ÉÈÇø53ÆðÊ¼µØÖ·, 1 Kbytes */
#define ADDR_FMC_SECTOR_54    ((uint32_t)0x0800d800)	/* ÉÈÇø54ÆðÊ¼µØÖ·, 1 Kbytes */
#define ADDR_FMC_SECTOR_55    ((uint32_t)0x0800dc00)	/* ÉÈÇø55ÆðÊ¼µØÖ·, 1 Kbytes */
#define ADDR_FMC_SECTOR_56    ((uint32_t)0x0800e000)	/* ÉÈÇø56ÆðÊ¼µØÖ·, 1 Kbytes */
#define ADDR_FMC_SECTOR_57    ((uint32_t)0x0800e400)	/* ÉÈÇø57ÆðÊ¼µØÖ·, 1 Kbytes */
#define ADDR_FMC_SECTOR_58    ((uint32_t)0x0800e800)	/* ÉÈÇø58ÆðÊ¼µØÖ·, 1 Kbytes */
#define ADDR_FMC_SECTOR_59    ((uint32_t)0x0800ec00)	/* ÉÈÇø59ÆðÊ¼µØÖ·, 1 Kbytes */
#define ADDR_FMC_SECTOR_60    ((uint32_t)0x0800f000)	/* ÉÈÇø60ÆðÊ¼µØÖ·, 1 Kbytes */
#define ADDR_FMC_SECTOR_61    ((uint32_t)0x0800f400)	/* ÉÈÇø61ÆðÊ¼µØÖ·, 1 Kbytes */
#define ADDR_FMC_SECTOR_62    ((uint32_t)0x0800f800)	/* ÉÈÇø62ÆðÊ¼µØÖ·, 1 Kbytes */
#define ADDR_FMC_SECTOR_63    ((uint32_t)0x0800fc00)	/* ÉÈÇø63ÆðÊ¼µØÖ·, 1 Kbytes */

extern uint32_t fmc_sector_get(uint32_t address);
extern uint8_t fmc_erase_sector(uint32_t fmc_sector);
extern uint8_t fmc_write_word(uint32_t address, uint8_t *rxbuf, uint32_t rxsize);
extern void fmc_read_word(uint32_t address, uint16_t length, uint32_t* data_addr);
extern uint8_t fmc_erase(uint32_t address, uint8_t erase_page_num);		
extern uint8_t fmc_write_single(uint32_t address, uint32_t *rxbuf);

#endif

