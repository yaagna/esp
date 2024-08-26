// Copyright (c) 2011-2024 Columbia University, System Level Design Group
// SPDX-License-Identifier: Apache-2.0
#include "libesp.h"
#include "cfg_glob.h"
#include "fuseml_glob.h"


// Added
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

static unsigned in_words_adj;
static unsigned out_words_adj;
static unsigned in_len;
static unsigned out_len;
static unsigned in_size;
static unsigned out_size;
static unsigned out_offset;
static unsigned size;


/////// Added Functions and generate code ///////////
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
//#include <string.h>

static void init_buffer(int32_t in_buffs_8x8[24], token_t *in)
{
	int i;
	int j;

	for (i = 0; i < 1; i++)
		for (j = 0; j < dma_word_reg2; j++)
			//in[i * in_words_adj + j] = (token_t) j;
                        in[i * in_words_adj + j] = (token_t) in_buffs_8x8[j];

	//for (i = 0; i < 1; i++)
		//for (j = 0; j < 2 * reg2; j++)
          //      for (j = 0; j < reg3; j++)      //gold is initialized using reg3
			//gold[i * out_words_adj + j] = (token_t) j;
                //        gold[i * out_words_adj + j] = (token_t) gold_buff_8x8[j];
}

static void init_parameters()
{
	if (DMA_WORD_PER_BEAT(sizeof(token_t)) == 0) {
		in_words_adj = dma_word_reg2;
		//out_words_adj = 2 * reg2;
                out_words_adj = dma_word_reg3;
	} else {
		in_words_adj = round_up(dma_word_reg2, DMA_WORD_PER_BEAT(sizeof(token_t)));
		//out_words_adj = round_up(reg2, DMA_WORD_PER_BEAT(sizeof(token_t)));
                out_words_adj = round_up(dma_word_reg3, DMA_WORD_PER_BEAT(sizeof(token_t)));
	}
	in_len = in_words_adj * (1);
	out_len =  out_words_adj * (1);
	in_size = in_len * sizeof(token_t);
	out_size = out_len * sizeof(token_t);
	out_offset = in_len;
	size = (out_offset * sizeof(token_t)) + out_size;
}

static void copy_result (token_t *out, int gold[4]) {

	for (int i = 0; i < 1; i++)
		for (int j = 0; j < dma_word_reg3; j++) { //reg3 is the output index
			gold[j] = out[i * out_words_adj + j];
		}
}

