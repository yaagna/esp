/* Copyright (c) 2011-2024 Columbia University, System Level Design Group */
/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>
/*
#ifndef __riscv
#include <stdlib.h>
#include <time.h>
#include <math.h>
#endif
*/

// Added header
#include <stdlib.h>
#include <time.h>
#include <math.h>
//#include <test.h>

#include <esp_accelerator.h>
#include <esp_probe.h>
#include <fixed_point.h>

typedef int32_t token_t;


static unsigned DMA_WORD_PER_BEAT(unsigned _st)
{
        return (sizeof(void *) / _st);
}


#define SLD_FUSEML 0x04a
#define DEV_NAME "sld,fuseml_rtl"

/* <<--params-->> */
const int32_t reg5 = 1;          // If this is '0' then it is unsigned multiplication if this is '1' then it is signed multiplication
const int32_t reg4 = 2;// Used
const int32_t reg7 = 1;          // reserved for future use
const int32_t reg6 = 1;          // reserved for future use
const int32_t reg1 = 8;
const int32_t reg3 = 64;//Used
const int32_t reg2 = 204;//Used
const int32_t reg9 = 1;          // reserved for future use
const int32_t reg8 = 1;          // reserved for future use
const int32_t reg10 = 1;          // reserved for future use

static unsigned in_words_adj;
static unsigned out_words_adj;
static unsigned in_len;
static unsigned out_len;
static unsigned in_size;
static unsigned out_size;
static unsigned out_offset;
static unsigned mem_size;

/* Size of the contiguous chunks for scatter/gather */
#define CHUNK_SHIFT 20
#define CHUNK_SIZE BIT(CHUNK_SHIFT)
#define NCHUNK(_sz) ((_sz % CHUNK_SIZE == 0) ?		\
			(_sz / CHUNK_SIZE) :		\
			(_sz / CHUNK_SIZE) + 1)

/* User defined registers */
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


static int validate_buf(token_t *out, token_t *gold)
{
	int i;
	int j;
	unsigned errors = 0;

	for (i = 0; i < 1; i++)
		//for (j = 0; j < 2*reg2; j++) {
		for (j = 0; j < reg3; j++) { //reg3 is the output index
			printf("acc out %d gold: %d, out: %d, gold_addr: %p, out_addr: %p \n", 
					j, gold[i * out_words_adj + j], out[i * out_words_adj + j], 
					&gold[i * out_words_adj + j], &out[i * out_words_adj + j]);
			
			if (gold[i * out_words_adj + j] != out[i * out_words_adj + j])
				errors++;
	
	}
	return errors;
}


