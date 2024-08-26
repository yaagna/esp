#ifndef __ESP_CFG_100_H__
#define __ESP_CFG_100_H__

#include "libesp.h"
#include "fuseml_rtl.h"

typedef int32_t token_t;

unsigned _st;

unsigned words_per_dma_packet = 2;


////////////////// for unsigned int 4x4 //////////////////
/* <<--params-def-->> */
#define REG5 1         // If this is '0' then it is unsigned multiplication if this is '1' then it is signed multiplication
#define REG4 3   //Used
#define REG7 1         // reserved for future use
#define REG6 1         // reserved for future use
#define REG1 8
#define REG3 4  //Used
#define REG2 24 //Used
#define REG9 1         // reserved for future use
#define REG8 1         // reserved for future use
#define REG10 1        // reserved for future use
////////////////// for unsigned int 4x4 //////////////////

/* <<--params-->> */
 int32_t reg5 = REG5;
 int32_t reg4 = REG4;
 int32_t reg7 = REG7;
 int32_t reg6 = REG6;
 int32_t reg1 = REG1;
 int32_t reg3 = REG3;
 int32_t reg2 = REG2;
 int32_t reg9 = REG9;
 int32_t reg8 = REG8;
 int32_t reg10 = REG10;

int32_t dma_word_reg3 = 0;
int32_t dma_word_reg2 = 0;


#define NACC 1

struct fuseml_rtl_access fuseml_cfg_100[] = {
	{
		/* <<--unsigned int 4x4-->> */	//0
		/* <<--descriptor-->> */
		.reg5 = 0,
		.reg4 = 0,
		.reg7 = REG7,
		.reg6 = REG6,
		.reg1 = REG1,
		//.reg3 = reg_3,
		//.reg2 = reg_2,
		.reg3 = 2,
		.reg2 = 12,
		.reg9 = REG9,
		.reg8 = REG8,
		.reg10 = REG10,
		.src_offset = 0,
		.dst_offset = 0,
		.esp.coherence = ACC_COH_NONE,
		.esp.p2p_store = 0,
		.esp.p2p_nsrcs = 0,
		.esp.p2p_srcs = {"", "", "", ""},
	}
};

esp_thread_info_t cfg_100[] = {
	{
		.run = true,
		.devname = "fuseml_rtl.0",
		.ioctl_req = FUSEML_RTL_IOC_ACCESS,
		.esp_desc = &(fuseml_cfg_100[0].esp),
	}
};

#endif /* __ESP_CFG_100_H__ */