void mult (int matrix_size, int conf, int char_weight_size, int char_input_size, int weight_matrix[matrix_size][matrix_size], int input_matrix[matrix_size][matrix_size], int result_matrix[matrix_size][matrix_size]) {

    if (conf == 0 || conf == 4) {           // int2x2

        /////////////////// Added Part ///////////////////

            // For Weight matrix
            char output_weight_matrix[matrix_size][matrix_size][char_weight_size];

            // For Input matrix
            int transpose_input_matrix[matrix_size][matrix_size];
            char output_input_matrix[matrix_size][matrix_size][char_input_size];

            // Generate the random matrix
            //generate_random_matrix(matrix_size, weight_matrix);
            //generate_random_matrix(matrix_size, input_matrix);
            int golden_matrix[matrix_size][matrix_size];

            // Do transpose of input matrix
            transpose(matrix_size, input_matrix, transpose_input_matrix);

            // Generate Golden Output matrix
            matrix_mult(matrix_size, weight_matrix, input_matrix, golden_matrix);

            // Print the random matrix and golden matrix
            //printf("Weight Matrix is: \n");
            //print_matrix(weight_matrix);
            //printf("\n Input Matrix is: \n");
            //print_matrix(input_matrix);
            //printf("\n Transpose Input Matrix is: \n");
            //print_matrix(transpose_input_matrix);
            //printf("\n Golden Matrix is: \n");
            //print_matrix(golden_matrix);

            // Convert the weight matrix to binary
            //printf("\n Output Matrix is: \n");
            weight_matrix_d2b(matrix_size, conf, char_weight_size, weight_matrix, output_weight_matrix);
            //print_matrix_b(output_weight_matrix);

            // Convert the input matrix to binary
            //printf("\n Output Matrix is: \n");
            input_matrix_d2b(matrix_size, conf, char_input_size, transpose_input_matrix, output_input_matrix);
            //print_matrix_b(output_weight_matrix);

            ///////////////////// Weight_0 /////////////////////
            char weight_input_0[16][32] = {0};
            weight_0_2x2(output_weight_matrix[0], weight_input_0[0]);
            weight_0_2x2(output_weight_matrix[2], weight_input_0[1]);
            weight_0_2x2(output_weight_matrix[4], weight_input_0[2]);
            weight_0_2x2(output_weight_matrix[6], weight_input_0[3]);
            weight_0_2x2(output_weight_matrix[0], weight_input_0[0]);
            weight_0_2x2(output_weight_matrix[8], weight_input_0[4]);
            weight_0_2x2(output_weight_matrix[10], weight_input_0[5]);
            weight_0_2x2(output_weight_matrix[12], weight_input_0[6]);
            weight_0_2x2(output_weight_matrix[14], weight_input_0[7]);
            weight_0_2x2(output_weight_matrix[16], weight_input_0[8]);
            weight_0_2x2(output_weight_matrix[18], weight_input_0[9]);
            weight_0_2x2(output_weight_matrix[20], weight_input_0[10]);
            weight_0_2x2(output_weight_matrix[22], weight_input_0[11]);
            weight_0_2x2(output_weight_matrix[24], weight_input_0[12]);
            weight_0_2x2(output_weight_matrix[26], weight_input_0[13]);
            weight_0_2x2(output_weight_matrix[28], weight_input_0[14]);
            weight_0_2x2(output_weight_matrix[30], weight_input_0[15]);

        /*
            printf("\n Weight_0 is: \n");
            for (int i = 0; i < 16; i++){
                for (int j = 0; j < 32; j++){
                    printf("%d", weight_input_0[i][j]);
                }
                printf("\n");
            }
        */

            // Convert weight_input_0 to hexadecimal
            unsigned int weight_hexadecimal_input_0[16] = {0};
            for (int i = 0; i < 16; i++){
                for (int j = 0; j < 32; j++){
                    weight_hexadecimal_input_0[i] += weight_input_0[i][j] * pow(2, 31-j);
                }
            }

        /*
            printf("\n Weight_0 in hexadecimal is: \n");
            for (int i = 0; i < 16; i++){
                printf("%x ", weight_hexadecimal_input_0[i]);
            }
        */

            ///////////////////// wbuff_0 /////////////////////
            unsigned int wbuff_0[514] = {0};
            for (int i = 0; i < 16; i++){
                for (int j = 0; j < 32; j++){
                    if (i == 0){
                        wbuff_0[j] = weight_hexadecimal_input_0[i];
                    }
                    else if (i == 1){
                        wbuff_0[j + 32] = weight_hexadecimal_input_0[i];
                    }
                    else if (i == 2){
                        wbuff_0[j + 64] = weight_hexadecimal_input_0[i];
                    }
                    else if (i == 3){
                        wbuff_0[j + 96] = weight_hexadecimal_input_0[i];
                    }
                    else if (i == 4){
                        wbuff_0[j + 128] = weight_hexadecimal_input_0[i];
                    }
                    else if (i == 5){
                        wbuff_0[j + 160] = weight_hexadecimal_input_0[i];
                    }
                    else if (i == 6){
                        wbuff_0[j + 192] = weight_hexadecimal_input_0[i];
                    }
                    else if (i == 7){
                        wbuff_0[j + 224] = weight_hexadecimal_input_0[i];
                    }
                    else if (i == 8){
                        wbuff_0[j + 256] = weight_hexadecimal_input_0[i];
                    }
                    else if (i == 9){
                        wbuff_0[j + 288] = weight_hexadecimal_input_0[i];
                    }
                    else if (i == 10){
                        wbuff_0[j + 320] = weight_hexadecimal_input_0[i];
                    }
                    else if (i == 11){
                        wbuff_0[j + 352] = weight_hexadecimal_input_0[i];
                    }
                    else if (i == 12){
                        wbuff_0[j + 384] = weight_hexadecimal_input_0[i];
                    }
                    else if (i == 13){
                        wbuff_0[j + 416] = weight_hexadecimal_input_0[i];
                    }
                    else if (i == 14){
                        wbuff_0[j + 448] = weight_hexadecimal_input_0[i];
                    }
                    else if (i == 15){
                        wbuff_0[j + 480] = weight_hexadecimal_input_0[i];
                    
                    }
                    wbuff_0[512] = 0;
                    wbuff_0[513] = 0;
                }
            }

            // Print wbuff_0

            // printf(" \n WBUFF_0 is: \n");
            // for (int i = 0; i < 258; i++){
            //     printf("%x\n", wbuff_0[i]);
            // }

            ///////////////////// Weight_1 /////////////////////
            char weight_input_1[16][32] = {0};
            weight_0_2x2(output_weight_matrix[1], weight_input_1[0]);
            weight_0_2x2(output_weight_matrix[3], weight_input_1[1]);
            weight_0_2x2(output_weight_matrix[5], weight_input_1[2]);
            weight_0_2x2(output_weight_matrix[7], weight_input_1[3]);
            weight_0_2x2(output_weight_matrix[9], weight_input_1[4]);
            weight_0_2x2(output_weight_matrix[11], weight_input_1[5]);
            weight_0_2x2(output_weight_matrix[13], weight_input_1[6]);
            weight_0_2x2(output_weight_matrix[15], weight_input_1[7]);
            weight_0_2x2(output_weight_matrix[17], weight_input_1[8]);
            weight_0_2x2(output_weight_matrix[19], weight_input_1[9]);
            weight_0_2x2(output_weight_matrix[21], weight_input_1[10]);
            weight_0_2x2(output_weight_matrix[23], weight_input_1[11]);
            weight_0_2x2(output_weight_matrix[25], weight_input_1[12]);
            weight_0_2x2(output_weight_matrix[27], weight_input_1[13]);
            weight_0_2x2(output_weight_matrix[29], weight_input_1[14]);
            weight_0_2x2(output_weight_matrix[31], weight_input_1[15]);

        /*
            printf("\n Weight_1 is: \n");
            for (int i = 0; i < 16; i++){
                for (int j = 0; j < 32; j++){
                    printf("%d", weight_input_1[i][j]);
                }
                printf("\n");
            }
        */

            // Convert weight_input_1 to hexadecimal
            unsigned int weight_hexadecimal_input_1[16] = {0};
            for (int i = 0; i < 16; i++){
                for (int j = 0; j < 32; j++){
                    weight_hexadecimal_input_1[i] += weight_input_1[i][j] * pow(2, 31-j);
                }
            }

        /*
            printf("\n Weight_1 in hexadecimal is: \n");
            for (int i = 0; i < 16; i++){
                printf("%x ", weight_hexadecimal_input_1[i]);
            } 
        */

            ///////////////////// wbuff_1 /////////////////////
            unsigned int wbuff_1[514] = {0};
            for (int i = 0; i < 16; i++){
                for (int j = 0; j < 32; j++){
                    if (i == 0){
                        wbuff_1[j + 1] = weight_hexadecimal_input_1[i];
                    }
                    else if (i == 1){
                        wbuff_1[j + 33] = weight_hexadecimal_input_1[i];
                    }
                    else if (i == 2){
                        wbuff_1[j + 65] = weight_hexadecimal_input_1[i];
                    }
                    else if (i == 3){
                        wbuff_1[j + 97] = weight_hexadecimal_input_1[i];
                    }
                    else if (i == 4){
                        wbuff_1[j + 129] = weight_hexadecimal_input_1[i];
                    }
                    else if (i == 5){
                        wbuff_1[j + 161] = weight_hexadecimal_input_1[i];
                    }
                    else if (i == 6){
                        wbuff_1[j + 193] = weight_hexadecimal_input_1[i];
                    }
                    else if (i == 7){
                        wbuff_1[j + 225] = weight_hexadecimal_input_1[i];
                    }
                    else if (i == 8){
                        wbuff_1[j + 257] = weight_hexadecimal_input_1[i];
                    }
                    else if (i == 9){
                        wbuff_1[j + 289] = weight_hexadecimal_input_1[i];
                    }
                    else if (i == 10){
                        wbuff_1[j + 321] = weight_hexadecimal_input_1[i];
                    }
                    else if (i == 11){
                        wbuff_1[j + 353] = weight_hexadecimal_input_1[i];
                    }
                    else if (i == 12){
                        wbuff_1[j + 385] = weight_hexadecimal_input_1[i];
                    }
                    else if (i == 13){
                        wbuff_1[j + 417] = weight_hexadecimal_input_1[i];
                    }
                    else if (i == 14){
                        wbuff_1[j + 449] = weight_hexadecimal_input_1[i];
                    }
                    else if (i == 15){
                        wbuff_1[j + 481] = weight_hexadecimal_input_1[i];
                    
                    }
                    wbuff_1[0] = 0;
                    wbuff_1[513] = 0;
                }
            }

            // Print wbuff_1
            // for (int i = 0; i < 258; i++){
            //     printf("\n%x\n", wbuff_1[i]);
            // }

            ///////////////////// Weight_2 /////////////////////
            char weight_input_2[16][32] = {0};
            weight_2_2x2(output_weight_matrix[0], weight_input_2[0]);
            weight_2_2x2(output_weight_matrix[2], weight_input_2[1]);
            weight_2_2x2(output_weight_matrix[4], weight_input_2[2]);
            weight_2_2x2(output_weight_matrix[6], weight_input_2[3]);
            weight_2_2x2(output_weight_matrix[8], weight_input_2[4]);
            weight_2_2x2(output_weight_matrix[10], weight_input_2[5]);
            weight_2_2x2(output_weight_matrix[12], weight_input_2[6]);
            weight_2_2x2(output_weight_matrix[14], weight_input_2[7]);
            weight_2_2x2(output_weight_matrix[16], weight_input_2[8]);
            weight_2_2x2(output_weight_matrix[18], weight_input_2[9]);
            weight_2_2x2(output_weight_matrix[20], weight_input_2[10]);
            weight_2_2x2(output_weight_matrix[22], weight_input_2[11]);
            weight_2_2x2(output_weight_matrix[24], weight_input_2[12]);
            weight_2_2x2(output_weight_matrix[26], weight_input_2[13]);
            weight_2_2x2(output_weight_matrix[28], weight_input_2[14]);
            weight_2_2x2(output_weight_matrix[30], weight_input_2[15]);

        /*
            printf("\n Weight_2 is: \n");
            for (int i = 0; i < 16; i++){
                for (int j = 0; j < 32; j++){
                    printf("%d", weight_input_2[i][j]);
                }
                printf("\n");
            }
        */

            // Convert weight_input_2 to hexadecimal
            unsigned int weight_hexadecimal_input_2[16] = {0};
            for (int i = 0; i < 16; i++){
                for (int j = 0; j < 32; j++){
                    weight_hexadecimal_input_2[i] += weight_input_2[i][j] * pow(2, 31-j);
                }
            }

        /*
            printf("\n Weight_2 in hexadecimal is: \n");
            for (int i = 0; i < 16; i++){
                printf("%x ", weight_hexadecimal_input_2[i]);
            }
        */

            ///////////////////// wbuff_2 /////////////////////
            unsigned int wbuff_2[514] = {0};
            for (int i = 0; i < 16; i++){
                for (int j = 0; j < 32; j++){
                    if (i == 0){
                        wbuff_2[j + 1] = weight_hexadecimal_input_2[i];
                    }
                    else if (i == 1){
                        wbuff_2[j + 33] = weight_hexadecimal_input_2[i];
                    }
                    else if (i == 2){
                        wbuff_2[j + 65] = weight_hexadecimal_input_2[i];
                    }
                    else if (i == 3){
                        wbuff_2[j + 97] = weight_hexadecimal_input_2[i];
                    }
                    else if (i == 4){
                        wbuff_2[j + 129] = weight_hexadecimal_input_2[i];
                    }
                    else if (i == 5){
                        wbuff_2[j + 161] = weight_hexadecimal_input_2[i];
                    }
                    else if (i == 6){
                        wbuff_2[j + 193] = weight_hexadecimal_input_2[i];
                    }
                    else if (i == 7){
                        wbuff_2[j + 225] = weight_hexadecimal_input_2[i];
                    }
                    else if (i == 8){
                        wbuff_2[j + 257] = weight_hexadecimal_input_2[i];
                    }
                    else if (i == 9){
                        wbuff_2[j + 289] = weight_hexadecimal_input_2[i];
                    }
                    else if (i == 10){
                        wbuff_2[j + 321] = weight_hexadecimal_input_2[i];
                    }
                    else if (i == 11){
                        wbuff_2[j + 353] = weight_hexadecimal_input_2[i];
                    }
                    else if (i == 12){
                        wbuff_2[j + 385] = weight_hexadecimal_input_2[i];
                    }
                    else if (i == 13){
                        wbuff_2[j + 417] = weight_hexadecimal_input_2[i];
                    }
                    else if (i == 14){
                        wbuff_2[j + 449] = weight_hexadecimal_input_2[i];
                    }
                    else if (i == 15){
                        wbuff_2[j + 481] = weight_hexadecimal_input_2[i];
                    
                    }
                    wbuff_2[0] = 0;
                    wbuff_2[513] = 0;
                }
            }

            // Print wbuff_2
            // for (int i = 0; i < 258; i++){
            //     printf("\n%x\n", wbuff_2[i]);
            // }

            ///////////////////// Weight_3 /////////////////////
            char weight_input_3[16][32] = {0};
            weight_2_2x2(output_weight_matrix[1], weight_input_3[0]);
            weight_2_2x2(output_weight_matrix[3], weight_input_3[1]);
            weight_2_2x2(output_weight_matrix[5], weight_input_3[2]);
            weight_2_2x2(output_weight_matrix[7], weight_input_3[3]);
            weight_2_2x2(output_weight_matrix[9], weight_input_3[4]);
            weight_2_2x2(output_weight_matrix[11], weight_input_3[5]);
            weight_2_2x2(output_weight_matrix[13], weight_input_3[6]);
            weight_2_2x2(output_weight_matrix[15], weight_input_3[7]);
            weight_2_2x2(output_weight_matrix[17], weight_input_3[8]);
            weight_2_2x2(output_weight_matrix[19], weight_input_3[9]);
            weight_2_2x2(output_weight_matrix[21], weight_input_3[10]);
            weight_2_2x2(output_weight_matrix[23], weight_input_3[11]);
            weight_2_2x2(output_weight_matrix[25], weight_input_3[12]);
            weight_2_2x2(output_weight_matrix[27], weight_input_3[13]);
            weight_2_2x2(output_weight_matrix[29], weight_input_3[14]);
            weight_2_2x2(output_weight_matrix[31], weight_input_3[15]);

        /*
            printf("\n Weight_3 is: \n");
            for (int i = 0; i < 16; i++){
                for (int j = 0; j < 32; j++){
                    printf("%d", weight_input_3[i][j]);
                }
                printf("\n");
            }
        */

            // Convert weight_input_3 to hexadecimal
            unsigned int weight_hexadecimal_input_3[16] = {0};
            for (int i = 0; i < 16; i++){
                for (int j = 0; j < 32; j++){
                    weight_hexadecimal_input_3[i] += weight_input_3[i][j] * pow(2, 31-j);
                }
            }

        /*
            printf("\n Weight_3 in hexadecimal is: \n");
            for (int i = 0; i < 16; i++){
                printf("%x ", weight_hexadecimal_input_3[i]);
            }
        */

            ///////////////////// wbuff_3 /////////////////////
            unsigned int wbuff_3[514] = {0};
                for (int i = 0; i < 16; i++){
                for (int j = 0; j < 32; j++){
                    if (i == 0){
                        wbuff_3[j + 2] = weight_hexadecimal_input_3[i];
                    }
                    else if (i == 1){
                        wbuff_3[j + 34] = weight_hexadecimal_input_3[i];
                    }
                    else if (i == 2){
                        wbuff_3[j + 66] = weight_hexadecimal_input_3[i];
                    }
                    else if (i == 3){
                        wbuff_3[j + 98] = weight_hexadecimal_input_3[i];
                    }
                    else if (i == 4){
                        wbuff_3[j + 130] = weight_hexadecimal_input_3[i];
                    }
                    else if (i == 5){
                        wbuff_3[j + 162] = weight_hexadecimal_input_3[i];
                    }
                    else if (i == 6){
                        wbuff_3[j + 194] = weight_hexadecimal_input_3[i];
                    }
                    else if (i == 7){
                        wbuff_3[j + 226] = weight_hexadecimal_input_3[i];
                    }
                    else if (i == 8){
                        wbuff_3[j + 258] = weight_hexadecimal_input_3[i];
                    }
                    else if (i == 9){
                        wbuff_3[j + 290] = weight_hexadecimal_input_3[i];
                    }
                    else if (i == 10){
                        wbuff_3[j + 322] = weight_hexadecimal_input_3[i];
                    }
                    else if (i == 11){
                        wbuff_3[j + 354] = weight_hexadecimal_input_3[i];
                    }
                    else if (i == 12){
                        wbuff_3[j + 386] = weight_hexadecimal_input_3[i];
                    }
                    else if (i == 13){
                        wbuff_3[j + 418] = weight_hexadecimal_input_3[i];
                    }
                    else if (i == 14){
                        wbuff_3[j + 450] = weight_hexadecimal_input_3[i];
                    }
                    else if (i == 15){
                        wbuff_3[j + 482] = weight_hexadecimal_input_3[i];
                    }
                    wbuff_3[0] = 0;
                    wbuff_3[1] = 0;
                }
            }

            // Print wbuff_3
            // for (int i = 0; i < 258; i++){
            //     printf("\n%x\n", wbuff_3[i]);
            // }

            ///////////////////// Input_0 /////////////////////
            char input_0[32][32] = {0};
            weight_0_2x2(output_input_matrix[0], input_0[0]);
            weight_0_2x2(output_input_matrix[1], input_0[1]);
            weight_0_2x2(output_input_matrix[2], input_0[2]);
            weight_0_2x2(output_input_matrix[3], input_0[3]);
            weight_0_2x2(output_input_matrix[4], input_0[4]);
            weight_0_2x2(output_input_matrix[5], input_0[5]);
            weight_0_2x2(output_input_matrix[6], input_0[6]);
            weight_0_2x2(output_input_matrix[7], input_0[7]);
            weight_0_2x2(output_input_matrix[8], input_0[8]);
            weight_0_2x2(output_input_matrix[9], input_0[9]);
            weight_0_2x2(output_input_matrix[10], input_0[10]);
            weight_0_2x2(output_input_matrix[11], input_0[11]);
            weight_0_2x2(output_input_matrix[12], input_0[12]);
            weight_0_2x2(output_input_matrix[13], input_0[13]);
            weight_0_2x2(output_input_matrix[14], input_0[14]);
            weight_0_2x2(output_input_matrix[15], input_0[15]);
            weight_0_2x2(output_input_matrix[16], input_0[16]);
            weight_0_2x2(output_input_matrix[17], input_0[17]);
            weight_0_2x2(output_input_matrix[18], input_0[18]);
            weight_0_2x2(output_input_matrix[19], input_0[19]);
            weight_0_2x2(output_input_matrix[20], input_0[20]);
            weight_0_2x2(output_input_matrix[21], input_0[21]);
            weight_0_2x2(output_input_matrix[22], input_0[22]);
            weight_0_2x2(output_input_matrix[23], input_0[23]);
            weight_0_2x2(output_input_matrix[24], input_0[24]);
            weight_0_2x2(output_input_matrix[25], input_0[25]);
            weight_0_2x2(output_input_matrix[26], input_0[26]);
            weight_0_2x2(output_input_matrix[27], input_0[27]);
            weight_0_2x2(output_input_matrix[28], input_0[28]);
            weight_0_2x2(output_input_matrix[29], input_0[29]);
            weight_0_2x2(output_input_matrix[30], input_0[30]);
            weight_0_2x2(output_input_matrix[31], input_0[31]);

        /*
            printf("\n Input_0 is: \n");
            for (int i = 0; i < 32; i++){
                for (int j = 0; j < 32; j++){
                    printf("%d", input_0[i][j]);
                }
                printf("\n");
            }
        */

            // Convert input_0 to hexadecimal
            unsigned int input_hexadecimal_input_0[32] = {0};
            for (int i = 0; i < 32; i++){
                for (int j = 0; j < 32; j++){
                    input_hexadecimal_input_0[i] += input_0[i][j] * pow(2, 31-j);
                }
            }

        /*
            printf("\n Input_0 in hexadecimal is: \n");
            for (int i = 0; i < 32; i++){
                printf("%x ", input_hexadecimal_input_0[i]);
            }
        */

            ///////////////////// ibuff_0 /////////////////////
            unsigned int ibuff_0[514] = {0};
            for (int i = 0; i < 16; i++){
                for (int j = 0; j < 32; j++){
                    if(i == 0){
                        ibuff_0[j] = input_hexadecimal_input_0[j];
                    }
                    else if(i == 1){
                        ibuff_0[j + 32] = input_hexadecimal_input_0[j];
                    }
                    else if(i == 2){
                        ibuff_0[j + 64] = input_hexadecimal_input_0[j];
                    }
                    else if(i == 3){
                        ibuff_0[j + 96] = input_hexadecimal_input_0[j];
                    }
                    else if(i == 4){
                        ibuff_0[j + 128] = input_hexadecimal_input_0[j];
                    }
                    else if(i == 5){
                        ibuff_0[j + 160] = input_hexadecimal_input_0[j];
                    }
                    else if(i == 6){
                        ibuff_0[j + 192] = input_hexadecimal_input_0[j];
                    }
                    else if(i == 7){
                        ibuff_0[j + 224] = input_hexadecimal_input_0[j];
                    }
                    else if (i == 8){
                        ibuff_0[j + 256] = input_hexadecimal_input_0[j];
                    }
                    else if (i == 9){
                        ibuff_0[j + 288] = input_hexadecimal_input_0[j];
                    }
                    else if (i == 10){
                        ibuff_0[j + 320] = input_hexadecimal_input_0[j];
                    }
                    else if (i == 11){
                        ibuff_0[j + 352] = input_hexadecimal_input_0[j];
                    }
                    else if (i == 12){
                        ibuff_0[j + 384] = input_hexadecimal_input_0[j];
                    }
                    else if (i == 13){
                        ibuff_0[j + 416] = input_hexadecimal_input_0[j];
                    }
                    else if (i == 14){
                        ibuff_0[j + 448] = input_hexadecimal_input_0[j];
                    }
                    else if (i == 15){
                        ibuff_0[j + 480] = input_hexadecimal_input_0[j];
                    }
                    ibuff_0[512] = 0;
                    ibuff_0[513] = 0;
                }
            }  

            ///////////////////// Input_1 /////////////////////
            char input_1[32][32] = {0};
            weight_2_2x2(output_input_matrix[0], input_1[0]);
            weight_2_2x2(output_input_matrix[1], input_1[1]);
            weight_2_2x2(output_input_matrix[2], input_1[2]);
            weight_2_2x2(output_input_matrix[3], input_1[3]);
            weight_2_2x2(output_input_matrix[4], input_1[4]);
            weight_2_2x2(output_input_matrix[5], input_1[5]);
            weight_2_2x2(output_input_matrix[6], input_1[6]);
            weight_2_2x2(output_input_matrix[7], input_1[7]);
            weight_2_2x2(output_input_matrix[8], input_1[8]);
            weight_2_2x2(output_input_matrix[9], input_1[9]);
            weight_2_2x2(output_input_matrix[10], input_1[10]);
            weight_2_2x2(output_input_matrix[11], input_1[11]);
            weight_2_2x2(output_input_matrix[12], input_1[12]);
            weight_2_2x2(output_input_matrix[13], input_1[13]);
            weight_2_2x2(output_input_matrix[14], input_1[14]);
            weight_2_2x2(output_input_matrix[15], input_1[15]);
            weight_2_2x2(output_input_matrix[16], input_1[16]);
            weight_2_2x2(output_input_matrix[17], input_1[17]);
            weight_2_2x2(output_input_matrix[18], input_1[18]);
            weight_2_2x2(output_input_matrix[19], input_1[19]);
            weight_2_2x2(output_input_matrix[20], input_1[20]);
            weight_2_2x2(output_input_matrix[21], input_1[21]);
            weight_2_2x2(output_input_matrix[22], input_1[22]);
            weight_2_2x2(output_input_matrix[23], input_1[23]);
            weight_2_2x2(output_input_matrix[24], input_1[24]);
            weight_2_2x2(output_input_matrix[25], input_1[25]);
            weight_2_2x2(output_input_matrix[26], input_1[26]);
            weight_2_2x2(output_input_matrix[27], input_1[27]);
            weight_2_2x2(output_input_matrix[28], input_1[28]);
            weight_2_2x2(output_input_matrix[29], input_1[29]);
            weight_2_2x2(output_input_matrix[30], input_1[30]);
            weight_2_2x2(output_input_matrix[31], input_1[31]);

        /*
            printf("\n Input_1 is: \n");
            for (int i = 0; i < 32; i++){
                for (int j = 0; j < 32; j++){
                    printf("%d", input_1[i][j]);
                }
                printf("\n");
            }
        */

            // Convert input_1 to hexadecimal
            unsigned int input_hexadecimal_input_1[32] = {0};
            for (int i = 0; i < 32; i++){
                for (int j = 0; j < 32; j++){
                    input_hexadecimal_input_1[i] += input_1[i][j] * pow(2, 31-j);
                }
            }

        /*
            printf("\n Input_1 in hexadecimal is: \n");
            for (int i = 0; i < 32; i++){
                printf("%x ", input_hexadecimal_input_1[i]);
            }
        */

            ///////////////////// ibuff_1 /////////////////////
            unsigned int ibuff_1[514] = {0};
            for (int i = 0; i < 16; i++){
                for (int j = 0; j < 32; j++){
                    if (i == 0){
                        ibuff_1[j + 1] = input_hexadecimal_input_1[j];
                    }
                    else if (i == 1){
                        ibuff_1[j + 33] = input_hexadecimal_input_1[j];
                    }
                    else if (i == 2){
                        ibuff_1[j + 65] = input_hexadecimal_input_1[j];
                    }
                    else if (i == 3){
                        ibuff_1[j + 97] = input_hexadecimal_input_1[j];
                    }
                    else if (i == 4){
                        ibuff_1[j + 129] = input_hexadecimal_input_1[j];
                    }
                    else if (i == 5){
                        ibuff_1[j + 161] = input_hexadecimal_input_1[j];
                    }
                    else if (i == 6){
                        ibuff_1[j + 193] = input_hexadecimal_input_1[j];
                    }
                    else if (i == 7){
                        ibuff_1[j + 225] = input_hexadecimal_input_1[j];
                    }
                    else if (i == 8){
                        ibuff_1[j + 257] = input_hexadecimal_input_1[j];
                    }
                    else if (i == 9){
                        ibuff_1[j + 289] = input_hexadecimal_input_1[j];
                    }
                    else if (i == 10){
                        ibuff_1[j + 321] = input_hexadecimal_input_1[j];
                    }
                    else if (i == 11){
                        ibuff_1[j + 353] = input_hexadecimal_input_1[j];
                    }
                    else if (i == 12){
                        ibuff_1[j + 385] = input_hexadecimal_input_1[j];
                    }
                    else if (i == 13){
                        ibuff_1[j + 417] = input_hexadecimal_input_1[j];
                    }
                    else if (i == 14){
                        ibuff_1[j + 449] = input_hexadecimal_input_1[j];
                    }
                    else if (i == 15){
                        ibuff_1[j + 481] = input_hexadecimal_input_1[j];
                    }
                    ibuff_1[0] = 0;
                    ibuff_1[513] = 0;
                }
            }

            ///////////////////// Input Stream //////////////////////
            unsigned int input_vector_stream[3084] = {0};
            //for (int j = 0; j < 3; j++){
            //  if (j == 0){
                    for (int i = 0; i < 514; i++){
                        input_vector_stream[2*i] = ibuff_0[i];
                        input_vector_stream[2*i + 1] =ibuff_1[i];
                    }
                //}
                //if (j == 1){
                    for (int i = 0; i < 514; i++){
                        input_vector_stream[1028 + 2*i] = wbuff_0[i];
                        input_vector_stream[1028 + 2*i + 1] = wbuff_1[i];
                    }
                //}
                //if (j == 2){
                    for (int i = 0; i < 514; i++){
                        input_vector_stream[2056 + 2*i] = wbuff_2[i];
                        input_vector_stream[2056 + 2*i + 1] = wbuff_3[i];
                    }
                //}
            //}

    static int32_t in_buffs_2x2[3084] = {0};
    //static int32_t gold_buff_2x2[1024];
    for (int i = 0; i < 3084; i++){
        in_buffs_2x2[i] = input_vector_stream[i];
    }

    token_t *gold;
    token_t *buf;

    init_parameters();

    buf = (token_t *) esp_alloc(size);
    cfg_100[0].hw_buf = buf;

    gold = malloc(out_size);

    init_buffer(in_buffs_2x2, buf);
    esp_run(cfg_100, NACC);

    //validate_buffer(&buf[out_offset], each_run);

    int each_run [1024] = {0};
    copy_result(&buf[out_offset], each_run);

    for (int i = 0; i < matrix_size; i++){
       for (int j = 0; j < matrix_size; j++){
           if (i == 0){
                 result_matrix[i][j] = each_run[2*j];
             }
             else if (i == 1){
                 result_matrix[i][j] = each_run[2*j + 1];
             }
             else if (i == 2){
                 result_matrix[i][j] = each_run[2*j + 64];
             }
             else if (i == 3){
                 result_matrix[i][j] = each_run[2*j + 65];
             }
             else if (i == 4){
                 result_matrix[i][j] = each_run[2*j + 128];
             }
             else if (i == 5){
                 result_matrix[i][j] = each_run[2*j + 129];
             }
             else if (i == 6){
                 result_matrix[i][j] = each_run[2*j + 192];
             }
             else if (i == 7){
                 result_matrix[i][j] = each_run[2*j + 193];
             }
             else if (i == 8){
                 result_matrix[i][j] = each_run[2*j + 256];
             }
             else if (i == 9){
                 result_matrix[i][j] = each_run[2*j + 257];
             }
             else if (i == 10){
                 result_matrix[i][j] = each_run[2*j + 320];
             }
             else if (i == 11){
                 result_matrix[i][j] = each_run[2*j + 321];
             }
             else if (i == 12){
                 result_matrix[i][j] = each_run[2*j + 384];
             }
             else if (i == 13){
                 result_matrix[i][j] = each_run[2*j + 385];
             }
             else if (i == 14){
                 result_matrix[i][j] = each_run[2*j + 448];
             }
             else if (i == 15){
                 result_matrix[i][j] = each_run[2*j + 449];
             }
             else if (i == 16){
                 result_matrix[i][j] = each_run[2*j + 512];
             }
             else if (i == 17){
                 result_matrix[i][j] = each_run[2*j + 513];
             }
             else if (i == 18){
                 result_matrix[i][j] = each_run[2*j + 576];
             }
             else if (i == 19){
                 result_matrix[i][j] = each_run[2*j + 577];
             }
             else if (i == 20){
                 result_matrix[i][j] = each_run[2*j + 640];
             }
             else if (i == 21){
                 result_matrix[i][j] = each_run[2*j + 641];
             }
             else if (i == 22){
                 result_matrix[i][j] = each_run[2*j + 704];
             }
             else if (i == 23){
                 result_matrix[i][j] = each_run[2*j + 705];
             }
             else if (i == 24){
                 result_matrix[i][j] = each_run[2*j + 768];
             }
             else if (i == 25){
                 result_matrix[i][j] = each_run[2*j + 769];
             }
             else if (i == 26){
                 result_matrix[i][j] = each_run[2*j + 832];
             }
             else if (i == 27){
                 result_matrix[i][j] = each_run[2*j + 833];
             }
             else if (i == 28){
                 result_matrix[i][j] = each_run[2*j + 896];
             }
             else if (i == 29){
                 result_matrix[i][j] = each_run[2*j + 897];
             }
             else if (i == 30){
                 result_matrix[i][j] = each_run[2*j + 960];
             }
             else if (i == 31){
                 result_matrix[i][j] = each_run[2*j + 961];
             }
         }
     }

     //print_matrix(matrix_size, result_matrix);

     for (int i = 0; i < 1024; i++){
	each_run[i] = 0;
     }


    }

    else if (conf == 1 || conf == 5) {       // int4x4

        ///////// Added ///////////// 
            int golden_matrix[matrix_size][matrix_size];

            // For Weight matrix
            char output_weight_matrix[matrix_size][matrix_size][4];
            char transformed_weight_matrix[matrix_size][matrix_size][8];

            // For Input matrix
            int transpose_input_matrix[matrix_size][matrix_size];
            char output_input_matrix[matrix_size][matrix_size][4];
            char transformed_input_matrix[matrix_size][matrix_size][8];

            //print_matrix(matrix_size, weight_matrix);
            //print_matrix(matrix_size, input_matrix);


            // Do transpose of input matrix
            transpose(matrix_size, input_matrix, transpose_input_matrix);

            // Generate Golden Output matrix
            matrix_mult(matrix_size, weight_matrix, input_matrix, golden_matrix);
                

            weight_matrix_d2b(matrix_size, conf, char_weight_size, weight_matrix, output_weight_matrix);

            // Transform the weight matrix
            for (int i = 0; i < matrix_size; i++){
                for (int j = 0; j < matrix_size; j++){
                    weight_transform_4x4(output_weight_matrix[i][j], transformed_weight_matrix[i][j]);
                }
            }

            //print_transformed_matrix_b(matrix_size, transformed_weight_matrix);

            input_matrix_d2b(matrix_size, conf, char_input_size, transpose_input_matrix, output_input_matrix);

            // Transform the input matrix
            for (int i = 0; i < matrix_size; i++){
                for (int j = 0; j < matrix_size; j++){
                    input_transform_4x4(output_input_matrix[i][j], transformed_input_matrix[i][j]);
                }
            }

            ////////////////////////////// Weight_0 //////////////////////////////
            char weight_input_0[4][32] = {0};
            weight_0_4x4(transformed_weight_matrix[0], weight_input_0[0]);
            weight_0_4x4(transformed_weight_matrix[2], weight_input_0[1]);
            weight_0_4x4(transformed_weight_matrix[4], weight_input_0[2]);
            weight_0_4x4(transformed_weight_matrix[6], weight_input_0[3]);

        /*
            printf("Weight_input 0 \n");
            for (int i = 0; i < 32; i++){
            printf("%d ", weight_input_0[0][i]);
            }
        */

            // Convert the weight_input_0 to hexadecimal note the binary is not in 2's complement
            unsigned int weight_hexadecimal_input_0[4] = {0};
            for (int i = 0; i < 4; i++){
                weight_hexadecimal_input_0[i] = 0;
                for (int j = 0; j < 32; j++){
                    weight_hexadecimal_input_0[i] += weight_input_0[i][j] * pow(2, 31-j);
                }
            }

        /*
            printf("Weight_input 0 \n");
            for (int i = 0; i < 4; i++){
            printf("%x ", weight_hexadecimal_input_0[i]);
            }
        */

            ////////////////////////////// WBUFF_0 //////////////////////////////
            // This function will take weight_hexadecimal_input_0 and then make it such that it can be fed into the WBUFF_0 directly
            /*
                For WBUFF_0 each entry in weight_hexadecimal_input_0 will be copied for 7 times and last two entries will be padded with 0
            */
            int wbuff_0[34] = {0};
            for (int i = 0; i < 4; i++){
                for (int j = 0; j < 8; j++){
                    if(i == 0){
                        wbuff_0[j] = weight_hexadecimal_input_0[i]; 
                    }
                    else if (i == 1){
                        wbuff_0[j + 8] = weight_hexadecimal_input_0[i];
                    }
                    else if(i == 2){
                        wbuff_0[j + 16] = weight_hexadecimal_input_0[i];
                    }
                    else if(i == 3){
                        wbuff_0[j + 24] = weight_hexadecimal_input_0[i];
                    }
                    wbuff_0[32] = 0;
                    wbuff_0[33] = 0;           
                }
            }

            ////////////////////////////// Weight_2 //////////////////////////////
            char weight_input_2[4][32] = {0};
            weight_2_4x4(transformed_weight_matrix[0], weight_input_2[0]);
            weight_2_4x4(transformed_weight_matrix[2], weight_input_2[1]);
            weight_2_4x4(transformed_weight_matrix[4], weight_input_2[2]);
            weight_2_4x4(transformed_weight_matrix[6], weight_input_2[3]);

            // Convert the weight_input_2 to decimal note the binary is not in 2's complement
            unsigned int weight_hexadecimal_input_2[4];
            for (int i = 0; i < 4; i++){
                weight_hexadecimal_input_2[i] = 0;
                for (int j = 0; j < 32; j++){
                    weight_hexadecimal_input_2[i] += weight_input_2[i][j] * pow(2, 31-j);
                }
            }

            ////////////////////////////// WBUFF_2 //////////////////////////////
            // This function will take weight_hexadecimal_input_2 and then make it such that it can be fed into the WBUFF_2 directly
            /*
                For WBUFF_0 each entry in weight_hexadecimal_input_2 will be copied for 7 times and first and last entries will be padded with 0
            */
            int wbuff_2[34] = {0};
            for (int i = 0; i < 4; i++){
                for (int j = 0; j < 8; j++){
                    if(i == 0){
                        wbuff_2[j + 1] = weight_hexadecimal_input_2[i]; 
                    }
                    else if (i == 1){
                        wbuff_2[j + 9] = weight_hexadecimal_input_2[i];
                    }
                    else if(i == 2){
                        wbuff_2[j + 17] = weight_hexadecimal_input_2[i];
                    }
                    else if(i == 3){
                        wbuff_2[j + 25] = weight_hexadecimal_input_2[i];
                    }
                    wbuff_2[0] = 0;
                    wbuff_2[33] = 0;           
                }
            }

            ////////////////////////////// Weight_1 //////////////////////////////
            char weight_input_1[4][32] = {0};
            weight_0_4x4(transformed_weight_matrix[1], weight_input_1[0]);
            weight_0_4x4(transformed_weight_matrix[3], weight_input_1[1]);
            weight_0_4x4(transformed_weight_matrix[5], weight_input_1[2]);
            weight_0_4x4(transformed_weight_matrix[7], weight_input_1[3]);

            // Convert the weight_input_1 to hexadecimal note the binary is not in 2's complement
            unsigned int weight_hexadecimal_input_1[4];
            for (int i = 0; i < 4; i++){
                weight_hexadecimal_input_1[i] = 0;
                for (int j = 0; j < 32; j++){
                    weight_hexadecimal_input_1[i] += weight_input_1[i][j] * pow(2, 31-j);
                }
            }

            ////////////////////////////// WBUFF_1 //////////////////////////////
            // This function will take weight_hexadecimal_input_1 and then make it such that it can be fed into the WBUFF_1 directly
            /*
                For WBUFF_0 each entry in weight_hexadecimal_input_1 will be copied for 7 times and first and last entries will be padded with 0
            */
            int wbuff_1[34] = {0};
            for (int i = 0; i < 4; i++){
                for (int j = 0; j < 8; j++){
                    if(i == 0){
                        wbuff_1[j + 1] = weight_hexadecimal_input_1[i]; 
                    }
                    else if (i == 1){
                        wbuff_1[j + 9] = weight_hexadecimal_input_1[i];
                    }
                    else if(i == 2){
                        wbuff_1[j + 17] = weight_hexadecimal_input_1[i];
                    }
                    else if(i == 3){
                        wbuff_1[j + 25] = weight_hexadecimal_input_1[i];
                    }
                    wbuff_1[0] = 0;
                    wbuff_1[33] = 0;           
                }
            }

            ////////////////////////////// Weight_3 //////////////////////////////
            char weight_input_3[4][32] = {0};
            weight_2_4x4(transformed_weight_matrix[1], weight_input_3[0]);
            weight_2_4x4(transformed_weight_matrix[3], weight_input_3[1]);
            weight_2_4x4(transformed_weight_matrix[5], weight_input_3[2]);
            weight_2_4x4(transformed_weight_matrix[7], weight_input_3[3]);

            // Convert the weight_input_1 to hexadecimal note the binary is not in 2's complement
            unsigned int weight_hexadecimal_input_3[4];
            for (int i = 0; i < 4; i++){
                weight_hexadecimal_input_3[i] = 0;
                for (int j = 0; j < 32; j++){
                    weight_hexadecimal_input_3[i] += weight_input_3[i][j] * pow(2, 31-j);
                }
            }

            ////////////////////////////// WBUFF_3 //////////////////////////////
            // This function will take weight_hexadecimal_input_3 and then make it such that it can be fed into the WBUFF_3 directly
            /*
                For WBUFF_0 each entry in weight_hexadecimal_input_3 will be copied for 7 times and first two entries will be padded with 0
            */
            int wbuff_3[34] = {0};
            for (int i = 0; i < 4; i++){
                for (int j = 0; j < 8; j++){
                    if(i == 0){
                        wbuff_3[j + 2] = weight_hexadecimal_input_3[i]; 
                    }
                    else if (i == 1){
                        wbuff_3[j + 10] = weight_hexadecimal_input_3[i];
                    }
                    else if(i == 2){
                        wbuff_3[j + 18] = weight_hexadecimal_input_3[i];
                    }
                    else if(i == 3){
                        wbuff_3[j + 26] = weight_hexadecimal_input_3[i];
                    }
                    wbuff_3[0] = 0;
                    wbuff_3[1] = 0;           
                }
            }

            ////////////////////////////// Input_0 //////////////////////////////
            char input_0[8][32] = {0};
            weight_0_4x4(transformed_input_matrix[0], input_0[0]);
            weight_0_4x4(transformed_input_matrix[1], input_0[1]);
            weight_0_4x4(transformed_input_matrix[2], input_0[2]);
            weight_0_4x4(transformed_input_matrix[3], input_0[3]);
            weight_0_4x4(transformed_input_matrix[4], input_0[4]);
            weight_0_4x4(transformed_input_matrix[5], input_0[5]);
            weight_0_4x4(transformed_input_matrix[6], input_0[6]);
            weight_0_4x4(transformed_input_matrix[7], input_0[7]);

            // Convert the input_input_0 to hexadecimal note the binary is not in 2's complement
            unsigned int input_hexadecimal_input_0[8];
            for (int i = 0; i < 8; i++){
                input_hexadecimal_input_0[i] = 0;
                for (int j = 0; j < 32; j++){
                    input_hexadecimal_input_0[i] += input_0[i][j] * pow(2, 31-j);
                }
            }

            ////////////////////////////// IBUFF_0 //////////////////////////////
            // This function will take input_hexadecimal_input_0 and then make it such that it can be fed into the IBUFF_0 directly
            /*
                For IBUFF_0 all 8 entries are repated 4 times and last 2 entries are padded with 0
            */
        int ibuff_0[34] = {0};
        for (int i = 0; i < 8; i++){
                for (int j = 0; j < 8; j++){
                if (i == 0) {
                    ibuff_0[j] = input_hexadecimal_input_0[j];
                }
                else if (i == 1){
                    ibuff_0[j + 8] = input_hexadecimal_input_0[j];
                }
                else if (i == 2){
                    ibuff_0[j + 16] = input_hexadecimal_input_0[j];
                }
                else if (i == 3){
                    ibuff_0[j + 24] = input_hexadecimal_input_0[j];
                }
                }
                ibuff_0[32] = 0;
                ibuff_0[33] = 0;
        }

            ////////////////////////////// Input_1 //////////////////////////////
            char input_1[8][32] = {0};
            weight_2_4x4(transformed_input_matrix[0], input_1[0]);
            weight_2_4x4(transformed_input_matrix[1], input_1[1]);
            weight_2_4x4(transformed_input_matrix[2], input_1[2]);
            weight_2_4x4(transformed_input_matrix[3], input_1[3]);
            weight_2_4x4(transformed_input_matrix[4], input_1[4]);
            weight_2_4x4(transformed_input_matrix[5], input_1[5]);
            weight_2_4x4(transformed_input_matrix[6], input_1[6]);
            weight_2_4x4(transformed_input_matrix[7], input_1[7]);

            // Convert the input_input_1 to hexadecimal note the binary is not in 2's complement
            unsigned int input_hexadecimal_input_1[8];
            for (int i = 0; i < 8; i++){
                input_hexadecimal_input_1[i] = 0;
                for (int j = 0; j < 32; j++){
                    input_hexadecimal_input_1[i] += input_1[i][j] * pow(2, 31-j);
                }
            }

            ////////////////////////////// IBUFF_1 //////////////////////////////
            // This function will take input_hexadecimal_input_1 and then make it such that it can be fed into the IBUFF_1 directly
            /*
                For IBUFF_1 all 8 entries are repated 4 times and first and last  entries are padded with 0
            */
        int ibuff_1[34] = {0};
        for (int i = 0; i < 8; i++){
                for (int j = 0; j < 8; j++){
                if (i == 0) {
                    ibuff_1[j + 1] = input_hexadecimal_input_1[j];
                }
                else if (i == 1){
                    ibuff_1[j + 9] = input_hexadecimal_input_1[j];
                }
                else if (i == 2){
                    ibuff_1[j + 17] = input_hexadecimal_input_1[j];
                }
                else if (i == 3){
                    ibuff_1[j + 25] = input_hexadecimal_input_1[j];
                }
                }
                ibuff_1[0] = 0;
                ibuff_1[33] = 0;
        }

            int input_vector_stream[204] = {0};
            for (int j = 0; j < 3; j++){
                if (j == 0){
                    for (int i = 0; i < 34; i++){
                        input_vector_stream[2*i] = ibuff_0[i];
                        input_vector_stream[2*i + 1] = ibuff_1[i];
                    }
                }
                else if (j == 1){
                    for (int i = 0; i < 34; i++){
                        input_vector_stream[68 + 2*i] = wbuff_0[i];
                        input_vector_stream[68 + 2*i + 1] = wbuff_1[i];
                    }
                }
                else if (j == 2){
                    for (int i = 0; i < 34; i++){
                        input_vector_stream[136 + 2*i] = wbuff_2[i];
                        input_vector_stream[136 + 2*i + 1] = wbuff_3[i];
                    }
                }

            }

        /*
            static unsigned int golden_output[64];

            for (int i = 0; i < 8; i++){
                for (int j  = 0; j < 8; j++){
                    if (i == 0){
                        golden_output[2*j] = golden_matrix[i][j];
                    }
                    else if (i == 1){
                        golden_output[2*j + 1] = golden_matrix[i][j];
                    }
                    else if (i == 2){
                        golden_output[2*j + 16] = golden_matrix[i][j];
                    }
                    else if (i == 3){
                        golden_output[2*j + 17] = golden_matrix[i][j];
                    }
                    else if (i == 4){
                        golden_output[2*j + 32] = golden_matrix[i][j];
                    }
                    else if (i == 5){
                        golden_output[2*j + 33] = golden_matrix[i][j];
                    }
                    else if (i == 6){
                        golden_output[2*j + 48] = golden_matrix[i][j];
                    }
                    else if (i == 7){
                        golden_output[2*j + 49] = golden_matrix[i][j];
                    }
                }
            }

            static int32_t in_buffs_4x4[204];
            static int32_t gold_buff_4x4[64];
            for (int i = 0; i < 204; i++){
                in_buffs_4x4[i] = input_vector_stream[i];
            }
        */

            int in_buffs_4x4[204] = {0};
            for (int i = 0; i < 204; i++){
                in_buffs_4x4[i] = input_vector_stream[i];
            };

    token_t *gold;
    token_t *buf;

    init_parameters();

    buf = (token_t *) esp_alloc(size);
    cfg_100[0].hw_buf = buf;

    gold = malloc(out_size);

    init_buffer(in_buffs_4x4, buf);
    esp_run(cfg_100, NACC);

    int32_t each_run[64] = {0};
    copy_result(&buf[out_offset], each_run);

    for (int i = 0; i < 8; i++){
        for (int j  = 0; j < 8; j++){
            if (i == 0){
                result_matrix[i][j] = each_run[2*j];
            }
            else if (i == 1){
                result_matrix[i][j] = each_run[2*j + 1];
            }
            else if (i == 2){
                result_matrix[i][j] = each_run[2*j + 16];
            }
            else if (i == 3){
                result_matrix[i][j] = each_run[2*j + 17];
            }
            else if (i == 4){
                result_matrix[i][j] = each_run[2*j + 32];
            }
            else if (i == 5){
                result_matrix[i][j] = each_run[2*j + 33];
            }
            else if (i == 6){
                result_matrix[i][j] = each_run[2*j + 48];
            }
            else if (i == 7){
                result_matrix[i][j] = each_run[2*j + 49];
            }
        }
    }

     for (int i = 0; i < 64; i++){
	each_run[i] = 0;
     }

    }
    
    else if (conf == 2 || conf == 6) {      // int8x2

        ///////// Added ///////////// 
            //int weight_matrix[SIZE][SIZE];
            //int input_matrix[SIZE][SIZE];
            int golden_matrix[matrix_size][matrix_size];

            // For Weight Matrix
            char output_weight_matrix[matrix_size][matrix_size][2];

            // For Input Matrix
            int transpose_input_matrix[matrix_size][matrix_size];
            char output_input_matrix[matrix_size][matrix_size][8];

            // Do transpose of input matrix
            transpose(matrix_size, input_matrix, transpose_input_matrix);
            

            // Generate the golden matrix
            matrix_mult(matrix_size, weight_matrix, input_matrix, golden_matrix);

            // Print the random matrix and golden matrix
            //printf("Weight Matrix is: \n");
            //print_matrix(weight_matrix);
            //printf("\n Input Matrix is: \n");
            //print_matrix(input_matrix);
            //printf("\n Transpose Input Matrix is: \n");
            //print_matrix(transpose_input_matrix);
            //printf("\n Golden Matrix is: \n");
            //print_matrix(golden_matrix);

            // Convert the weight matrix to binary
            weight_matrix_d2b(matrix_size, conf, char_weight_size, weight_matrix, output_weight_matrix);
            //printf("\n Weight Matrix in binary is: \n");
            //print_weight_matrix_b(output_weight_matrix);

            // Convert the input matrix to binary
            input_matrix_d2b(matrix_size, conf, char_input_size, transpose_input_matrix, output_input_matrix);
            //printf("\n Input Matrix in binary is: \n");
            //print_input_matrix_b(output_input_matrix);

            ////////////////////////////// Input_0 //////////////////////////////
            
            // Transform the input matrix for Input_0
            char input_0[matrix_size][32];
            for (int i = 0; i < matrix_size; i++){
                transform_input_8x2(output_input_matrix[i][0], output_input_matrix[i][1], output_input_matrix[i][2], output_input_matrix[i][3], input_0[i]);
            }

        /*
            printf("\n Transformed Input Matrix for Input_0 is: \n");
            for (int i = 0; i < SIZE; i++){
                for (int j = 0; j < 32; j++){
                    printf("%d", input_0[i][j]);
                }
                printf("\n");
            }
        */

            // Transform the input_0 matrix for orchestrated input
            char input_0_orchestrated[matrix_size][32];
            for (int i = 0; i < matrix_size; i++){
                input_orchestration_8x2(input_0[i], input_0_orchestrated[i]);
            }
        /*
            printf("\n Orchestrated Input Matrix for Input_0 is: \n");
            for (int i = 0; i < SIZE; i++){
                for (int j = 0; j < 32; j++){
                    printf("%d", input_0_orchestrated[i][j]);
                }
                printf("\n");
            }
        */

            // Convert the input_input_0 to hexadecimal note the binary is not in 2's complement
            unsigned int input_hexadecimal_input_0[8];
            for (int i = 0; i < 8; i++){
                input_hexadecimal_input_0[i] = 0;
                for (int j = 0; j < 32; j++){
                    input_hexadecimal_input_0[i] += input_0_orchestrated[i][j] * pow(2, 31-j);
                }
            }

            ////////////////////////////// ibuff_0 //////////////////////////////
            // This function will take input_hexadecimal_input_0 and then make it such that it can be fed into the IBUFF_0 directly
            /*
                For IBUFF_0 all 8 entries are repated 4 times and last 2 entries are padded with 0
            */
        unsigned int ibuff_0[34] = {0};
        for (int i = 0; i < 8; i++){
                for (int j = 0; j < 8; j++){
                if (i == 0) {
                    ibuff_0[j] = input_hexadecimal_input_0[j];
                }
                else if (i == 1){
                    ibuff_0[j + 8] = input_hexadecimal_input_0[j];
                }
                else if (i == 2){
                    ibuff_0[j + 16] = input_hexadecimal_input_0[j];
                }
                else if (i == 3){
                    ibuff_0[j + 24] = input_hexadecimal_input_0[j];
                }
                }
                ibuff_0[32] = 0;
                ibuff_0[33] = 0;
        }

        /*
        // Print ibuff_0
            printf(" \n IBUFF_0 is: \n");
            for (int i = 0; i < 34; i++){
                printf("%x\n", ibuff_0[i]);
            }
        */

            ////////////////////////////// Input_1 //////////////////////////////

            // Transform the input matrix for Input_1
            char input_1[matrix_size][32];
            for (int i = 0; i < matrix_size; i++){
                transform_input_8x2(output_input_matrix[i][4], output_input_matrix[i][5], output_input_matrix[i][6], output_input_matrix[i][7], input_1[i]);
            }

        /*
            printf("\n Transformed Input Matrix for Input_1 is: \n");
            for (int i = 0; i < SIZE; i++){
                for (int j = 0; j < 32; j++){
                    printf("%d", input_1[i][j]);
                }
                printf("\n");
            }
        */

            // Transform the input_1 matrix for orchestrated input
            char input_1_orchestrated[matrix_size][32];
            for (int i = 0; i < matrix_size; i++){
                input_orchestration_8x2(input_1[i], input_1_orchestrated[i]);
            }

        /*
            printf("\n Orchestrated Input Matrix for Input_1 is: \n");
            for (int i = 0; i < SIZE; i++){
                for (int j = 0; j < 32; j++){
                    printf("%d", input_1_orchestrated[i][j]);
                }
                printf("\n");
            }
        */

            // Convert the input_input_1 to hexadecimal note the binary is not in 2's complement
            unsigned int input_hexadecimal_input_1[8];
            for (int i = 0; i < 8; i++){
                input_hexadecimal_input_1[i] = 0;
                for (int j = 0; j < 32; j++){
                    input_hexadecimal_input_1[i] += input_1_orchestrated[i][j] * pow(2, 31-j);
                }
            }

            ////////////////////////////// ibuff_1 //////////////////////////////
            // This function will take input_hexadecimal_input_1 and then make it such that it can be fed into the IBUFF_1 directly
            /*
                For IBUFF_1 all 8 entries are repated 4 times and first and last  entries are padded with 0
            */
        unsigned int ibuff_1[34] = {0};
        for (int i = 0; i < 8; i++){
                for (int j = 0; j < 8; j++){
                if (i == 0) {
                    ibuff_1[j + 1] = input_hexadecimal_input_1[j];
                }
                else if (i == 1){
                    ibuff_1[j + 9] = input_hexadecimal_input_1[j];
                }
                else if (i == 2){
                    ibuff_1[j + 17] = input_hexadecimal_input_1[j];
                }
                else if (i == 3){
                    ibuff_1[j + 25] = input_hexadecimal_input_1[j];
                }
                }
                ibuff_1[0] = 0;
                ibuff_1[33] = 0;
        }

        /*
        // Print ibuff_0
            printf(" \n IBUFF_1 is: \n");
            for (int i = 0; i < 34; i++){
                printf("%x\n", ibuff_1[i]);
            }
        */

            ////////////////////////////// Weight_0 //////////////////////////////
            char weight_0[4][8] = {0};
            transform_weight_8x2(output_weight_matrix[0][0], output_weight_matrix[0][1], output_weight_matrix[0][2], output_weight_matrix[0][3], weight_0[0]);
            transform_weight_8x2(output_weight_matrix[2][0], output_weight_matrix[2][1], output_weight_matrix[2][2], output_weight_matrix[2][3], weight_0[1]);
            transform_weight_8x2(output_weight_matrix[4][0], output_weight_matrix[4][1], output_weight_matrix[4][2], output_weight_matrix[4][3], weight_0[2]);
            transform_weight_8x2(output_weight_matrix[6][0], output_weight_matrix[6][1], output_weight_matrix[6][2], output_weight_matrix[6][3], weight_0[3]);
        
        /*
            printf("\n Weight_0 is: \n");
            for (int i = 0; i < 4; i++){
                for (int j = 0; j < 8; j++){
                    printf("%d", weight_0[i][j]);
                }
                printf("\n");
            }
        */

            // Expand the weight_0 matrix to 32 bits
            char weight_0_32[4][32] = {0};
            weight_32_8x2(weight_0[0], weight_0_32[0]);
            weight_32_8x2(weight_0[1], weight_0_32[1]);
            weight_32_8x2(weight_0[2], weight_0_32[2]);
            weight_32_8x2(weight_0[3], weight_0_32[3]); 

        /*
            printf("\n Weight_0 for 32bits is: \n");
            for (int i = 0; i < 4; i++){
                for (int j = 0; j < 32; j++){
                    printf("%d", weight_0_32[i][j]);
                }
                printf("\n");
            }
        */

            // Convert to hexadecimal
            unsigned int weight_hexadecimal_input_0[4] = {0};
            for (int i = 0; i < 4; i++){
                weight_hexadecimal_input_0[i] = 0;
                for (int j = 0; j < 32; j++){
                    weight_hexadecimal_input_0[i] = weight_hexadecimal_input_0[i] + weight_0_32[i][j] * pow(2, 31-j);
                }
            }

        /*
            // Print the hexadecimal values
            printf("\n Hexadecimal values for weight_0 are: \n");
            for (int i = 0; i < 4; i++){
                printf("%x\n", weight_hexadecimal_input_0[i]);
            }
        */

            ////////////////////////////// wbuff_0 //////////////////////////////
            unsigned int wbuff_0[34] = {0};
            for (int i = 0; i < 4; i++){
                for (int j = 0; j < 8; j++){
                    if(i == 0){
                        wbuff_0[j] = weight_hexadecimal_input_0[i]; 
                    }
                    else if (i == 1){
                        wbuff_0[j + 8] = weight_hexadecimal_input_0[i];
                    }
                    else if(i == 2){
                        wbuff_0[j + 16] = weight_hexadecimal_input_0[i];
                    }
                    else if(i == 3){
                        wbuff_0[j + 24] = weight_hexadecimal_input_0[i];
                    }
                    wbuff_0[32] = 0;
                    wbuff_0[33] = 0;           
                }
            }

        /*
            // Print wbuff_0
            printf(" \n WBUFF_0 is: \n");
            for (int i = 0; i < 34; i++){
                printf("%x\n", wbuff_0[i]);
            }
        */

            ////////////////////////////// Weight_2 //////////////////////////////
            char weight_2[4][8] = {0};
            transform_weight_8x2(output_weight_matrix[0][4], output_weight_matrix[0][5], output_weight_matrix[0][6], output_weight_matrix[0][7], weight_2[0]);
            transform_weight_8x2(output_weight_matrix[2][4], output_weight_matrix[2][5], output_weight_matrix[2][6], output_weight_matrix[2][7], weight_2[1]);
            transform_weight_8x2(output_weight_matrix[4][4], output_weight_matrix[4][5], output_weight_matrix[4][6], output_weight_matrix[4][7], weight_2[2]);
            transform_weight_8x2(output_weight_matrix[6][4], output_weight_matrix[6][5], output_weight_matrix[6][6], output_weight_matrix[6][7], weight_2[3]);

        /*
            printf("\n Weight_2 is: \n");
            for (int i = 0; i < 4; i++){
                for (int j = 0; j < 8; j++){
                    printf("%d", weight_2[i][j]);
                }
                printf("\n");
            }
        */

            // Expand the weight_2 matrix to 32 bits
            char weight_2_32[4][32] = {0};
            weight_32_8x2(weight_2[0], weight_2_32[0]);
            weight_32_8x2(weight_2[1], weight_2_32[1]);
            weight_32_8x2(weight_2[2], weight_2_32[2]);
            weight_32_8x2(weight_2[3], weight_2_32[3]); 

        /*
            printf("\n Weight_2 for 32bits is: \n");
            for (int i = 0; i < 4; i++){
                for (int j = 0; j < 32; j++){
                    printf("%d", weight_2_32[i][j]);
                }
                printf("\n");
            }
        */

            // Convert to hexadecimal
            unsigned int weight_hexadecimal_input_2[4];
            for (int i = 0; i < 4; i++){
                weight_hexadecimal_input_2[i] = 0;
                for (int j = 0; j < 32; j++){
                    weight_hexadecimal_input_2[i] = weight_hexadecimal_input_2[i] + weight_2_32[i][j] * pow(2, 31-j);
                }
            }

        /*
            // Print the hexadecimal values
            printf("\n Hexadecimal values for weight_2 are: \n");
            for (int i = 0; i < 4; i++){
                printf("%x\n", weight_hexadecimal_input_2[i]);
            }
        */

            ////////////////////////////// wbuff_2 //////////////////////////////
            unsigned int wbuff_2[34] = {0};
            for (int i = 0; i < 4; i++){
                for (int j = 0; j < 8; j++){
                    if(i == 0){
                        wbuff_2[j + 1] = weight_hexadecimal_input_2[i]; 
                    }
                    else if (i == 1){
                        wbuff_2[j + 9] = weight_hexadecimal_input_2[i];
                    }
                    else if(i == 2){
                        wbuff_2[j + 17] = weight_hexadecimal_input_2[i];
                    }
                    else if(i == 3){
                        wbuff_2[j + 25] = weight_hexadecimal_input_2[i];
                    }
                    wbuff_2[0] = 0;
                    wbuff_2[33] = 0;           
                }
            }

        /*
            // Print wbuff_2
            printf(" \n WBUFF_2 is: \n");
            for (int i = 0; i < 34; i++){
                printf("%x\n", wbuff_2[i]);
            }
        */

            ////////////////////////////// Weight_1 //////////////////////////////
            char weight_1[4][8] = {0};
            transform_weight_8x2(output_weight_matrix[1][0], output_weight_matrix[1][1], output_weight_matrix[1][2], output_weight_matrix[1][3], weight_1[0]);
            transform_weight_8x2(output_weight_matrix[3][0], output_weight_matrix[3][1], output_weight_matrix[3][2], output_weight_matrix[3][3], weight_1[1]);
            transform_weight_8x2(output_weight_matrix[5][0], output_weight_matrix[5][1], output_weight_matrix[5][2], output_weight_matrix[5][3], weight_1[2]);
            transform_weight_8x2(output_weight_matrix[7][0], output_weight_matrix[7][1], output_weight_matrix[7][2], output_weight_matrix[7][3], weight_1[3]);

        /*
            printf("\n Weight_1 is: \n");
            for (int i = 0; i < 4; i++){
                for (int j = 0; j < 8; j++){
                    printf("%d", weight_1[i][j]);
                }
                printf("\n");
            }
        */

            // Expand the weight_1 matrix to 32 bits
            char weight_1_32[4][32] = {0};
            weight_32_8x2(weight_1[0], weight_1_32[0]);
            weight_32_8x2(weight_1[1], weight_1_32[1]);
            weight_32_8x2(weight_1[2], weight_1_32[2]);
            weight_32_8x2(weight_1[3], weight_1_32[3]); 

        /*
            printf("\n Weight_1 for 32bits is: \n");
            for (int i = 0; i < 4; i++){
                for (int j = 0; j < 32; j++){
                    printf("%d", weight_1_32[i][j]);
                }
                printf("\n");
            }
        */

            // Convert to hexadecimal
            unsigned int weight_hexadecimal_input_1[4];
            for (int i = 0; i < 4; i++){
                weight_hexadecimal_input_1[i] = 0;
                for (int j = 0; j < 32; j++){
                    weight_hexadecimal_input_1[i] = weight_hexadecimal_input_1[i] + weight_1_32[i][j] * pow(2, 31-j);
                }
            }

            ////////////////////////////// wbuff_1 //////////////////////////////
            unsigned int wbuff_1[34] = {0};
            for (int i = 0; i < 4; i++){
                for (int j = 0; j < 8; j++){
                    if(i == 0){
                        wbuff_1[j + 1] = weight_hexadecimal_input_1[i]; 
                    }
                    else if (i == 1){
                        wbuff_1[j + 9] = weight_hexadecimal_input_1[i];
                    }
                    else if(i == 2){
                        wbuff_1[j + 17] = weight_hexadecimal_input_1[i];
                    }
                    else if(i == 3){
                        wbuff_1[j + 25] = weight_hexadecimal_input_1[i];
                    }
                    wbuff_1[0] = 0;
                    wbuff_1[33] = 0;           
                }
            }

        /*
            // Print wbuff_1
            printf(" \n WBUFF_1 is: \n");
            for (int i = 0; i < 34; i++){
                printf("%x\n", wbuff_1[i]);
            }
        */

            ////////////////////////////// Weight_3 //////////////////////////////
            char weight_3[4][8] = {0};
            transform_weight_8x2(output_weight_matrix[1][4], output_weight_matrix[1][5], output_weight_matrix[1][6], output_weight_matrix[1][7], weight_3[0]);
            transform_weight_8x2(output_weight_matrix[3][4], output_weight_matrix[3][5], output_weight_matrix[3][6], output_weight_matrix[3][7], weight_3[1]);
            transform_weight_8x2(output_weight_matrix[5][4], output_weight_matrix[5][5], output_weight_matrix[5][6], output_weight_matrix[5][7], weight_3[2]);
            transform_weight_8x2(output_weight_matrix[7][4], output_weight_matrix[7][5], output_weight_matrix[7][6], output_weight_matrix[7][7], weight_3[3]);

        /*
            printf("\n Weight_3 is: \n");
            for (int i = 0; i < 4; i++){
                for (int j = 0; j < 8; j++){
                    printf("%d", weight_3[i][j]);
                }
                printf("\n");
            }
        */

            // Expand the weight_3 matrix to 32 bits
            char weight_3_32[4][32] = {0};
            weight_32_8x2(weight_3[0], weight_3_32[0]);
            weight_32_8x2(weight_3[1], weight_3_32[1]);
            weight_32_8x2(weight_3[2], weight_3_32[2]);
            weight_32_8x2(weight_3[3], weight_3_32[3]); 

        /*
            printf("\n Weight_1 for 32bits is: \n");
            for (int i = 0; i < 4; i++){
                for (int j = 0; j < 32; j++){
                    printf("%d", weight_3_32[i][j]);
                }
                printf("\n");
            }
        */

            // Convert to hexadecimal
            unsigned int weight_hexadecimal_input_3[4];
            for (int i = 0; i < 4; i++){
                weight_hexadecimal_input_3[i] = 0;
                for (int j = 0; j < 32; j++){
                    weight_hexadecimal_input_3[i] = weight_hexadecimal_input_3[i] + weight_3_32[i][j] * pow(2, 31-j);
                }
            }

            ////////////////////////////// wbuff_3 //////////////////////////////
            unsigned int wbuff_3[34] = {0};
            for (int i = 0; i < 4; i++){
                for (int j = 0; j < 8; j++){
                    if(i == 0){
                        wbuff_3[j + 2] = weight_hexadecimal_input_3[i]; 
                    }
                    else if (i == 1){
                        wbuff_3[j + 10] = weight_hexadecimal_input_3[i];
                    }
                    else if(i == 2){
                        wbuff_3[j + 18] = weight_hexadecimal_input_3[i];
                    }
                    else if(i == 3){
                        wbuff_3[j + 26] = weight_hexadecimal_input_3[i];
                    }
                    wbuff_3[0] = 0;
                    wbuff_3[1] = 0;           
                }
            }

        /*
            // Print wbuff_3
            printf(" \n WBUFF_3 is: \n");
            for (int i = 0; i < 34; i++){
                printf("%x\n", wbuff_3[i]);
            }
        */

            ////////////////////////////// Input_vector stream for FPGA //////////////////////////////
            static unsigned int input_vector_stream[204] = {0};
            for (int j = 0; j < 3; j++){
                if (j == 0){
                    for (int i = 0; i < 34; i++){
                        input_vector_stream[2*i] = ibuff_0[i];
                        input_vector_stream[2*i + 1] = ibuff_1[i];
                    }
                }
                else if (j == 1){
                    for (int i = 0; i < 34; i++){
                        input_vector_stream[68 + 2*i] = wbuff_0[i];
                        input_vector_stream[68 + 2*i + 1] = wbuff_1[i];
                    }
                }
                else if (j == 2){
                    for (int i = 0; i < 34; i++){
                        input_vector_stream[136 + 2*i] = wbuff_2[i];
                        input_vector_stream[136 + 2*i + 1] = wbuff_3[i];
                    }
                }

            }

        /*
            // Print input_vector_stream
            printf(" \n Input_vector_stream is: \n");
            for (int i = 0; i < 204; i++){
                printf("%x\n", input_vector_stream[i]);
            }
        */

            ////////////////////////////// Golden Output //////////////////////////////
        /*
            static unsigned int golden_output[64];

            for (int i = 0; i < 8; i++){
                for (int j  = 0; j < 8; j++){
                    if (i == 0){
                        golden_output[2*j] = golden_matrix[i][j];
                    }
                    else if (i == 1){
                        golden_output[2*j + 1] = golden_matrix[i][j];
                    }
                    else if (i == 2){
                        golden_output[2*j + 16] = golden_matrix[i][j];
                    }
                    else if (i == 3){
                        golden_output[2*j + 17] = golden_matrix[i][j];
                    }
                    else if (i == 4){
                        golden_output[2*j + 32] = golden_matrix[i][j];
                    }
                    else if (i == 5){
                        golden_output[2*j + 33] = golden_matrix[i][j];
                    }
                    else if (i == 6){
                        golden_output[2*j + 48] = golden_matrix[i][j];
                    }
                    else if (i == 7){
                        golden_output[2*j + 49] = golden_matrix[i][j];
                    }
                }
            }
        */

            static int32_t in_buffs_8x2[204] = {0};
            //static int32_t gold_buff_8x2[64];
            for (int i = 0; i < 204; i++){
                in_buffs_8x2[i] = input_vector_stream[i];
            }

	token_t *gold;
	token_t *buf;

	init_parameters();

	buf = (token_t *) esp_alloc(size);
	cfg_100[0].hw_buf = buf;
    
	gold = malloc(out_size);

	init_buffer(in_buffs_8x2, buf);
	esp_run(cfg_100, NACC);

    int each_run[64] = {0};
    copy_result(&buf[out_offset], each_run);

    for (int i = 0; i < 8; i++){
        for (int j  = 0; j < 8; j++){
            if (i == 0){
                result_matrix[i][j] = each_run[2*j];
            }
            else if (i == 1){
                result_matrix[i][j] = each_run[2*j + 1];
            }
            else if (i == 2){
                result_matrix[i][j] = each_run[2*j + 16];
            }
            else if (i == 3){
                result_matrix[i][j] = each_run[2*j + 17];
            }
            else if (i == 4){
                result_matrix[i][j] = each_run[2*j + 32];
            }
            else if (i == 5){
                result_matrix[i][j] = each_run[2*j + 33];
            }
            else if (i == 6){
                result_matrix[i][j] = each_run[2*j + 48];
            }
            else if (i == 7){
                result_matrix[i][j] = each_run[2*j + 49];
            }
        }
    }

     for (int i = 0; i < 64; i++){
	each_run[i] = 0;
     }

    }
    
    else if (conf == 3 || conf == 7) {      // int8x8

        /////////////////// Added Part ///////////////////

            //int weight_matrix[SIZE][SIZE];
            //int input_matrix[SIZE][SIZE];
            int golden_matrix[matrix_size][matrix_size];

            // For Weight Matrix
            char output_weight_matrix[matrix_size][matrix_size][8];
            char transformed_weight_matrix[matrix_size][matrix_size][32];

            // For Input Matrix
            int transpose_input_matrix[matrix_size][matrix_size];
            char output_input_matrix[matrix_size][matrix_size][8];
            char transformed_input_matrix[matrix_size][matrix_size][32];

            // Generate random matrices
            //generate_random_matrix(weight_matrix);
            //generate_random_matrix(input_matrix);

            // Do transpose of input matrix
            transpose(matrix_size, input_matrix, transpose_input_matrix);

            // Generate the golden matrix
            matrix_mult(matrix_size, weight_matrix, input_matrix, golden_matrix);

            // Print the random matrix and golden matrix
            //printf("Weight Matrix is: \n");
            //print_matrix(weight_matrix);
            //printf("\n Input Matrix is: \n");
            //print_matrix(input_matrix);
            //printf("\n Transpose Input Matrix is: \n");
            //print_matrix(transpose_input_matrix);
            //printf("\n Golden Matrix is: \n");
            //print_matrix(golden_matrix);

            // Convert the weight matrix to binary
            weight_matrix_d2b(matrix_size, conf, char_weight_size, weight_matrix, output_weight_matrix);
            //printf("\n Weight Matrix in binary is: \n");
            //print_matrix_b(output_weight_matrix);

            // Convert the input matrix to binary
            input_matrix_d2b(matrix_size, conf, char_input_size, transpose_input_matrix, output_input_matrix);
            //printf("\n Input Matrix in binary is: \n");
            //print_matrix_b(output_input_matrix);

            // Transform the weight matrix
            for (int i = 0; i < matrix_size; i++){
                for (int j = 0; j < matrix_size; j++){
                    weight_transform_8x8(output_weight_matrix[i][j], transformed_weight_matrix[i][j]);
                }
            }



            // Print the transformed weight matrix
        /*
            printf("\n Transformed Weight Matrix is: \n");
            for (int i = 0; i < 32; i++){
                printf("%d", transformed_weight_matrix[0][0][i]);
            }
            printf("\n");
            for (int i = 0; i < 32; i++){
                printf("%d", transformed_weight_matrix[0][1][i]);
            }
            printf("\n");
            for (int i = 0; i < 32; i++){
                printf("%d", transformed_weight_matrix[1][0][i]);
            }
            printf("\n");
            for (int i = 0; i < 32; i++){
                printf("%d", transformed_weight_matrix[1][1][i]);
            }
            printf("\n");
        */

            //print_matrix_b(transformed_weight_matrix);


            // Transform the input matrix
            for (int i = 0; i < matrix_size; i++){
                for (int j = 0; j < matrix_size; j++){
                    input_transform_8x8(output_input_matrix[i][j], transformed_input_matrix[i][j]);
                }
            }

            // Print the transformed input matrix
        /*
            printf("\n Transformed Input Matrix is: \n");
            for (int i = 0; i < 32; i++){
                printf("%d", transformed_input_matrix[0][0][i]);
            }
            printf("\n");
            for (int i = 0; i < 32; i++){
                printf("%d", transformed_input_matrix[0][1][i]);
            }
            printf("\n");
            for (int i = 0; i < 32; i++){
                printf("%d", transformed_input_matrix[1][0][i]);
            }
            printf("\n");
            for (int i = 0; i < 32; i++){
                printf("%d", transformed_input_matrix[1][1][i]);
            }
            printf("\n");
        */

            //print_matrix_b(transformed_input_matrix);

            //////////////// Flatten Weights ////////////////
            unsigned int weights[4][32] = {0};
            for (int i = 0; i < 4; i++){
                for (int j = 0; j < 32; j++){
                    if (i == 0)
                        weights[i][j] = transformed_weight_matrix[0][0][j];
                    else if (i == 1)
                        weights[i][j] = transformed_weight_matrix[0][1][j];
                    else if (i == 2)
                        weights[i][j] = transformed_weight_matrix[1][0][j];
                    else if (i == 3)
                        weights[i][j] = transformed_weight_matrix[1][1][j];
                }
            }

        /*
            printf("\n Flattened Weights is: \n");
            for (int i = 0; i < 4; i++){
                for (int j = 0; j < 32; j++){
                    printf("%d", weights[i][j]);
                }
                printf("\n");
            }
        */

            //////////////// Hexadecimal Weights ////////////////
            unsigned int hex_weights[4] = {0};
            for (int i = 0; i < 4; i++){
                hex_weights[i] = 0;
                for (int j = 0; j < 32; j++){
                    hex_weights[i] += weights[i][j] * pow(2, 31-j);
                }
            }

        /*
            printf("\n Hexadecimal Weights is: \n");
            for (int i = 0; i < 4; i++){
                printf("%x\n", hex_weights[i]);
            }
        */

            //////////////// Flatten Inputs ////////////////
            unsigned int inputs[4][32] = {0};
            for (int i = 0; i < 4; i++){
                for (int j = 0; j < 32; j++){
                    if (i == 0)
                        inputs[i][j] = transformed_input_matrix[0][0][j];
                    else if (i == 1)
                        inputs[i][j] = transformed_input_matrix[0][1][j];
                    else if (i == 2)
                        inputs[i][j] = transformed_input_matrix[1][0][j];
                    else if (i == 3)
                        inputs[i][j] = transformed_input_matrix[1][1][j];
                }
            }

        /*
            printf("\n Flattened Inputs is: \n");
            for (int i = 0; i < 4; i++){
                for (int j = 0; j < 32; j++){
                    printf("%d", inputs[i][j]);
                }
                printf("\n");
            }
        */

            //////////////// Hexadecimal Inputs ////////////////
            unsigned int hex_inputs[4] = {0};
            for (int i = 0; i < 4; i++){
                hex_inputs[i] = 0;
                for (int j = 0; j < 32; j++){
                    hex_inputs[i] += inputs[i][j] * pow(2, 31-j);
                }
            }

        /*
            printf("\n Hexadecimal Inputs is: \n");
            for (int i = 0; i < 4; i++){
                printf("%x\n", hex_inputs[i]);
            }
        */

            //////////////////// wbuff_0 ////////////////////
            // Save the transformed weight matrix[0][0] in a wbuff_0[0]
            int32_t wbuff_0[4] = {0};
            wbuff_0[0] = hex_weights[0];
            wbuff_0[1] = hex_weights[0];
            wbuff_0[2] = 0;
            wbuff_0[3] = 0;

        /*
            printf("\n wbuff_0 is: \n");
            for (int i = 0; i < 4; i++){
                printf("%x", wbuff_0[i]);
                printf("\n");
            }
        */

            //////////////////// wbuff_1 ////////////////////
            int32_t wbuff_1[4] = {0};
            wbuff_1[0] = 0;
            wbuff_1[1] = hex_weights[2];
            wbuff_1[2] = hex_weights[2];
            wbuff_1[3] = 0;

        /*
            printf("\n wbuff_1 is: \n");
            for (int i = 0; i < 4; i++){
                printf("%x", wbuff_1[i]);
                printf("\n");
            }
        */

            //////////////////// wbuff_2 ////////////////////
            int32_t wbuff_2[4] = {0};
            wbuff_2[0] = 0;
            wbuff_2[1] = hex_weights[1];
            wbuff_2[2] = hex_weights[1];
            wbuff_2[3] = 0;

        /*
            printf("\n wbuff_2 is: \n");
            for (int i = 0; i < 4; i++){
                printf("%x", wbuff_2[i]);
                printf("\n");
            }
        */


            //////////////////// wbuff_3 ////////////////////
            int32_t wbuff_3[4] = {0};
            wbuff_3[0] = 0;
            wbuff_3[1] = 0;
            wbuff_3[2] = hex_weights[3];
            wbuff_3[3] = hex_weights[3];

        /*
            printf("\n wbuff_3 is: \n");
            for (int i = 0; i < 4; i++){
                printf("%x", wbuff_3[i]);
                printf("\n");
            }
        */

            //////////////////// ibuff_0 ////////////////////
            int32_t ibuff_0[4] = {0};
            ibuff_0[0] = hex_inputs[0];
            ibuff_0[1] = hex_inputs[2];
            ibuff_0[2] = 0;
            ibuff_0[3] = 0;

        /*
            printf("\n ibuff_0 is: \n");
            for (int i = 0; i < 4; i++){
                    printf("%x", ibuff_0[i]);
                printf("\n");
            }
        */


            //////////////////// ibuff_1 ////////////////////
            int32_t ibuff_1[4] = {0};
            ibuff_1[0] = 0;
            ibuff_1[1] = hex_inputs[1];
            ibuff_1[2] = hex_inputs[3];
            ibuff_1[3] = 0;


        /*
            printf("\n ibuff_1 is: \n");
            for (int i = 0; i < 4; i++){
                    printf("%x", ibuff_1[i]);
                printf("\n");
            }
        */


            //////////////////// input vector stream ////////////////////
            int32_t input_vector_stream[24] = {0};
            for (int j = 0; j < 3; j++){
                if (j == 0)
                    for (int i = 0; i < 4; i++){
                        input_vector_stream[2*i] = ibuff_0[i];
                        input_vector_stream[2*i + 1] = ibuff_1[i];
                    }
                else if (j == 1)
                    for (int i = 0; i < 4; i++){
                        input_vector_stream[8 + 2*i] = wbuff_0[i];
                        input_vector_stream[8 + 2*i + 1] = wbuff_1[i];
                    }
                else if (j == 2)
                    for (int i = 0; i < 4; i++){
                        input_vector_stream[16 + 2*i] = wbuff_2[i];
                        input_vector_stream[16 + 2*i + 1] = wbuff_3[i];
                    }
            }

            //////////////////// Golden Output ////////////////////
            //int32_t gold_buff_8x8[4] = {0};
            //gold_buff_8x8[0] = golden_matrix[0][0];
            //gold_buff_8x8[1] = golden_matrix[1][0];
            //gold_buff_8x8[2] = golden_matrix[0][1];
            //gold_buff_8x8[3] = golden_matrix[1][1];

            int32_t in_buffs_8x8[24] = {0};
            //int32_t gold_buff_8x8[4];
            
            for (int i = 0; i < 24; i++){
                in_buffs_8x8[i] = input_vector_stream[i];
        }

            token_t *gold;
            token_t *buf;

            init_parameters();

            buf = (token_t *) esp_alloc(size);
            cfg_100[0].hw_buf = buf;

            gold = malloc(out_size);

            init_buffer(in_buffs_8x8, buf);
            esp_run(cfg_100, NACC);

            int each_run [4] = {0};
            copy_result(&buf[out_offset], each_run);

            result_matrix[0][0] = each_run[0];
            result_matrix[1][0] = each_run[1];
            result_matrix[0][1] = each_run[2];
            result_matrix[1][1] = each_run[3];

            for (int i = 0; i < 4; i++){
            each_run[i] = 0;
            } 

    }

}