static void init_buf (token_t *in, token_t * gold)
{
	int i;
	int j;

        // Added

int32_t in_buffs[204] = {
0x8779ffab,                                                                        
0x0,                                                                               
0xa86d4d05,                                                                        
0xbe6528ab,                                                                        
0x8d9a9be3,                                                                        
0xa06c307e,                                                                        
0xc2f8bdb3,                                                                        
0x1757cd0a,                                                                        
0xf290dad7,                                                                       
0x61f50d78,                                                                        
0xcefb61f5,                                                                        
0x7218987e,                                                                        
0x79e22587,                                                                        
0x4d4af897,                                                                        
0x9b87402d,                                                                        
0xdbc576c,                                                                         
0x8779ffab,                                                                        
0xd4261257,                                                                        
0xa86d4d05,                                                                        
0xbe6528ab,                                                                        
0x8d9a9be3,                                                                       
0xa06c307e,                                                                        
0xc2f8bdb3,                                                                        
0x1757cd0a,                                                                        
0xf290dad7,                                                                        
0x61f50d78,                                                                        
0xcefb61f5,                                                                        
0x7218987e,                                                                        
0x79e22587,                                                                        
0x4d4af897,                                                                        
0x9b87402d,                                                                        
0xdbc576c,                                                                         
0x8779ffab,                                                                        
0xd4261257,                                                                        
0xa86d4d05,                                                                        
0xbe6528ab,                                                                        
0x8d9a9be3,                                                                        
0xa06c307e,                                                                        
0xc2f8bdb3,                                                                        
0x1757cd0a,                                                                        
0xf290dad7,                                                                        
0x61f50d78,                                                                        
0xcefb61f5,                                                                        
0x7218987e,                                                                        
0x79e22587,                                                                        
0x4d4af897,                                                                        
0x9b87402d,                                                                        
0xdbc576c,                                                                         
0x8779ffab,                                                                        
0xd4261257,                                                                        
0xa86d4d05,                                                                        
0xbe6528ab,                                                                        
0x8d9a9be3,                                                                        
0xa06c307e,                                                                        
0xc2f8bdb3,                                                                        
0x1757cd0a,                                                                        
0xf290dad7,                                                                        
0x61f50d78,                                                                        
0xcefb61f5,                                                                        
0x7218987e,                                                                        
0x79e22587,                                                                        
0x4d4af897,                                                                        
0x9b87402d,                                                                        
0xdbc576c,                                                                         
0x0,                                                                               
0xd4261257,                                                                        
0x0,                                                                               
0x0,                                                                               
0x77117711,                                                                        
0x0,                                                                               
0x77117711,                                                                        
0x44ff44ff,                                                                        
0x77117711,                                                                        
0x44ff44ff,                                                                        
0x77117711,                                                                        
0x44ff44ff,                                                                        
0x77117711,                                                                        
0x44ff44ff,                                                                        
0x77117711,                                                                        
0x44ff44ff,                                                                        
0x77117711,                                                                        
0x44ff44ff,                                                                        
0x77117711,                                                                        
0x44ff44ff,                                                                        
0xaa22aa22,                                                                        
0x44ff44ff,                                                                        
0xaa22aa22,                                                                        
0x44004400,                                                                        
0xaa22aa22,                                                                        
0x44004400,                                                                        
0xaa22aa22,                                                                        
0x44004400,                                                                        
0xaa22aa22,                                                                        
0x44004400,                                                                        
0xaa22aa22,                                                                        
0x44004400,                                                                        
0xaa22aa22,                                                                        
0x44004400,                                                                        
0xaa22aa22,                                                                        
0x44004400,                                                                        
0x77007700,                                                                        
0x44004400,                                                                        
0x77007700,                                                                        
0x77ff77ff,                                                                        
0x77007700,                                                                        
0x77ff77ff,                                                                        
0x77007700,                                                                        
0x77ff77ff,                                                                        
0x77007700,                                                                        
0x77ff77ff,                                                                        
0x77007700,                                                                        
0x77ff77ff,                                                                        
0x77007700,                                                                        
0x77ff77ff,                                                                        
0x77007700,                                                                        
0x77ff77ff,                                                                        
0x33113311,                                                                        
0x77ff77ff,                                                                        
0x33113311,                                                                        
0xee00ee00,                                                                        
0x33113311,                                                                        
0xee00ee00,                                                                        
0x33113311,                                                                        
0xee00ee00,                                                                        
0x33113311,                                                                        
0xee00ee00,                                                                        
0x33113311,                                                                        
0xee00ee00,                                                                        
0x33113311,                                                                        
0xee00ee00,                                                                        
0x33113311,                                                                        
0xee00ee00,                                                                        
0x0,                                                                               
0xee00ee00,                                                                        
0x0,                                                                               
0x0,                                                                               
0x0,                                                                               
0x0,                                                                               
0x88778877,                                                                        
0x0,                                                                               
0x88778877,                                                                        
0x11441144,                                                                        
0x88778877,                                                                        
0x11441144,                                                                        
0x88778877,                                                                        
0x11441144,                                                                        
0x88778877,                                                                        
0x11441144,                                                                        
0x88778877,                                                                        
0x11441144,                                                                        
0x88778877,                                                                        
0x11441144,                                                                        
0x88778877,                                                                        
0x11441144,                                                                        
0xdd99dd99,                                                                        
0x11441144,                                                                        
0xdd99dd99,                                                                        
0x33553355,                                                                        
0xdd99dd99,                                                                        
0x33553355,                                                                        
0xdd99dd99,                                                                        
0x33553355,                                                                        
0xdd99dd99,                                                                        
0x33553355,                                                                        
0xdd99dd99,                                                                        
0x33553355,                                                                        
0xdd99dd99,                                                                        
0x33553355,                                                                        
0xdd99dd99,                                                                        
0x33553355,                                                                        
0x11bb11bb,                                                                        
0x33553355,                                                                        
0x11bb11bb,                                                                        
0x88ee88ee,                                                                        
0x11bb11bb,                                                                        
0x88ee88ee,                                                                        
0x11bb11bb,                                                                        
0x88ee88ee,                                                                        
0x11bb11bb,                                                                        
0x88ee88ee,                                                                        
0x11bb11bb,                                                                        
0x88ee88ee,                                                                        
0x11bb11bb,                                                                        
0x88ee88ee,                                                                        
0x11bb11bb,                                                                        
0x88ee88ee,                                                                        
0xee99ee99,                                                                        
0x88ee88ee,                                                                        
0xee99ee99,                                                                        
0x66ff66ff,                                                                        
0xee99ee99,                                                                        
0x66ff66ff,                                                                        
0xee99ee99,                                                                        
0x66ff66ff,                                                                        
0xee99ee99,                                                                        
0x66ff66ff,                                                                        
0xee99ee99,                                                                        
0x66ff66ff,                                                                        
0xee99ee99,                                                                        
0x66ff66ff,                                                                        
0xee99ee99,                                                                        
0x66ff66ff,                                                                        
0x0,                                                                               
0x66ff66ff 
};

int32_t gold_buff[64] = {
132,                                                                             
54,                                                                              
5,                                                                               
-170,                                                                            
-45,                                                                             
-163,                                                                            
-196,                                                                            
-94,                                                                             
206,                                                                             
-157,                                                                            
333,                                                                             
-62,                                                                             
-337,                                                                            
-50,                                                                             
229,                                                                             
-51,                                                                             
391,                                                                             
-170,                                                                            
-156,                                                                            
-201,                                                                            
518,                                                                            
-397,                                                                            
242,                                                                             
-149,                                                                            
573,                                                                             
-223,                                                                            
-655,                                                                            
44,                                                                              
-277,                                                                            
118,                                                                             
-618,                                                                            
183,                                                                             
245,                                                                             
477,                                                                             
94,                                                                              
-64,                                                                             
400,                                                                             
304,                                                                             
76,                                                                             
-58,                                                                             
210,                                                                             
345,                                                                             
-191,                                                                            
-214,                                                                            
-41,                                                                             
-295,                                                                            
-228,                                                                            
-59,                                                                             
163,                                                                             
-65,                                                                             
-106,                                                                            
-79,                                                                             
10,                                                                              
242,                                                                             
55,                                                                              
-364,                                                                            
96,                                                                              
249,                                                                             
-264,                                                                            
-475,                                                                            
-58,                                                                             
-100,                                                                            
104,                                                                             
131
};

        // Added Complete



	for (i = 0; i < 1; i++)
		//for (j = 0; j < 2*reg2; j++)
		for (j = 0; j < reg2; j++){
			in[i * in_words_adj + j] = (token_t) in_buffs[j];
			//printf("in_buff = %x\n", in_buffs[j]);
			}
	for (i = 0; i < 1; i++)
		//for (j = 0; j < 2*reg2; j++)
		for (j = 0; j < reg3; j++) //gold is initialized using reg3
			gold[i * out_words_adj + j] = (token_t) gold_buff[j];
}


