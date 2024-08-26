// Copyright (c) 2011-2024 Columbia University, System Level Design Group
// SPDX-License-Identifier: Apache-2.0
#ifndef __ESP_CFG_009_H__
#define __ESP_CFG_009_H__

#include "libesp.h"
#include "fuseml_rtl.h"

typedef int32_t token_t;

unsigned _st;

unsigned words_per_dma_packet = 2;

/* <<--params-def-->> */
#define REG5 1         // If this is '0' then it is unsigned multiplication if this is '1' then it is signed multiplication
#define REG4 0   //Used
#define REG7 1         // reserved for future use
#define REG6 1         // reserved for future use
#define REG1 8
#define REG3 1024  //Used
#define REG2 3084  //Used
#define REG9 1         // reserved for future use
#define REG8 1         // reserved for future use
#define REG10 1        // reserved for future use

////// Paramaterized ///////
//int32_t reg_3;
//reg_3 = REG3/words_per_dma_packet;
//int32_t reg_2;
//reg_2 = REG2/words_per_dma_packet;

/* <<--params-->> */
const int32_t reg5 = REG5;
const int32_t reg4 = REG4;
const int32_t reg7 = REG7;
const int32_t reg6 = REG6;
const int32_t reg1 = REG1;
const int32_t reg3 = REG3;
const int32_t reg2 = REG2;
const int32_t reg9 = REG9;
const int32_t reg8 = REG8;
const int32_t reg10 = REG10;

#define NACC 1

struct fuseml_rtl_access fuseml_cfg_009[] = {
	{
		/* <<--descriptor-->> */
		.reg5 = REG5,
		.reg4 = REG4,
		.reg7 = REG7,
		.reg6 = REG6,
		.reg1 = REG1,
		//.reg3 = reg_3,
		//.reg2 = reg_2,
		.reg3 = 512,
		.reg2 = 1542,
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

esp_thread_info_t cfg_009[] = {
	{
		.run = true,
		.devname = "fuseml_rtl.0",
		.ioctl_req = FUSEML_RTL_IOC_ACCESS,
		.esp_desc = &(fuseml_cfg_009[0].esp),
	}
};

#endif /* __ESP_CFG_004_H__ */
