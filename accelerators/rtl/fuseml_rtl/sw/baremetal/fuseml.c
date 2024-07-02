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
const int32_t reg5 = 1;          // reserved for future use
const int32_t reg4 = 1;// Used
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
		//for (j = 0; j < 2 * reg2; j++)
                for (j = 0; j < reg3; j++) { //reg3 is the output index
			printf("acc out %d gold: %d, out: %d, gold_addr: %p, out_addr: %p \n", 
					j, gold[i * out_words_adj + j], out[i * out_words_adj + j], 
					&gold[i * out_words_adj + j], &out[i * out_words_adj + j]);

			if (gold[i * out_words_adj + j] != out[i * out_words_adj + j])
				errors++;
        }
	return errors;
}


static void init_buf (token_t *in, token_t * gold, int run)
{
	int i;
	int j;

        // Added
int32_t in_buffs_0[204] = { 
0x88111133, //IBUF0
0x00000000, //IBUF1

0x11116699,
0x66aacc33,

0x44ffbb,
0xaa6699aa,

0xffff1188,
0x77eeee33,

0xaaffcc99,
0x55336611,

0xddaa77ee,
0xee111111,

0x7700bbaa,
0xccff9977,

0x668888ff,
0xffee4499,

0x88111133,
0xaa55aacc,

0x11116699,
0x66aacc33,

0x44ffbb,
0xaa6699aa,

0xffff1188,
0x77eeee33,

0xaaffcc99,
0x55336611,

0xddaa77ee,
0xee111111,

0x7700bbaa,
0xccff9977,

0x668888ff,
0xffee4499,

0x88111133,
0xaa55aacc,

0x11116699,
0x66aacc33,

0x44ffbb,
0xaa6699aa,

0xffff1188,
0x77eeee33,

0xaaffcc99,
0x55336611,

0xddaa77ee,
0xee111111,

0x7700bbaa,
0xccff9977,

0x668888ff,
0xffee4499,

0x88111133,
0xaa55aacc,

0x11116699,
0x66aacc33,

0x44ffbb,
0xaa6699aa,

0xffff1188,
0x77eeee33,

0xaaffcc99,
0x55336611,

0xddaa77ee,
0xee111111,

0x7700bbaa,
0xccff9977,

0x668888ff,
0xffee4499,

0x00000000,
0xaa55aacc,

0x00000000,
0x00000000,



0xafa5aa0f, //WBUF0
0x00000000, //WBUF1

0xafa5aa0f,
0xa5aa5500,

0xafa5aa0f,
0xa5aa5500,

0xafa5aa0f,
0xa5aa5500,

0xafa5aa0f,
0xa5aa5500,

0xafa5aa0f,
0xa5aa5500,

0xafa5aa0f,
0xa5aa5500,

0xafa5aa0f,
0xa5aa5500,

0xa05aaaf0,
0xa5aa5500,

0xa05aaaf0,
0x50ff005a,

0xa05aaaf0,
0x50ff005a,

0xa05aaaf0,
0x50ff005a,

0xa05aaaf0,
0x50ff005a,

0xa05aaaf0,
0x50ff005a,

0xa05aaaf0,
0x50ff005a,

0xa05aaaf0,
0x50ff005a,

0xff505aaa,
0x50ff005a,

0xff505aaa,
0xff0050fa,

0xff505aaa,
0xff0050fa,

0xff505aaa,
0xff0050fa,

0xff505aaa,
0xff0050fa,

0xff505aaa,
0xff0050fa,

0xff505aaa,
0xff0050fa,

0xff505aaa,
0xff0050fa,

0x5faffa55,
0xff0050fa,

0x5faffa55,
0x50faafaa,

0x5faffa55,
0x50faafaa,

0x5faffa55,
0x50faafaa,

0x5faffa55,
0x50faafaa,

0x5faffa55,
0x50faafaa,

0x5faffa55,
0x50faafaa,

0x5faffa55,
0x50faafaa,

0x00000000,
0x50faafaa,

0x00000000,
0x00000000,



0x00000000, //WBUF2
0x00000000, //WBUF3

0x5f5505a5,
0x00000000,

0x5f5505a5,
0xa0a0ff,

0x5f5505a5,
0xa0a0ff,

0x5f5505a5,
0xa0a0ff,

0x5f5505a5,
0xa0a0ff,

0x5f5505a5,
0xa0a0ff,

0x5f5505a5,
0xa0a0ff,

0x5f5505a5,
0xa0a0ff,

0x5a00a5a0,
0xa0a0ff,

0x5a00a5a0,
0x50affaa0,

0x5a00a5a0,
0x50affaa0,

0x5a00a5a0,
0x50affaa0,

0x5a00a5a0,
0x50affaa0,

0x5a00a5a0,
0x50affaa0,

0x5a00a5a0,
0x50affaa0,

0x5a00a5a0,
0x50affaa0,

0xa55550aa,
0x50affaa0,

0xa55550aa,
0xaaa00005,

0xa55550aa,
0xaaa00005,

0xa55550aa,
0xaaa00005,

0xa55550aa,
0xaaa00005,

0xa55550aa,
0xaaa00005,

0xa55550aa,
0xaaa00005,

0xa55550aa,
0xaaa00005,

0xff05ff5f,
0xaaa00005,

0xff05ff5f,
0xa55affaf,

0xff05ff5f,
0xa55affaf,

0xff05ff5f,
0xa55affaf,

0xff05ff5f,
0xa55affaf,

0xff05ff5f,
0xa55affaf,

0xff05ff5f,
0xa55affaf,

0xff05ff5f,
0xa55affaf,

0x00000000,
0xa55affaf

};


/*
int32_t in_buffs[204] = {
0xeeccee66,
0x00000000,
0xaabb7755,
0x11444433,
0x33776644,
0x55ffbb44,
0xcc559900,
0xaacc6699,
0x4455dd11,
0x22aa7722,
0x88cc2255,
0xeebb9977,
0x006644aa,
0x00ff0044,
0xbb115555,
0x660066ff,
0xeeccee66,
0x8833dd11,
0xaabb7755,
0x11444433,
0x33776644,
0x55ffbb44,
0xcc559900,
0xaacc6699,
0x4455dd11,
0x22aa7722,
0x88cc2255,
0xeebb9977,
0x6644aa,
0xff0044,
0xbb115555,
0x660066ff,
0xeeccee66,
0x8833dd11,
0xaabb7755,
0x11444433,
0x33776644,
0x55ffbb44,
0xcc559900,
0xaacc6699,
0x4455dd11,
0x22aa7722,
0x88cc2255,
0xeebb9977,
0x6644aa,
0xff0044,
0xbb115555,
0x660066ff,
0xeeccee66,
0x8833dd11,
0xaabb7755,
0x11444433,
0x33776644,
0x55ffbb44,
0xcc559900,
0xaacc6699,
0x4455dd11,
0x22aa7722,
0x88cc2255,
0xeebb9977,
0x6644aa,
0xff0044,
0xbb115555,
0x660066ff,
0x0,
0x8833dd11,
0x0,
0x0,
0xaaa5055f,
0x0,
0xaaa5055f,
0x55a0f50f,
0xaaa5055f,
0x55a0f50f,
0xaaa5055f,
0x55a0f50f,
0xaaa5055f,
0x55a0f50f,
0xaaa5055f,
0x55a0f50f,
0xaaa5055f,
0x55a0f50f,
0xaaa5055f,
0x55a0f50f,
0x50a0055f,
0x55a0f50f,
0x50a0055f,
0x5aaf55a0,
0x50a0055f,
0x5aaf55a0,
0x50a0055f,
0x5aaf55a0,
0x50a0055f,
0x5aaf55a0,
0x50a0055f,
0x5aaf55a0,
0x50a0055f,
0x5aaf55a0,
0x50a0055f,
0x5aaf55a0,
0x50a0fa0,
0x5aaf55a0,
0x50a0fa0,
0xa00500ff,
0x50a0fa0,
0xa00500ff,
0x50a0fa0,
0xa00500ff,
0x50a0fa0,
0xa00500ff,
0x50a0fa0,
0xa00500ff,
0x50a0fa0,
0xa00500ff,
0x50a0fa0,
0xa00500ff,
0x5a5550a,
0xa00500ff,
0x5a5550a,
0x5faa50f0,
0x5a5550a,
0x5faa50f0,
0x5a5550a,
0x5faa50f0,
0x5a5550a,
0x5faa50f0,
0x5a5550a,
0x5faa50f0,
0x5a5550a,
0x5faa50f0,
0x5a5550a,
0x5faa50f0,
0x0,
0x5faa50f0,
0x0,
0x0,
0x0,
0x0,
0xfaa0a00a,
0x0,
0xfaa0a00a,
0x5f0f0af0,
0xfaa0a00a,
0x5f0f0af0,
0xfaa0a00a,
0x5f0f0af0,
0xfaa0a00a,
0x5f0f0af0,
0xfaa0a00a,
0x5f0f0af0,
0xfaa0a00a,
0x5f0f0af0,
0xfaa0a00a,
0x5f0f0af0,
0x55f505ff,
0x5f0f0af0,
0x55f505ff,
0xaa55fa0,
0x55f505ff,
0xaa55fa0,
0x55f505ff,
0xaa55fa0,
0x55f505ff,
0xaa55fa0,
0x55f505ff,
0xaa55fa0,
0x55f505ff,
0xaa55fa0,
0x55f505ff,
0xaa55fa0,
0x550ff50f,
0xaa55fa0,
0x550ff50f,
0xaff005f5,
0x550ff50f,
0xaff005f5,
0x550ff50f,
0xaff005f5,
0x550ff50f,
0xaff005f5,
0x550ff50f,
0xaff005f5,
0x550ff50f,
0xaff005f5,
0x550ff50f,
0xaff005f5,
0xa50a050,
0xaff005f5,
0xa50a050,
0xa055aaaa,
0xa50a050,
0xa055aaaa,
0xa50a050,
0xa055aaaa,
0xa50a050,
0xa055aaaa,
0xa50a050,
0xa055aaaa,
0xa50a050,
0xa055aaaa,
0xa50a050,
0xa055aaaa,
0x0,
0xa055aaaa
};
*/





int32_t gold_buff_0[64] = { 

//Col0
358,
362,

399,
391,

466,
441,

486,
533,

430,
379,

549,
591,

528,
373,

314,
276,

401,
424,

412,
415,

597,
522,

417,
607,

406,
477,

554,
609,

511,
355,

458,
415,

453,
396,

449,
362,

570,
486,

510,
419,

463,
392,

589,
474,

633,
635,

490,
496,

448,
546,

547,
575,

746,
799,

586,
630,

564,
570,

637,
771,

630,
588,

514,
539
};


/*
int32_t gold_buff[64] = {
307,
426,
578,
466,
501,
453,
393,
341,
430,
435,
212,
210,
380,
376,
404,
294,
356,
346,
449,
570,
356,
426,
361,
364,
517,
422,
285,
268,
422,
385,
361,
386,
194,
439,
372,
431,
267,
357,
294,
354,
278,
537,
120,
287,
308,
462,
226,
416,
179,
426,
397,
587,
304,
507,
270,
419,
267,
481,
143,
221,
256,
526,
209,
404
};
*/

int32_t in_buffs_1[204] = {
0xff77ff22,
0x0,
0x22cc77,
0x1111ddcc,
0xcccc6633,
0x2211ccff,
0x885555bb,
0x11559955,
0x66aaffdd,
0x44009977,
0xdd6677ee,
0xffcccc00,
0xee446688,
0x7722dd44,
0xbbddccee,
0xbbee3366,
0xff77ff22,
0x4400eeee,
0x22cc77,
0x1111ddcc,
0xcccc6633,
0x2211ccff,
0x885555bb,
0x11559955,
0x66aaffdd,
0x44009977,
0xdd6677ee,
0xffcccc00,
0xee446688,
0x7722dd44,
0xbbddccee,
0xbbee3366,
0xff77ff22,
0x4400eeee,
0x22cc77,
0x1111ddcc,
0xcccc6633,
0x2211ccff,
0x885555bb,
0x11559955,
0x66aaffdd,
0x44009977,
0xdd6677ee,
0xffcccc00,
0xee446688,
0x7722dd44,
0xbbddccee,
0xbbee3366,
0xff77ff22,
0x4400eeee,
0x22cc77,
0x1111ddcc,
0xcccc6633,
0x2211ccff,
0x885555bb,
0x11559955,
0x66aaffdd,
0x44009977,
0xdd6677ee,
0xffcccc00,
0xee446688,
0x7722dd44,
0xbbddccee,
0xbbee3366,
0x0,
0x4400eeee,
0x0,
0x0,
0x55a0555f,
0x0,
0x55a0555f,
0xfffa055a,
0x55a0555f,
0xfffa055a,
0x55a0555f,
0xfffa055a,
0x55a0555f,
0xfffa055a,
0x55a0555f,
0xfffa055a,
0x55a0555f,
0xfffa055a,
0x55a0555f,
0xfffa055a,
0xa5a0055f,
0xfffa055a,
0xa5a0055f,
0x5055aa0a,
0xa5a0055f,
0x5055aa0a,
0xa5a0055f,
0x5055aa0a,
0xa5a0055f,
0x5055aa0a,
0xa5a0055f,
0x5055aa0a,
0xa5a0055f,
0x5055aa0a,
0xa5a0055f,
0x5055aa0a,
0x500aaaa5,
0x5055aa0a,
0x500aaaa5,
0xa5aaf0f0,
0x500aaaa5,
0xa5aaf0f0,
0x500aaaa5,
0xa5aaf0f0,
0x500aaaa5,
0xa5aaf0f0,
0x500aaaa5,
0xa5aaf0f0,
0x500aaaa5,
0xa5aaf0f0,
0x500aaaa5,
0xa5aaf0f0,
0x5a05fa50,
0xa5aaf0f0,
0x5a05fa50,
0xffa055f0,
0x5a05fa50,
0xffa055f0,
0x5a05fa50,
0xffa055f0,
0x5a05fa50,
0xffa055f0,
0x5a05fa50,
0xffa055f0,
0x5a05fa50,
0xffa055f0,
0x5a05fa50,
0xffa055f0,
0x0,
0xffa055f0,
0x0,
0x0,
0x0,
0x0,
0x50a5055a,
0x0,
0x50a5055a,
0xfa0fff55,
0x50a5055a,
0xfa0fff55,
0x50a5055a,
0xfa0fff55,
0x50a5055a,
0xfa0fff55,
0x50a5055a,
0xfa0fff55,
0x50a5055a,
0xfa0fff55,
0x50a5055a,
0xfa0fff55,
0xfa0af5fa,
0xfa0fff55,
0xfa0af5fa,
0xaf5a05fa,
0xfa0af5fa,
0xaf5a05fa,
0xfa0af5fa,
0xaf5a05fa,
0xfa0af5fa,
0xaf5a05fa,
0xfa0af5fa,
0xaf5a05fa,
0xfa0af5fa,
0xaf5a05fa,
0xfa0af5fa,
0xaf5a05fa,
0x55f5055a,
0xaf5a05fa,
0x55f5055a,
0xa050ff0f,
0x55f5055a,
0xa050ff0f,
0x55f5055a,
0xa050ff0f,
0x55f5055a,
0xa050ff0f,
0x55f5055a,
0xa050ff0f,
0x55f5055a,
0xa050ff0f,
0x55f5055a,
0xa050ff0f,
0x5f5fa5a,
0xa050ff0f,
0x5f5fa5a,
0x5fa00f0f,
0x5f5fa5a,
0x5fa00f0f,
0x5f5fa5a,
0x5fa00f0f,
0x5f5fa5a,
0x5fa00f0f,
0x5f5fa5a,
0x5fa00f0f,
0x5f5fa5a,
0x5fa00f0f,
0x5f5fa5a,
0x5fa00f0f,
0x0,
0x5fa00f0f
};

int32_t gold_buff_1[64] = {
387,
658,
331,
437,
265,
354,
261,
358,
339,
596,
386,
626,
343,
654,
299,
611,
507,
408,
527,
421,
358,
291,
435,
310,
480,
436,
528,
437,
630,
501,
573,
319,
405,
703,
348,
442,
337,
466,
283,
433,
386,
622,
457,
685,
433,
604,
281,
570,
517,
590,
294,
377,
378,
359,
306,
327,
398,
512,
498,
553,
580,
491,
398,
486
};

	for (i = 0; i < 1; i++)
		//for (j = 0; j < 2*reg2; j++)
		for (j = 0; j < reg2; j++){
                        if (run == 0)
			       in[i * in_words_adj + j] = (token_t) in_buffs_0[j];
                        else
			       in[i * in_words_adj + j] = (token_t) in_buffs_1[j];
			//printf("in_buff = %x\n", in_buffs[j]);
			}
	for (i = 0; i < 1; i++)
		//for (j = 0; j < 2*reg2; j++)
		for (j = 0; j < reg3; j++) //gold is initialized using reg3
                        if (run = 0)
			      gold[i * out_words_adj + j] = (token_t) gold_buff_0[j];
                        else 
                              gold[i * out_words_adj + j] = (token_t) gold_buff_0[j];
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
			init_buf(mem, gold, 0);

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
for (int i = 0; i < 2; i++) {
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
                        init_buf(mem, gold, 1);
}
			/* Validation */
/*
			errors = validate_buf(&mem[out_offset], gold);
			if (errors)
				printf("  ... FAIL\n");
			else
				printf("  ... PASS\n");
*/
		}
		aligned_free(ptable);
		aligned_free(mem);
		aligned_free(gold);
	}

	return 0;
}