int main(int argc, char * argv[])
{
	int i;
	int n;
	int ndev;
	struct esp_device *espdevs;
        struct esp_device espdevs_1;
	struct esp_device *dev;
	unsigned done;
	unsigned **ptable;
	token_t *mem;
	token_t *gold;
	unsigned errors = 0;
	unsigned coherence;


	//division factor for number of 32-bit words (reg2) into DMA packets
	unsigned words_per_dma_packet = DMA_WORD_PER_BEAT(sizeof(token_t));
        //unsigned words_per_dma_packet = 2;
        printf("Size of token: %d\n", sizeof(token_t));
        printf("Words %d\n", DMA_WORD_PER_BEAT(sizeof(token_t)));

	if (DMA_WORD_PER_BEAT(sizeof(token_t)) == 0) {
		in_words_adj = reg2;
		out_words_adj = reg3;
	} else {
		in_words_adj = round_up(reg2, DMA_WORD_PER_BEAT(sizeof(token_t)));
		out_words_adj = round_up(reg3, DMA_WORD_PER_BEAT(sizeof(token_t)));
	}
	in_len = in_words_adj * (1);
	out_len = out_words_adj * (1);
	in_size = in_len * sizeof(token_t);
	out_size = out_len * sizeof(token_t);
	out_offset  = in_len;
	mem_size = (out_offset * sizeof(token_t)) + out_size;


	// Search for the device
	printf("Scanning device tree... \n");
	espdevs_1.addr = 0x60010000;

/*
	ndev = probe(&espdevs, VENDOR_SLD, SLD_FUSEML, DEV_NAME);
	if (ndev == 0) {
		printf("fuseml not found\n");
		return 0;
	}
*/
	dev = &espdevs_1;
	ndev = 1;

	for (n = 0; n < ndev; n++) {

		printf("**************** %s.%d ****************\n", DEV_NAME, n);

		//dev = &espdevs[n];

		// Check DMA capabilities
		if (ioread32(dev, PT_NCHUNK_MAX_REG) == 0) {
			printf("  -> scatter-gather DMA is disabled. Abort.\n");
			return 0;
		}

		if (ioread32(dev, PT_NCHUNK_MAX_REG) < NCHUNK(mem_size)) {
			printf("  -> Not enough TLB entries available. Abort.\n");
			return 0;
		}

		// Allocate memory
		gold = aligned_malloc(out_size);
		mem = aligned_malloc(mem_size);
		//printf("  memory buffer base-address = %p\n", mem);

		// Alocate and populate page table
		ptable = aligned_malloc(NCHUNK(mem_size) * sizeof(unsigned *));
		for (i = 0; i < NCHUNK(mem_size); i++)
			ptable[i] = (unsigned *) &mem[i * (CHUNK_SIZE / sizeof(token_t))];

		//printf("  ptable = %p\n", ptable);
		//printf("  nchunk = %lu\n", NCHUNK(mem_size));

#ifndef __riscv
		for (coherence = ACC_COH_NONE; coherence <= ACC_COH_RECALL; coherence++) {
#else
		{
			/* TODO: Restore full test once ESP caches are integrated */
			coherence = ACC_COH_NONE;
#endif
			printf("  --------------------\n");
			//printf("  Generate input...\n");
			init_buf(mem, gold);

			// Pass common configuration parameters

			iowrite32(dev, SELECT_REG, ioread32(dev, DEVID_REG));
			iowrite32(dev, COHERENCE_REG, coherence);

#ifndef __sparc
			iowrite32(dev, PT_ADDRESS_REG, (unsigned long long) ptable);
#else
			iowrite32(dev, PT_ADDRESS_REG, (unsigned) ptable);
#endif
			iowrite32(dev, PT_NCHUNK_REG, NCHUNK(mem_size));
			iowrite32(dev, PT_SHIFT_REG, CHUNK_SHIFT);

			// Use the following if input and output data are not allocated at the default offsets
			iowrite32(dev, SRC_OFFSET_REG, 0x0);
			iowrite32(dev, DST_OFFSET_REG, 0x0);

			// Pass accelerator-specific configuration parameters
			/* <<--regs-config-->> */
		iowrite32(dev, FUSEML_REG5_REG, reg5);
		iowrite32(dev, FUSEML_REG4_REG, reg4);
		iowrite32(dev, FUSEML_REG7_REG, reg7);
		iowrite32(dev, FUSEML_REG6_REG, reg6);
		iowrite32(dev, FUSEML_REG1_REG, reg1);
		iowrite32(dev, FUSEML_REG3_REG, reg3/words_per_dma_packet);   // The number of 64-bit DMA packets for DMA
		iowrite32(dev, FUSEML_REG2_REG, reg2/words_per_dma_packet);   // The number of 64-bit DMA packets for DMA read
		iowrite32(dev, FUSEML_REG9_REG, reg9);
		iowrite32(dev, FUSEML_REG8_REG, reg8);
		iowrite32(dev, FUSEML_REG10_REG, reg10);

			// Flush (customize coherence model here)
			//esp_flush(coherence);

			// Start accelerators

			printf("  Start...\n");
			iowrite32(dev, CMD_REG, CMD_MASK_START);

			// Wait for completion
			done = 0;
			while (!done) {
				done = ioread32(dev, STATUS_REG);
				done &= STATUS_MASK_DONE;
			}
			iowrite32(dev, CMD_REG, 0x0);

			printf("  Done\n");
			printf("  validating...\n");

			/* Validation */

			errors = validate_buf(&mem[out_offset], gold);
			if (errors)
				printf("  ... FAIL\n");
			else
				printf("  ... PASS\n");

		}
		aligned_free(ptable);
		aligned_free(mem);
		aligned_free(gold);
	}

	return 0;
}