void matrix_add(int n, int matrix1[n][n], int matrix2[n][n], int result [n][n]) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            result[i][j] = matrix1[i][j] + matrix2[i][j];
        }
    }
}

// Function to generate submatrix from big matrix
void generate_submatrix(int n, int matrix[n][n], int submatrix_1[n/2][n/2], int submatrix_2[n/2][n/2], int submatrix_3[n/2][n/2], int submatrix_4[n/2][n/2]) {
    // Submatrix_1 is the top left quarter of the matrix
    for (int i = 0; i < n/2; i++) {
        for (int j = 0; j < n/2; j++) {
            submatrix_1[i][j] = matrix[i][j];
        }
    }

    // Submatrix_2 is the top right quarter of the matrix
    for (int i = 0; i < n/2; i++) {
        for (int j = n/2; j < n; j++) {
            submatrix_2[i][j - n/2] = matrix[i][j];
        }
    }

    // Submatrix_3 is the bottom left quarter of the matrix
    for (int i = n/2; i < n; i++) {
        for (int j = 0; j < n/2; j++) {
            submatrix_3[i - n/2][j] = matrix[i][j];
        }
    }

    // Submatrix_4 is the bottom right quarter of the matrix
    for (int i = n/2; i < n; i++) {
        for (int j = n/2; j < n; j++) {
            submatrix_4[i - n/2][j - n/2] = matrix[i][j];
        }
    }
}


