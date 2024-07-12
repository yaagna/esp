// Copyright (c) 2011-2024 Columbia University, System Level Design Group
// SPDX-License-Identifier: Apache-2.0
#include "libesp.h"
#include "cfg_int8x8.h"
//#include "fuseml.h"


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

#define SIZE 2

// Function to generate a random matrix of size 2x2
void generate_random_matrix(int matrix[SIZE][SIZE]) {
    // Seed the random number generator with millisecond precision
    srand (time(NULL) * 1000 + clock());
    // srand((unsigned int)time(NULL));
    // give a test seed for random number generation
    //srand(1);

    // Fill the matrix with random values between 0 and 255
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            matrix[i][j] = rand() % 256;
        }
    }
}

// Function to generate trasnpose of an input matrix
void transpose(int matrix[SIZE][SIZE], int transpose_matrix[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            transpose_matrix[j][i] = matrix[i][j];
        }
    }
}

// Function to multiply two matrices
void matrix_mult(int matrix1[SIZE][SIZE], int matrix2[SIZE][SIZE], int result[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            result[i][j] = 0;
            for (int k = 0; k < SIZE; k++) {
                result[i][j] += matrix1[i][k] * matrix2[k][j];
            }
        }
    }
}

// Function to print the matrix
void print_matrix(int matrix[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

// Function to print the matrix in binary
void print_matrix_b(char matrix[SIZE][SIZE][8]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            printf("At position %d, %d: \n", i, j);
            for (int k = 0; k < 8; k++){
                printf("%d", matrix[i][j][k]);
            }
            printf("\n");
        }
    }
}

// Function to convert decimal to binary
void d2b(int decimal, char binary[]){
    int i = 0;
    // Convert decimal to binary
    for (i = 7; i >= 0; i--){
        binary[i] = decimal % 2;
        decimal = decimal / 2;
    }
}

// Function to convert matrix from decimal to binary
void matrix_d2b(int decimal_matrix[SIZE][SIZE], char binary_matrix[SIZE][SIZE][8]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            d2b(decimal_matrix[i][j], binary_matrix[i][j]);
        }
    }
}

/*
Do the transformation from a1a2a3a4a5a6a7a8 to a7a8a5a6a7a8a5a6_a3 a4 a1 a2 a3 a4 a1 a2_a7 a8 a5 a6 a7 a8 a5 a6_a3 a4 a1 a2 a3 a4 a1 a2
               Location -> 0 1 2 3 4 5 6 7     0 1 2 3 4 5 6 7  8  9  10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31
On all entries of the weight matrix
*/
void weight_transform(char input[8], char output[32]){
    for (int i = 0; i < 8; i++){
        if (i == 0) // for a1
            output[30] = output[26] = output[14] = output[10] = input[i];
        else if (i == 1) // for a2
            output[31] = output[27] = output[15] = output[11] = input[i];
        else if (i == 2) // for a3
            output[28] = output[24] = output[12] = output[8] = input[i];
        else if (i == 3) // for a4
            output[29] = output[25] = output[13] = output[9] = input[i];
        else if (i == 4) // for a5
            output[22] = output[18] = output[6] = output[2] = input[i];
        else if (i == 5) // for a6
            output[23] = output[19] = output[7] = output[3] = input[i];
        else if (i == 6) // for a7
            output[20] = output[16] = output[4] = output[0] = input[i];
        else if (i == 7) // for a8
            output[21] = output[17] = output[5] = output[1] = input[i];
    }
}

/* 
Do the transformation from a1a2a3a4a5a6a7a8 to a7a8a7a8a5a6a5a6_a7a8 a7 a8 a5 a6 a5 a6_a3 a4 a3 a4 a1 a2 a1 a2_a3 a4 a3 a4 a1 a2 a1 a2
               Location -> 0 1 2 3 4 5 6 7     0 1 2 3 4 5 6 7  8 9 10 11  12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31
On all entries of the input matrix
*/
void input_transform(char input[8], char output[32]){
    for (int i = 0; i < 8; i++){
        if (i == 0) // for a1
            output[30] = output[28] = output[22] = output[20] = input[i];
        else if (i == 1) // for a2
            output[31] = output[29] = output[23] = output[21] = input[i];
        else if (i == 2) // for a3
            output[26] = output[24] = output[18] = output[16] = input[i];
        else if (i == 3) // for a4
            output[27] = output[25] = output[19] = output[17] = input[i];
        else if (i == 4) // for a5
            output[14] = output[12] = output[6] = output[4] = input[i];
        else if (i == 5) // for a6
            output[15] = output[13] = output[7] = output[5] = input[i];
        else if (i == 6) // for a7
            output[10] = output[8] = output[2] = output[0] = input[i];
        else if (i == 7) // for a8
            output[11] = output[9] = output[3] = output[1] = input[i];
    }
}

