// Copyright (c) 2011-2024 Columbia University, System Level Design Group
// SPDX-License-Identifier: Apache-2.0
#include "libesp.h"
#include "cfg_signed_int4x4.h"
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

#define BASE_MATRIX_SIZE 8

// Function to generate a random matrix of size 16x16
static void generate_random_matrix(int matrix_size, int matrix[matrix_size][matrix_size]) {
    // Seed the random number generator with millisecond precision
    srand (time(NULL) * 1000 + clock());
    // srand((unsigned int)time(NULL));
    //srand(1);

    // Fill the matrix with random values between -8 and 7
    for (int i = 0; i < matrix_size; i++) {
        for (int j = 0; j < matrix_size; j++) {
            matrix[i][j] = rand() % 16 - 8;
        }
    }
}

// Function to generate trasnpose of an input matrix
static void transpose(int matrix_size, int matrix[matrix_size][matrix_size], int transpose_matrix[matrix_size][matrix_size]) {
    for (int i = 0; i < matrix_size; i++) {
        for (int j = 0; j < matrix_size; j++) {
            transpose_matrix[j][i] = matrix[i][j];
        }
    }
}

// Function to print the matrix
static void print_matrix(int matrix_size, int matrix[matrix_size][matrix_size]) {
    for (int i = 0; i < matrix_size; i++) {
        for (int j = 0; j < matrix_size; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

// Function to print the matrix in binary

static void print_matrix_b(int matrix_size, char matrix[matrix_size][matrix_size][4]) {
    for (int i = 0; i < matrix_size; i++) {
        for (int j = 0; j < matrix_size; j++) {
            printf("At position %d, %d: \n", i, j);
            for (int k = 0; k < 4; k++){
                printf("%d", matrix[i][j][k]);
            }
            printf("\n");
        }
    }
}


// Function to print the transformed matrix in binary

static void print_transformed_matrix_b(int matrix_size, char matrix[matrix_size][matrix_size][8]) {
    for (int i = 0; i < matrix_size; i++) {
        for (int j = 0; j < matrix_size; j++) {
            printf("At position %d, %d: \n", i, j);
            for (int k = 0; k < 8; k++){
                printf("%d", matrix[i][j][k]);
            }
            printf("\n");
        }
    }
}


// Function to multiply two matrices
static void matrix_mult(int matrix_size, int matrix1[matrix_size][matrix_size], int matrix2[matrix_size][matrix_size], int result[matrix_size][matrix_size]) {
    for (int i = 0; i < matrix_size; i++) {
        for (int j = 0; j < matrix_size; j++) {
            result[i][j] = 0;
            for (int k = 0; k < matrix_size; k++) {
                result[i][j] += matrix1[i][k] * matrix2[k][j];
            }
        }
    }
}

// Function to convert decimal to binary
static void d2b(int decimal, char binary[]){
    int i = 0;
    // Convert decimal to binary
    for (i = 3; i >= 0; i--){
        binary[i] = (decimal & 1) ? 1 : 0;
        decimal >>= 1;
    }
}
// Function to convert matrix from decimal to binary
static void matrix_d2b(int matrix_size, int decimal_matrix[matrix_size][matrix_size], char binary_matrix[matrix_size][matrix_size][4]) {
    for (int i = 0; i < matrix_size; i++) {
        for (int j = 0; j < matrix_size; j++) {
            d2b(decimal_matrix[i][j], binary_matrix[i][j]);
        }
    }
}

// Do the transformation from MSB4 MSB3 MSB2 MSB1 ->  MSB4 MSB3 MSB4 MSB3 MSB2 MSB1 MSB2 MSB1 on Weight Matrix entries
// Function for weight transformation
void weight_transform(char input[4], char output[8]){
    for (int i = 0; i < 2; ++i) {
        output[6 + i] = input[2 + i];
    }
    for (int i = 0; i < 2; i++){
        output[4 + i] = input[2 + i];
    }
    for (int i = 0; i < 2; i++){
        output[2 + i] = input[i];
    }
    for (int i = 0; i < 2; i++){
        output[i] = input[i];
    }

}

// Do the transformation for input matrix from MSB4 MSB3 MSB2 MSB1 -> MSB2 MSB1 MSB4 MSB3 MSB2 MSB1 MSB4 MSB3
// Function for input transformation
void input_transform(char input[4], char output[8]){
    for (int i = 0; i < 2; ++i) {
        output[6 + i] = input[i];
    }
    for (int i = 0; i < 2; i++){
        output[4 + i] = input[i+2];
    }
    for (int i = 0; i < 2; i++){
        output[2 + i] = input[i];
    }
    for (int i = 0; i < 2; i++){
        output[i] = input[i+2];
    }
}

// Weight_0 it contains first 4 elements of 1st, 3rd, 5th and so on row of weight matrix
// So weight_0 will have 4 entries each 32 bits wide
// Weight_0 entry 1 = concatenation of binary_matrix[0][3],binary_matrix[0][2],binary_matrix[0][1],binary_matrix[0][0]
/* 
So the below function will take transformed_matrix and then strore 1st 4 elements of 1st row into weight_0[0]
and 1st 4 elements of 3rd row into weight_0[1] and so on
*/
void weight_0(char input[4][8], char output[32]){
    // output[24 to 31] = input[0][8]
    // output[16 to 23] = input[1][8]
    // output[8 to 15] = input[2][8]
    // output[0 to 7] = input[3][8]
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 8; j++){
            if(i == 0){
                output[24+j] = input[i][j];
            }
            else if(i == 1){
                output[16+j] = input[i][j];
            }
            else if(i == 2){
                output[8+j] = input[i][j];
            }
            else if(i == 3){
                output[j] = input[i][j];
            }
        }
    }
}

// Weight_2 it contains second 4 elements of 1st, 3rd and 5th row of weight matrix
// So weight_2 will have 4 entries each 32 bits wide
// Weight_2 entry 1 = concatenation of binary_matrix[0][7],binary_matrix[0][6],binary_matrix[0][5],binary_matrix[0][4]
/* 
So the below function will take transformed_matrix and then strore 1st 4 elements of 1st row into weight_2[0]
and 1st 4 elements of 3rd row into weight_2[1] and so on
*/
void weight_2(char input[4][8], char output[32]){
    // output[24 to 31] = input[0][8]
    // output[16 to 23] = input[1][8]
    // output[8 to 15] = input[2][8]
    // output[0 to 7] = input[3][8]
    for (int i = 4; i < 8; i++){
        for (int j = 0; j < 8; j++){
            if(i == 4){
                output[24+j] = input[i][j];
            }
            else if(i == 5){
                output[16+j] = input[i][j];
            }
            else if(i == 6){
                output[8+j] = input[i][j];
            }
            else if(i == 7){
                output[j] = input[i][j];
            }
        }
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
static void init_buffer(int32_t in_buffs_4x4[204],  token_t *in)
{
	int i;
	int j;



	for (i = 0; i < 1; i++)
		for (j = 0; j < reg2; j++)
			//in[i * in_words_adj + j] = (token_t) j;
                        in[i * in_words_adj + j] = (token_t) in_buffs_4x4[j];

	//for (i = 0; i < 1; i++)
		//for (j = 0; j < 2 * reg2; j++)
          //      for (j = 0; j < reg3; j++)      //gold is initialized using reg3
			//gold[i * out_words_adj + j] = (token_t) j;
            //            gold[i * out_words_adj + j] = (token_t) gold_buff_4x4[j];
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

static void copy_result (token_t *out, int gold[64]) {

	for (int i = 0; i < 1; i++)
		for (int j = 0; j < reg3; j++) { //reg3 is the output index
			gold[j] = out[i * out_words_adj + j];
		}

}

static void mult(int matrix_size, int weight_matrix[matrix_size][matrix_size], int input_matrix[matrix_size][matrix_size], int result_matrix[matrix_size][matrix_size]){

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
        

    matrix_d2b(matrix_size, weight_matrix, output_weight_matrix);

    // Transform the weight matrix
    for (int i = 0; i < matrix_size; i++){
        for (int j = 0; j < matrix_size; j++){
            weight_transform(output_weight_matrix[i][j], transformed_weight_matrix[i][j]);
        }
    }

    //print_transformed_matrix_b(matrix_size, transformed_weight_matrix);

    matrix_d2b(matrix_size, transpose_input_matrix, output_input_matrix);

    // Transform the input matrix
    for (int i = 0; i < matrix_size; i++){
        for (int j = 0; j < matrix_size; j++){
            input_transform(output_input_matrix[i][j], transformed_input_matrix[i][j]);
        }
    }

    ////////////////////////////// Weight_0 //////////////////////////////
    char weight_input_0[4][32] = {0};
    weight_0(transformed_weight_matrix[0], weight_input_0[0]);
    weight_0(transformed_weight_matrix[2], weight_input_0[1]);
    weight_0(transformed_weight_matrix[4], weight_input_0[2]);
    weight_0(transformed_weight_matrix[6], weight_input_0[3]);

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
    weight_2(transformed_weight_matrix[0], weight_input_2[0]);
    weight_2(transformed_weight_matrix[2], weight_input_2[1]);
    weight_2(transformed_weight_matrix[4], weight_input_2[2]);
    weight_2(transformed_weight_matrix[6], weight_input_2[3]);

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
    weight_0(transformed_weight_matrix[1], weight_input_1[0]);
    weight_0(transformed_weight_matrix[3], weight_input_1[1]);
    weight_0(transformed_weight_matrix[5], weight_input_1[2]);
    weight_0(transformed_weight_matrix[7], weight_input_1[3]);

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
    weight_2(transformed_weight_matrix[1], weight_input_3[0]);
    weight_2(transformed_weight_matrix[3], weight_input_3[1]);
    weight_2(transformed_weight_matrix[5], weight_input_3[2]);
    weight_2(transformed_weight_matrix[7], weight_input_3[3]);

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
    weight_0(transformed_input_matrix[0], input_0[0]);
    weight_0(transformed_input_matrix[1], input_0[1]);
    weight_0(transformed_input_matrix[2], input_0[2]);
    weight_0(transformed_input_matrix[3], input_0[3]);
    weight_0(transformed_input_matrix[4], input_0[4]);
    weight_0(transformed_input_matrix[5], input_0[5]);
    weight_0(transformed_input_matrix[6], input_0[6]);
    weight_0(transformed_input_matrix[7], input_0[7]);

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
    weight_2(transformed_input_matrix[0], input_1[0]);
    weight_2(transformed_input_matrix[1], input_1[1]);
    weight_2(transformed_input_matrix[2], input_1[2]);
    weight_2(transformed_input_matrix[3], input_1[3]);
    weight_2(transformed_input_matrix[4], input_1[4]);
    weight_2(transformed_input_matrix[5], input_1[5]);
    weight_2(transformed_input_matrix[6], input_1[6]);
    weight_2(transformed_input_matrix[7], input_1[7]);

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

/*
    printf("In_buffs \n");
    for (int i = 0; i < 204; i++) {
	printf("%x \n ", input_vector_stream[i]);
    };
*/

    token_t *gold;
    token_t *buf;

    init_parameters();

    buf = (token_t *) esp_alloc(size);
    cfg_005[0].hw_buf = buf;

    gold = malloc(out_size);

    init_buffer(in_buffs_4x4, buf);
    esp_run(cfg_005, NACC);

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

/*
	printf("\n Each run is \n");
	for (int i = 0; i < 64; i++){
		printf( "%d ", each_run[i]);	
	}
*/

     for (int i = 0; i < 64; i++){
	each_run[i] = 0;
     }

	//printf("\n Golden_matrix \n");
	//print_matrix(matrix_size, golden_matrix);
	//printf("\n Result_matrix \n");
	//print_matrix(matrix_size, result_matrix);

/*
    for (int i = 0; i < 64; i++){
        gold_buff_4x4[i] = golden_output[i];
    }
*/

    // Print the random matrix and golden matrix
    //printf("Weight Matrix is: \n");
    //print_matrix(weight_matrix);
    //printf("\n Input Matrix is: \n");
    //print_matrix(input_matrix);
    //printf("\n Transpose Input Matrix is: \n");
    //print_matrix(transpose_input_matrix);
    //printf("\n Golden Matrix is: \n");
    //print_matrix(golden_matrix);

/*
    printf(" \n Input_vector_stream is: \n");
    for (int i = 0; i < 204; i++){
        printf("%x\n", in_buffs_4x4[i]);
    }
    printf("\n Golden Output is: \n");
    for (int i = 0; i < 64; i++){
        printf("%d\n", gold_buff_4x4[i]);
    }
*/
    /////////////// Added Complete ///////////////

};

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

// This is the generic strassen function
static void strassen(int matrix_size, int matrix1[matrix_size][matrix_size], int matrix2[matrix_size][matrix_size], int result_matrix[matrix_size][matrix_size]) {

    // matrix1 is the weight matrix
    // matrix2 is the input matrix

    if (matrix_size == BASE_MATRIX_SIZE) {
        mult(BASE_MATRIX_SIZE, matrix1, matrix2, result_matrix);
	print_matrix(matrix_size, result_matrix);
	printf("\n If statement executed first \n");
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
            mult(BASE_MATRIX_SIZE, a, e, ae);
	    printf("\n ae executed \n");
            mult(BASE_MATRIX_SIZE, a, f, af);
	    printf("\n af executed \n");
            mult(BASE_MATRIX_SIZE, b, g, bg);
	    printf("\n bg executed \n");
            mult(BASE_MATRIX_SIZE, b, h, bh);
	    printf("\n bh executed \n");
            mult(BASE_MATRIX_SIZE, c, e, ce);
	    printf("\n ce executed \n");
            mult(BASE_MATRIX_SIZE, c, f, cf);
	    printf("\n cf executed \n");
            mult(BASE_MATRIX_SIZE, d, g, dg);
	    printf("\n dg executed \n");
            mult(BASE_MATRIX_SIZE, d, h, dh);
	    printf("\n dh executed \n");
        }
        else {
	    printf("\n else executed \n");
            strassen(submatrix_size, a, e, ae);
            strassen(submatrix_size, a, f, af);
            strassen(submatrix_size, b, g, bg);
            strassen(submatrix_size, b, h, bh);
            strassen(submatrix_size, c, e, ce);
            strassen(submatrix_size, c, f, cf);
            strassen(submatrix_size, d, g, dg);
            strassen(submatrix_size, d, h, dh);
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
	int errors;

    /// Added ///
   
    int MATRIX_SIZE;
    printf("Enter the matrix size; the BASE_MATRIX_SIZE is 8 so enter any number in power of 2 and >= 8: ");
    scanf("%d", &MATRIX_SIZE);
    int matrix_size = MATRIX_SIZE;

    int weight_matrix[matrix_size][matrix_size];
    int input_matrix[matrix_size][matrix_size];
    int golden_matrix[matrix_size][matrix_size];
    int strassen_matrix[matrix_size][matrix_size];

for (int i = 0; i < 51; i++) {

    // Generate the random matrix
    generate_random_matrix(matrix_size, weight_matrix);
    generate_random_matrix(matrix_size, input_matrix);


    /// Added Complete ///

	//token_t *gold;
	//token_t *buf;

	//init_parameters();

//for (int i = 0; i < 51; i++) {

	//buf = (token_t *) esp_alloc(size);
	//cfg_005[0].hw_buf = buf;
    
	//gold = malloc(out_size);

	//init_buffer(buf, gold);

	strassen(matrix_size, weight_matrix, input_matrix, strassen_matrix);

	clock_t b = clock();
	matrix_mult(matrix_size, weight_matrix, input_matrix, golden_matrix);
	clock_t e = clock();
	double cpu_time_used = ((double) (e - b)) / CLOCKS_PER_SEC;
   	printf("\n");
    	//Print the time taken
    	printf("Time taken for Strassen matrix multiplication in CPU: %f\n", cpu_time_used);

    // Check if the results are the same
    int error = 0;
    for (int i = 0; i < matrix_size; i++) {
        for (int j = 0; j < matrix_size; j++) {
            if (golden_matrix[i][j] != strassen_matrix[i][j]) {
                error = 1;
                break;
            }
        }
    }


    printf("\n Errors: %d\n", error);

	printf("\n====== %s ======\n\n", cfg_005[0].devname);
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

	//esp_run(cfg_005, NACC);

	print_matrix(matrix_size, golden_matrix);

	printf("\n  ** DONE **\n");

	//errors = validate_buffer(&buf[out_offset], gold);

	//free(gold);
	//esp_free(buf);

/*
	if (!errors)
		printf("+ Test PASSED\n");
	else
		printf("+ Test FAILED\n");
*/

	printf("\n====== %s ======\n\n", cfg_005[0].devname);
//}
}
	return errors;

}