static int validate_buffer(token_t *out, token_t *gold)
{
	int i;
	int j;
	unsigned errors = 0;

        /*
	for (i = 0; i < 1; i++)
		for (j = 0; j < 2 * reg2; j++)
			if (gold[i * out_words_adj + j] != out[i * out_words_adj + j])
				errors++;

	return errors;
        */
	for (i = 0; i < 1; i++)
		//for (j = 0; j < 2*reg2; j++) {
		for (j = 0; j < dma_word_reg3; j++) { //reg3 is the output index
			printf("acc out %d gold: %d, out: %d, gold_addr: %p, out_addr: %p \n", 
					j, gold[i * out_words_adj + j], out[i * out_words_adj + j], 
					&gold[i * out_words_adj + j], &out[i * out_words_adj + j]);
			
			if (gold[i * out_words_adj + j] != out[i * out_words_adj + j])
				errors++;
	
	}
	return errors;

}



void strassen (int BASE_MATRIX_SIZE, int conf, int matrix_size, int char_input_size, int char_weight_size, int matrix1[matrix_size][matrix_size], int matrix2[matrix_size][matrix_size], int result_matrix[matrix_size][matrix_size]) {
    // matrix1 is weight matrix
    // matrix2 is input matrix

    if (matrix_size == BASE_MATRIX_SIZE) {
        mult(BASE_MATRIX_SIZE, conf, char_weight_size, char_input_size, matrix1, matrix2, result_matrix);
    }

    else {
        int submatrix_size = matrix_size / 2;

        // For Weight Matrix                 matrix1 is Weight Matrix
        int a[submatrix_size][submatrix_size];
        int b[submatrix_size][submatrix_size];
        int c[submatrix_size][submatrix_size];
        int d[submatrix_size][submatrix_size];
        
        // For Input Matrix                matrix2 is Input Matrix
        int e[submatrix_size][submatrix_size];
        int f[submatrix_size][submatrix_size];
        int g[submatrix_size][submatrix_size];
        int h[submatrix_size][submatrix_size];

        // Generate weight submatrices
        generate_submatrix(matrix_size, matrix1, a, b, c, d);

        // Generate input submatrices
        generate_submatrix(matrix_size, matrix2, e, f, g, h);

        // For partial results
        int ae[submatrix_size][submatrix_size];
        int af[submatrix_size][submatrix_size];
        int bg[submatrix_size][submatrix_size];
        int bh[submatrix_size][submatrix_size];
        int ce[submatrix_size][submatrix_size];
        int cf[submatrix_size][submatrix_size];
        int dg[submatrix_size][submatrix_size];
        int dh[submatrix_size][submatrix_size];

        // For partial result matrix
        int C11[submatrix_size][submatrix_size];
        int C12[submatrix_size][submatrix_size];
        int C21[submatrix_size][submatrix_size];
        int C22[submatrix_size][submatrix_size];

// Multiply the submatrices to get partial results
        if (submatrix_size == BASE_MATRIX_SIZE) {
            mult(BASE_MATRIX_SIZE, conf, char_weight_size, char_input_size, a, e, ae);
	    printf("\n ae executed \n");
            mult(BASE_MATRIX_SIZE, conf, char_weight_size, char_input_size,a, f, af);
	    printf("\n af executed \n");
            mult(BASE_MATRIX_SIZE, conf, char_weight_size, char_input_size, b, g, bg);
	    printf("\n bg executed \n");
            mult(BASE_MATRIX_SIZE, conf, char_weight_size, char_input_size, b, h, bh);
	    printf("\n bh executed \n");
            mult(BASE_MATRIX_SIZE, conf, char_weight_size, char_input_size, c, e, ce);
	    printf("\n ce executed \n");
            mult(BASE_MATRIX_SIZE, conf, char_weight_size, char_input_size, c, f, cf);
	    printf("\n cf executed \n");
            mult(BASE_MATRIX_SIZE, conf, char_weight_size, char_input_size, d, g, dg);
	    printf("\n dg executed \n");
            mult(BASE_MATRIX_SIZE, conf, char_weight_size, char_input_size, d, h, dh);
	    printf("\n dh executed \n");
        }
        else {
	    printf("\n else executed \n");
            strassen(BASE_MATRIX_SIZE, conf, submatrix_size, char_input_size, char_weight_size, a, e, ae);
            strassen(BASE_MATRIX_SIZE, conf, submatrix_size, char_input_size, char_weight_size, a, f, af);
            strassen(BASE_MATRIX_SIZE, conf, submatrix_size, char_input_size, char_weight_size, b, g, bg);
            strassen(BASE_MATRIX_SIZE, conf, submatrix_size, char_input_size, char_weight_size, b, h, bh);
            strassen(BASE_MATRIX_SIZE, conf, submatrix_size, char_input_size, char_weight_size, c, e, ce);
            strassen(BASE_MATRIX_SIZE, conf, submatrix_size, char_input_size, char_weight_size, c, f, cf);
            strassen(BASE_MATRIX_SIZE, conf, submatrix_size, char_input_size, char_weight_size, d, g, dg);
            strassen(BASE_MATRIX_SIZE, conf, submatrix_size, char_input_size, char_weight_size, d, h, dh);
        }
        // Add the partial results to get partial matrices
        matrix_add(submatrix_size, ae, bg, C11);
        matrix_add(submatrix_size, af, bh, C12);
        matrix_add(submatrix_size, ce, dg, C21);
        matrix_add(submatrix_size, cf, dh, C22);
	printf("\n C11 is \n");
	print_matrix(submatrix_size, C11);
	printf("\n C12 is \n");
	print_matrix(submatrix_size, C12);
	printf("\n C21 is \n");
	print_matrix(submatrix_size, C21);
	printf("\n C22 is \n");
	print_matrix(submatrix_size, C22);

        // Combine the partial matrices to get the final result
        for (int i = 0; i < submatrix_size; i++) {
            for (int j = 0; j < submatrix_size; j++) {
                result_matrix[i][j] = C11[i][j];
                result_matrix[i][j + submatrix_size] = C12[i][j];
                result_matrix[i + submatrix_size][j] = C21[i][j];
                result_matrix[i + submatrix_size][j + submatrix_size] = C22[i][j];
            }
        }
    }

}