////// complete the added part /////////////////////

/* User-defined code */
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
		for (j = 0; j < reg3; j++) { //reg3 is the output index
			printf("acc out %d gold: %d, out: %d, gold_addr: %p, out_addr: %p \n", 
					j, gold[i * out_words_adj + j], out[i * out_words_adj + j], 
					&gold[i * out_words_adj + j], &out[i * out_words_adj + j]);
			
			if (gold[i * out_words_adj + j] != out[i * out_words_adj + j])
				errors++;
	
	}
	return errors;

}


/* User-defined code */
static void init_buffer(token_t *in, token_t * gold)
{
	int i;
	int j;





    /////////////////// Added Part ///////////////////

    int weight_matrix[SIZE][SIZE];
    int input_matrix[SIZE][SIZE];
    int golden_matrix[SIZE][SIZE];

    // For Weight Matrix
    char output_weight_matrix[SIZE][SIZE][8];
    char transformed_weight_matrix[SIZE][SIZE][32];

    // For Input Matrix
    int transpose_input_matrix[SIZE][SIZE];
    char output_input_matrix[SIZE][SIZE][8];
    char transformed_input_matrix[SIZE][SIZE][32];

    // Generate random matrices
    generate_random_matrix(weight_matrix);
    generate_random_matrix(input_matrix);

    // Do transpose of input matrix
    transpose(input_matrix, transpose_input_matrix);

    // Generate the golden matrix
    matrix_mult(weight_matrix, input_matrix, golden_matrix);

    // Print the random matrix and golden matrix
    printf("Weight Matrix is: \n");
    print_matrix(weight_matrix);
    printf("\n Input Matrix is: \n");
    print_matrix(input_matrix);
    //printf("\n Transpose Input Matrix is: \n");
    //print_matrix(transpose_input_matrix);
    printf("\n Golden Matrix is: \n");
    print_matrix(golden_matrix);

    // Convert the weight matrix to binary
    matrix_d2b(weight_matrix, output_weight_matrix);
    //printf("\n Weight Matrix in binary is: \n");
    //print_matrix_b(output_weight_matrix);

    // Convert the input matrix to binary
    matrix_d2b(transpose_input_matrix, output_input_matrix);
    //printf("\n Input Matrix in binary is: \n");
    //print_matrix_b(output_input_matrix);

    // Transform the weight matrix
    for (int i = 0; i < SIZE; i++){
        for (int j = 0; j < SIZE; j++){
            weight_transform(output_weight_matrix[i][j], transformed_weight_matrix[i][j]);
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
    for (int i = 0; i < SIZE; i++){
        for (int j = 0; j < SIZE; j++){
            input_transform(output_input_matrix[i][j], transformed_input_matrix[i][j]);
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
    unsigned int weights[4][32];
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
    unsigned int hex_weights[4];
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
    unsigned int inputs[4][32];
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
    unsigned int hex_inputs[4];
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
    int32_t wbuff_0[4];
    wbuff_0[0] = hex_weights[0];
    wbuff_0[1] = hex_weights[0];
    wbuff_0[2] = 0;
    wbuff_0[3] = 0;

    printf("\n wbuff_0 is: \n");
    for (int i = 0; i < 4; i++){
        printf("%x", wbuff_0[i]);
        printf("\n");
    }

    //////////////////// wbuff_1 ////////////////////
    int32_t wbuff_1[4];
    wbuff_1[0] = 0;
    wbuff_1[1] = hex_weights[2];
    wbuff_1[2] = hex_weights[2];
    wbuff_1[3] = 0;


    printf("\n wbuff_1 is: \n");
    for (int i = 0; i < 4; i++){
        printf("%x", wbuff_1[i]);
        printf("\n");
    }


    //////////////////// wbuff_2 ////////////////////
    int32_t wbuff_2[4];
    wbuff_2[0] = 0;
    wbuff_2[1] = hex_weights[1];
    wbuff_2[2] = hex_weights[1];
    wbuff_2[3] = 0;


    printf("\n wbuff_2 is: \n");
    for (int i = 0; i < 4; i++){
        printf("%x", wbuff_2[i]);
        printf("\n");
    }


    //////////////////// wbuff_3 ////////////////////
    int32_t wbuff_3[4];
    wbuff_3[0] = 0;
    wbuff_3[1] = 0;
    wbuff_3[2] = hex_weights[3];
    wbuff_3[3] = hex_weights[3];


    printf("\n wbuff_3 is: \n");
    for (int i = 0; i < 4; i++){
        printf("%x", wbuff_3[i]);
        printf("\n");
    }


    //////////////////// ibuff_0 ////////////////////
    int32_t ibuff_0[4];
    ibuff_0[0] = hex_inputs[0];
    ibuff_0[1] = hex_inputs[2];
    ibuff_0[2] = 0;
    ibuff_0[3] = 0;


    printf("\n ibuff_0 is: \n");
    for (int i = 0; i < 4; i++){
            printf("%x", ibuff_0[i]);
        printf("\n");
    }


    //////////////////// ibuff_1 ////////////////////
    int32_t ibuff_1[4];
    ibuff_1[0] = 0;
    ibuff_1[1] = hex_inputs[1];
    ibuff_1[2] = hex_inputs[3];
    ibuff_1[3] = 0;


    printf("\n ibuff_1 is: \n");
    for (int i = 0; i < 4; i++){
            printf("%x", ibuff_1[i]);
        printf("\n");
    }


    //////////////////// input vector stream ////////////////////
    int32_t input_vector_stream[24];
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
    int32_t gold_buff_8x8[4];
    gold_buff_8x8[0] = golden_matrix[0][0];
    gold_buff_8x8[1] = golden_matrix[1][0];
    gold_buff_8x8[2] = golden_matrix[0][1];
    gold_buff_8x8[3] = golden_matrix[1][1];

    int32_t in_buffs_8x8[24];
    //int32_t gold_buff_8x8[4];
    
    for (int i = 0; i < 24; i++){
         in_buffs_8x8[i] = input_vector_stream[i];
}

    printf("\n gold_buff is: \n");
    for (int i = 0; i < 4; i++){
            printf("%d", gold_buff_8x8[i]);
        printf("\n");
    }

/////////////////// Added Part Finish ///////////////////



	for (i = 0; i < 1; i++)
		for (j = 0; j < reg2; j++)
			//in[i * in_words_adj + j] = (token_t) j;
                        in[i * in_words_adj + j] = (token_t) in_buffs_8x8[j];

	for (i = 0; i < 1; i++)
		//for (j = 0; j < 2 * reg2; j++)
                for (j = 0; j < reg3; j++)      //gold is initialized using reg3
			//gold[i * out_words_adj + j] = (token_t) j;
                        gold[i * out_words_adj + j] = (token_t) gold_buff_8x8[j];
}


/* User-defined code */
static void init_parameters()
{
	if (DMA_WORD_PER_BEAT(sizeof(token_t)) == 0) {
		in_words_adj = reg2;
		//out_words_adj = 2 * reg2;
                out_words_adj = reg3;
	} else {
		in_words_adj = round_up(reg2, DMA_WORD_PER_BEAT(sizeof(token_t)));
		//out_words_adj = round_up(reg2, DMA_WORD_PER_BEAT(sizeof(token_t)));
                out_words_adj = round_up(reg3, DMA_WORD_PER_BEAT(sizeof(token_t)));
	}
	in_len = in_words_adj * (1);
	out_len =  out_words_adj * (1);
	in_size = in_len * sizeof(token_t);
	out_size = out_len * sizeof(token_t);
	out_offset = in_len;
	size = (out_offset * sizeof(token_t)) + out_size;
}


int main(int argc, char **argv)
{
	int errors;

	token_t *gold;
	token_t *buf;

	init_parameters();

	buf = (token_t *) esp_alloc(size);
	cfg_002[0].hw_buf = buf;
    
	gold = malloc(out_size);

	init_buffer(buf, gold);

	printf("\n====== %s ======\n\n", cfg_002[0].devname);
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

	esp_run(cfg_002, NACC);

	printf("\n  ** DONE **\n");

	errors = validate_buffer(&buf[out_offset], gold);

	free(gold);
	esp_free(buf);

	if (!errors)
		printf("+ Test PASSED\n");
	else
		printf("+ Test FAILED\n");

	printf("\n====== %s ======\n\n", cfg_002[0].devname);

	return errors;
}