int main(int argc, char **argv)
{

    /// Added ///
   
    int errors = 0;

    // Added //
    int conf;
    printf ("What is operation you want to do? \n");
    printf ("0. Input bitwidth = 2, Weight bitwidth = 2; Unsigned INT 2x2 \n");
    printf ("1. Input bitwidth = 4, Weight bitwidth = 4; Unsigned INT 4x4 \n");
    printf ("2. Input bitwidth = 8, Weight bitwidth = 2; Unsigned INT 8x2 \n");
    printf ("3. Input bitwidth = 8, Weight bitwidth = 8; Unsigned INT 8x8 \n");
    printf ("4. Input bitwidth = 2, Weight bitwidth = 2; Signed INT 2x2 \n");
    printf ("5. Input bitwidth = 4, Weight bitwidth = 4; Signed INT 4x4 \n");
    printf ("6. Input bitwidth = 8, Weight bitwidth = 2; Signed INT 8x2 \n");
    printf ("7. Input bitwidth = 8, Weight bitwidth = 8; Signed INT 8x8 \n");
    printf("Enter the operation number: ");
    scanf("%d", &conf);

    if (conf == 0) {
    	((struct fuseml_rtl_access*)cfg_100[0].esp_desc)->reg5 = 0;
	((struct fuseml_rtl_access*)cfg_100[0].esp_desc)->reg4 = 0;
	((struct fuseml_rtl_access*)cfg_100[0].esp_desc)->reg3 = 512;	 // 1024 divide by 2;
	((struct fuseml_rtl_access*)cfg_100[0].esp_desc)->reg2 = 1542;	 // 3084 divide by 2;
	dma_word_reg2 = 3084;
	dma_word_reg3 = 1024;
    }
    else if (conf == 1) {
    	((struct fuseml_rtl_access*)cfg_100[0].esp_desc)->reg5 = 0;
	((struct fuseml_rtl_access*)cfg_100[0].esp_desc)->reg4 = 1;
	((struct fuseml_rtl_access*)cfg_100[0].esp_desc)->reg3 = 32; 	 // 64 divide by 2;
	((struct fuseml_rtl_access*)cfg_100[0].esp_desc)->reg2 = 102;	 // 204 divide by 2;
	dma_word_reg2 = 204;
	dma_word_reg3 = 64;
    }
    else if (conf == 2) {
    	((struct fuseml_rtl_access*)cfg_100[0].esp_desc)->reg5 = 0;
	((struct fuseml_rtl_access*)cfg_100[0].esp_desc)->reg4 = 2;
	((struct fuseml_rtl_access*)cfg_100[0].esp_desc)->reg3 = 32; 	 // 64 divide by 2;
	((struct fuseml_rtl_access*)cfg_100[0].esp_desc)->reg2 = 102;	 // 204 divide by 2;
	dma_word_reg2 = 204;
	dma_word_reg3 = 64;
    }
    else if (conf == 3) {
    	((struct fuseml_rtl_access*)cfg_100[0].esp_desc)->reg5 = 0;
	((struct fuseml_rtl_access*)cfg_100[0].esp_desc)->reg4 = 3;
	((struct fuseml_rtl_access*)cfg_100[0].esp_desc)->reg3 = 2; 	 // 4 divide by 2;
	((struct fuseml_rtl_access*)cfg_100[0].esp_desc)->reg2 = 12;	 // 24 divide by 2;
	dma_word_reg2 = 24;
	dma_word_reg3 = 4;
    }
    else if (conf == 4) {
    	((struct fuseml_rtl_access*)cfg_100[0].esp_desc)->reg5 = 1;
	((struct fuseml_rtl_access*)cfg_100[0].esp_desc)->reg4 = 0;
	((struct fuseml_rtl_access*)cfg_100[0].esp_desc)->reg3 = 512; 	 // 1024 divide by 2;
	((struct fuseml_rtl_access*)cfg_100[0].esp_desc)->reg2 = 1542;	 // 3084 divide by 2;
	dma_word_reg2 = 3084;
	dma_word_reg3 = 1024;
    }
    else if (conf == 5) {
    	((struct fuseml_rtl_access*)cfg_100[0].esp_desc)->reg5 = 1;
	((struct fuseml_rtl_access*)cfg_100[0].esp_desc)->reg4 = 1;
	((struct fuseml_rtl_access*)cfg_100[0].esp_desc)->reg3 = 32; 	 // 64 divide by 2;
	((struct fuseml_rtl_access*)cfg_100[0].esp_desc)->reg2 = 102;	 // 204 divide by 2;
	dma_word_reg2 = 204;
	dma_word_reg3 = 64;
    }
    else if (conf == 6) {
    	((struct fuseml_rtl_access*)cfg_100[0].esp_desc)->reg5 = 1;
	((struct fuseml_rtl_access*)cfg_100[0].esp_desc)->reg4 = 2;
	((struct fuseml_rtl_access*)cfg_100[0].esp_desc)->reg3 = 32; 	 // 64 divide by 2;
	((struct fuseml_rtl_access*)cfg_100[0].esp_desc)->reg2 = 102;	 // 204 divide by 2;
	dma_word_reg2 = 204;
	dma_word_reg3 = 64;
    }
    else if (conf == 7) {
    	((struct fuseml_rtl_access*)cfg_100[0].esp_desc)->reg5 = 1;
	((struct fuseml_rtl_access*)cfg_100[0].esp_desc)->reg4 = 3;
	((struct fuseml_rtl_access*)cfg_100[0].esp_desc)->reg3 = 2; 	 // 4 divide by 2;
	((struct fuseml_rtl_access*)cfg_100[0].esp_desc)->reg2 = 12;	 // 24 divide by 2;
	dma_word_reg2 = 24;
	dma_word_reg3 = 4;
    }



    int BASE_MATRIX_SIZE;
    if (conf == 0 || conf == 4) {
        BASE_MATRIX_SIZE = 32;
    }
    else if (conf == 1 || conf == 2 || conf == 5 || conf == 6) {
        BASE_MATRIX_SIZE = 8;
    }
    else if (conf == 3 || conf == 7) {
        BASE_MATRIX_SIZE = 2;
    }

    int char_input_size = 0;
    if (conf == 0 || conf == 4) {
        char_input_size = 2;
    }
    else if (conf == 1 || conf == 5) {
        char_input_size = 4;
    }
    else if (conf == 2 || conf == 3 || conf == 6 || conf == 7) {
        char_input_size = 8;
    }

    int char_weight_size = 0;
    if (conf == 0 || conf == 2 || conf == 4 || conf == 6) {
        char_weight_size = 2; 
    }
    else if (conf == 1 || conf == 5) {
        char_weight_size = 4;
    }
    else if (conf == 3 || conf == 7) {
        char_weight_size = 8;
    }

    int MATRIX_SIZE;
    printf("Enter the matrix size; the BASE_MATRIX_SIZE is %d, so enter any number in power of 2 and >= %d ", BASE_MATRIX_SIZE, BASE_MATRIX_SIZE);
    scanf("%d", &MATRIX_SIZE);

    int matrix_size = MATRIX_SIZE;

    int weight_matrix[matrix_size][matrix_size];
    int input_matrix[matrix_size][matrix_size];
    int golden_matrix[matrix_size][matrix_size];
    int strassen_matrix[matrix_size][matrix_size];

    generate_random_input_matrix(conf, matrix_size, input_matrix);
    generate_random_weight_matrix(conf, matrix_size, weight_matrix);
    
    printf("Input Matrix: \n");
    print_matrix(matrix_size, input_matrix);

    //printf("Weight Matrix: \n");
    //print_matrix(matrix_size, weight_matrix);

    // Strassen multiplication
    strassen(BASE_MATRIX_SIZE, conf, matrix_size, char_input_size, char_weight_size, weight_matrix, input_matrix, strassen_matrix);


    // Golden Matrix //
    matrix_mult(matrix_size, weight_matrix, input_matrix, golden_matrix);

    // Check the result
    //int errors = 0;

    for (int i = 0; i < matrix_size; i++) {
        for (int j = 0; j < matrix_size; j++) {
            if (golden_matrix[i][j] != strassen_matrix[i][j]) {
                errors++;
            }
        }
    }

    printf("Errors: %d\n", errors);

	printf("\n====== %s ======\n\n", cfg_100[0].devname);
	/* <<--print-params-->> */
	printf("  .reg5 = %d\n", reg5);
	printf("  .reg4 = %d\n", reg4);
	printf("  .reg7 = %d\n", reg7);
	printf("  .reg6 = %d\n", reg6);
	printf("  .reg1 = %d\n", reg1);
	printf("  .reg3 = %d\n", reg3);
	printf("  .reg2 = %d\n", reg2);
	printf("  .reg9 = %d\n", reg9);
	printf("  .reg8 = %d\n", reg8);
	printf("  .reg10 = %d\n", reg10);
	printf("\n  ** START **\n");

	//esp_run(cfg_007, NACC);

	printf("\n  ** DONE **\n");

	//errors = validate_buffer(&buf[out_offset], gold);

	//free(gold);
	//esp_free(buf);

	/*
	if (!errors)
		printf("+ Test PASSED\n");
	else
		printf("+ Test FAILED\n");

	printf("\n====== %s ======\n\n", cfg_007[0].devname);

	return errors;
	*/

}
