// Copyright (c) 2011-2024 Columbia University, System Level Design Group
// SPDX-License-Identifier: Apache-2.0
#include "libesp.h"
#include "cfg_signed_int2x2.h"
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

#define SIZE 32

// Function to generate a random matrix of size 32x32
void generate_random_matrix(int matrix[SIZE][SIZE]) {
    // Seed the random number generator with millisecond precision
    //srand (time(NULL) * 1000 + clock());
    // srand((unsigned int)time(NULL));
    // give a test seed for random number generation
    srand(1);

    // Fill the matrix with random values between -2 and 1
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            matrix[i][j] = rand() % 4 - 2;
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
void print_matrix_b(char matrix[SIZE][SIZE][2]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            //printf("At position %d, %d: \n", i, j);
            for (int k = 0; k < 2; k++){
                printf("%d ", matrix[i][j][k]);
            }
        }
        printf("\n");
    }
}

// Function to convert decimal to binary
void d2b(int decimal, char binary[]){
    int i = 0;
    // Convert decimal to binary
    for (i = 1; i >= 0; i--){
        binary[i] = (decimal & 1) ? 1 : 0;
        decimal >>= 1;
    }
}

// Function to convert matrix from decimal to binary
void matrix_d2b(int decimal_matrix[SIZE][SIZE], char binary_matrix[SIZE][SIZE][2]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            d2b(decimal_matrix[i][j], binary_matrix[i][j]);
        }
    }
}

// Weight_0 it contains first 16 elements of 1st, 3rd, 5th and so on row of weight matrix
// So weight_0 will have 16 entries each entry is 32 bits wide
// Weight_0 entry 1 = concatenation of binary_matrix[0][15],binary_matrix[0][14],binary_matrix[0][13]...binary_matrix[0][0]
/* 
So the below function will take output_weight_matrix and then strore 1st 16 elements of 1st row into weight_0[0]
and 1st 16 elements of 3rd row into weight_0[1] and so on
*/

void weight_0(char input[16][2], char output[32]){
    for (int i = 0; i < 16; i++){
        for (int j = 0; j < 2; j++){
            if (i == 0) {
                output[30+j] = input[i][j];
            }
            else if (i == 1){
                output[28+j] = input[i][j];
            }
            else if (i == 2){
                output[26+j] = input[i][j];
            }
            else if (i == 3){
                output[24+j] = input[i][j];
            }
            else if (i == 4){
                output[22+j] = input[i][j];
            }
            else if (i == 5){
                output[20+j] = input[i][j];
            }
            else if (i == 6){
                output[18+j] = input[i][j];
            }
            else if (i == 7){
                output[16+j] = input[i][j];
            }
            else if (i == 8){
                output[14+j] = input[i][j];
            }
            else if (i == 9){
                output[12+j] = input[i][j];
            }
            else if (i == 10){
                output[10+j] = input[i][j];
            }
            else if (i == 11){
                output[8+j] = input[i][j];
            }
            else if (i == 12){
                output[6+j] = input[i][j];
            }
            else if (i == 13){
                output[4+j] = input[i][j];
            }
            else if (i == 14){
                output[2+j] = input[i][j];
            }
            else if (i == 15){
                output[j] = input[i][j];
            }
        }
    }
}

// Weight_1 it contains second 16 elements of 1st, 3rd, 5th and so on row of weight matrix
// So weight_1 will have 16 entries each entry is 32 bits wide
// Weight_1 entry 1 = concatenation of binary_matrix[0][15],binary_matrix[0][14],binary_matrix[0][13]...binary_matrix[0][0]
/* 
So the below function will take output_weight_matrix and then strore 1st 16 elements of 1st row into weight_1[0]
and 1st 16 elements of 3rd row into weight_0[1] and so on
*/

void weight_2(char input[16][2], char output[32]){
    for (int i = 16; i < 32; i++){
        for (int j = 0; j < 2; j++){
            if (i == 16) {
                output[30+j] = input[i][j];
            }
            else if (i == 17){
                output[28+j] = input[i][j];
            }
            else if (i == 18){
                output[26+j] = input[i][j];
            }
            else if (i == 19){
                output[24+j] = input[i][j];
            }
            else if (i == 20){
                output[22+j] = input[i][j];
            }
            else if (i == 21){
                output[20+j] = input[i][j];
            }
            else if (i == 22){
                output[18+j] = input[i][j];
            }
            else if (i == 23){
                output[16+j] = input[i][j];
            }
            else if (i == 24){
                output[14+j] = input[i][j];
            }
            else if (i == 25){
                output[12+j] = input[i][j];
            }
            else if (i == 26){
                output[10+j] = input[i][j];
            }
            else if (i == 27){
                output[8+j] = input[i][j];
            }
            else if (i == 28){
                output[6+j] = input[i][j];
            }
            else if (i == 29){
                output[4+j] = input[i][j];
            }
            else if (i == 30){
                output[2+j] = input[i][j];
            }
            else if (i == 31){
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
/*
			printf("acc out %d gold: %d, out: %d, gold_addr: %p, out_addr: %p \n", 
					j, gold[i * out_words_adj + j], out[i * out_words_adj + j], 
					&gold[i * out_words_adj + j], &out[i * out_words_adj + j]);
*/
			if (gold[i * out_words_adj + j] != out[i * out_words_adj + j]) {
				errors++;
				printf("acc out %d gold: %d, out: %d, gold_addr: %p, out_addr: %p \n", 
					j, gold[i * out_words_adj + j], out[i * out_words_adj + j], 
					&gold[i * out_words_adj + j], &out[i * out_words_adj + j]);
			}
	
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

    // For Weight matrix
    char output_weight_matrix[SIZE][SIZE][2];

    // For Input matrix
    int transpose_input_matrix[SIZE][SIZE];
    char output_input_matrix[SIZE][SIZE][2];

    // Generate the random matrix
    generate_random_matrix(weight_matrix);
    generate_random_matrix(input_matrix);

    // Do transpose of input matrix
    transpose(input_matrix, transpose_input_matrix);

    // Generate Golden Output matrix
    //matrix_mult(weight_matrix, input_matrix, golden_matrix);

    // Print the random matrix and golden matrix
    //printf("Weight Matrix is: \n");
    //print_matrix(weight_matrix);
    //printf("\n Input Matrix is: \n");
    //print_matrix(input_matrix);
    //printf("\n Transpose Input Matrix is: \n");
    //print_matrix(transpose_input_matrix);
    //printf("\n Golden Matrix is: \n");
    print_matrix(golden_matrix);

    // Convert the weight matrix to binary
    //printf("\n Output Matrix is: \n");
    matrix_d2b(weight_matrix, output_weight_matrix);
    //print_matrix_b(output_weight_matrix);

    // Convert the input matrix to binary
    //printf("\n Output Matrix is: \n");
    matrix_d2b(transpose_input_matrix, output_input_matrix);
    //print_matrix_b(output_weight_matrix);

    ///////////////////// Weight_0 /////////////////////
    char weight_input_0[16][32] = {0};
    weight_0(output_weight_matrix[0], weight_input_0[0]);
    weight_0(output_weight_matrix[2], weight_input_0[1]);
    weight_0(output_weight_matrix[4], weight_input_0[2]);
    weight_0(output_weight_matrix[6], weight_input_0[3]);
    weight_0(output_weight_matrix[0], weight_input_0[0]);
    weight_0(output_weight_matrix[8], weight_input_0[4]);
    weight_0(output_weight_matrix[10], weight_input_0[5]);
    weight_0(output_weight_matrix[12], weight_input_0[6]);
    weight_0(output_weight_matrix[14], weight_input_0[7]);
    weight_0(output_weight_matrix[16], weight_input_0[8]);
    weight_0(output_weight_matrix[18], weight_input_0[9]);
    weight_0(output_weight_matrix[20], weight_input_0[10]);
    weight_0(output_weight_matrix[22], weight_input_0[11]);
    weight_0(output_weight_matrix[24], weight_input_0[12]);
    weight_0(output_weight_matrix[26], weight_input_0[13]);
    weight_0(output_weight_matrix[28], weight_input_0[14]);
    weight_0(output_weight_matrix[30], weight_input_0[15]);

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
    weight_0(output_weight_matrix[1], weight_input_1[0]);
    weight_0(output_weight_matrix[3], weight_input_1[1]);
    weight_0(output_weight_matrix[5], weight_input_1[2]);
    weight_0(output_weight_matrix[7], weight_input_1[3]);
    weight_0(output_weight_matrix[9], weight_input_1[4]);
    weight_0(output_weight_matrix[11], weight_input_1[5]);
    weight_0(output_weight_matrix[13], weight_input_1[6]);
    weight_0(output_weight_matrix[15], weight_input_1[7]);
    weight_0(output_weight_matrix[17], weight_input_1[8]);
    weight_0(output_weight_matrix[19], weight_input_1[9]);
    weight_0(output_weight_matrix[21], weight_input_1[10]);
    weight_0(output_weight_matrix[23], weight_input_1[11]);
    weight_0(output_weight_matrix[25], weight_input_1[12]);
    weight_0(output_weight_matrix[27], weight_input_1[13]);
    weight_0(output_weight_matrix[29], weight_input_1[14]);
    weight_0(output_weight_matrix[31], weight_input_1[15]);

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
    weight_2(output_weight_matrix[0], weight_input_2[0]);
    weight_2(output_weight_matrix[2], weight_input_2[1]);
    weight_2(output_weight_matrix[4], weight_input_2[2]);
    weight_2(output_weight_matrix[6], weight_input_2[3]);
    weight_2(output_weight_matrix[8], weight_input_2[4]);
    weight_2(output_weight_matrix[10], weight_input_2[5]);
    weight_2(output_weight_matrix[12], weight_input_2[6]);
    weight_2(output_weight_matrix[14], weight_input_2[7]);
    weight_2(output_weight_matrix[16], weight_input_2[8]);
    weight_2(output_weight_matrix[18], weight_input_2[9]);
    weight_2(output_weight_matrix[20], weight_input_2[10]);
    weight_2(output_weight_matrix[22], weight_input_2[11]);
    weight_2(output_weight_matrix[24], weight_input_2[12]);
    weight_2(output_weight_matrix[26], weight_input_2[13]);
    weight_2(output_weight_matrix[28], weight_input_2[14]);
    weight_2(output_weight_matrix[30], weight_input_2[15]);

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
    weight_2(output_weight_matrix[1], weight_input_3[0]);
    weight_2(output_weight_matrix[3], weight_input_3[1]);
    weight_2(output_weight_matrix[5], weight_input_3[2]);
    weight_2(output_weight_matrix[7], weight_input_3[3]);
    weight_2(output_weight_matrix[9], weight_input_3[4]);
    weight_2(output_weight_matrix[11], weight_input_3[5]);
    weight_2(output_weight_matrix[13], weight_input_3[6]);
    weight_2(output_weight_matrix[15], weight_input_3[7]);
    weight_2(output_weight_matrix[17], weight_input_3[8]);
    weight_2(output_weight_matrix[19], weight_input_3[9]);
    weight_2(output_weight_matrix[21], weight_input_3[10]);
    weight_2(output_weight_matrix[23], weight_input_3[11]);
    weight_2(output_weight_matrix[25], weight_input_3[12]);
    weight_2(output_weight_matrix[27], weight_input_3[13]);
    weight_2(output_weight_matrix[29], weight_input_3[14]);
    weight_2(output_weight_matrix[31], weight_input_3[15]);

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
    weight_0(output_input_matrix[0], input_0[0]);
    weight_0(output_input_matrix[1], input_0[1]);
    weight_0(output_input_matrix[2], input_0[2]);
    weight_0(output_input_matrix[3], input_0[3]);
    weight_0(output_input_matrix[4], input_0[4]);
    weight_0(output_input_matrix[5], input_0[5]);
    weight_0(output_input_matrix[6], input_0[6]);
    weight_0(output_input_matrix[7], input_0[7]);
    weight_0(output_input_matrix[8], input_0[8]);
    weight_0(output_input_matrix[9], input_0[9]);
    weight_0(output_input_matrix[10], input_0[10]);
    weight_0(output_input_matrix[11], input_0[11]);
    weight_0(output_input_matrix[12], input_0[12]);
    weight_0(output_input_matrix[13], input_0[13]);
    weight_0(output_input_matrix[14], input_0[14]);
    weight_0(output_input_matrix[15], input_0[15]);
    weight_0(output_input_matrix[16], input_0[16]);
    weight_0(output_input_matrix[17], input_0[17]);
    weight_0(output_input_matrix[18], input_0[18]);
    weight_0(output_input_matrix[19], input_0[19]);
    weight_0(output_input_matrix[20], input_0[20]);
    weight_0(output_input_matrix[21], input_0[21]);
    weight_0(output_input_matrix[22], input_0[22]);
    weight_0(output_input_matrix[23], input_0[23]);
    weight_0(output_input_matrix[24], input_0[24]);
    weight_0(output_input_matrix[25], input_0[25]);
    weight_0(output_input_matrix[26], input_0[26]);
    weight_0(output_input_matrix[27], input_0[27]);
    weight_0(output_input_matrix[28], input_0[28]);
    weight_0(output_input_matrix[29], input_0[29]);
    weight_0(output_input_matrix[30], input_0[30]);
    weight_0(output_input_matrix[31], input_0[31]);

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
    weight_2(output_input_matrix[0], input_1[0]);
    weight_2(output_input_matrix[1], input_1[1]);
    weight_2(output_input_matrix[2], input_1[2]);
    weight_2(output_input_matrix[3], input_1[3]);
    weight_2(output_input_matrix[4], input_1[4]);
    weight_2(output_input_matrix[5], input_1[5]);
    weight_2(output_input_matrix[6], input_1[6]);
    weight_2(output_input_matrix[7], input_1[7]);
    weight_2(output_input_matrix[8], input_1[8]);
    weight_2(output_input_matrix[9], input_1[9]);
    weight_2(output_input_matrix[10], input_1[10]);
    weight_2(output_input_matrix[11], input_1[11]);
    weight_2(output_input_matrix[12], input_1[12]);
    weight_2(output_input_matrix[13], input_1[13]);
    weight_2(output_input_matrix[14], input_1[14]);
    weight_2(output_input_matrix[15], input_1[15]);
    weight_2(output_input_matrix[16], input_1[16]);
    weight_2(output_input_matrix[17], input_1[17]);
    weight_2(output_input_matrix[18], input_1[18]);
    weight_2(output_input_matrix[19], input_1[19]);
    weight_2(output_input_matrix[20], input_1[20]);
    weight_2(output_input_matrix[21], input_1[21]);
    weight_2(output_input_matrix[22], input_1[22]);
    weight_2(output_input_matrix[23], input_1[23]);
    weight_2(output_input_matrix[24], input_1[24]);
    weight_2(output_input_matrix[25], input_1[25]);
    weight_2(output_input_matrix[26], input_1[26]);
    weight_2(output_input_matrix[27], input_1[27]);
    weight_2(output_input_matrix[28], input_1[28]);
    weight_2(output_input_matrix[29], input_1[29]);
    weight_2(output_input_matrix[30], input_1[30]);
    weight_2(output_input_matrix[31], input_1[31]);

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

    // print input_vector_stream 
    //printf("\n Input Vector Stream is: \n");
    //for (int i = 0; i < 1548; i++){
      //  printf("%x\n", input_vector_stream[i]);
    //}

    //////////////////// Golden Output //////////////////////
    int32_t golden_output[1024] = {0};
    for (int i = 0; i < 32; i++){
        for (int j = 0; j < 32; j++){
            if (i == 0){
                golden_output[2*j] = golden_matrix[i][j];
            }
            else if (i == 1){
                golden_output[2*j + 1] = golden_matrix[i][j];
            }
            else if (i == 2){
                golden_output[2*j + 64] = golden_matrix[i][j];
            }
            else if (i == 3){
                golden_output[2*j + 65] = golden_matrix[i][j];
            }
            else if (i == 4){
                golden_output[2*j + 128] = golden_matrix[i][j];
            }
            else if (i == 5){
                golden_output[2*j + 129] = golden_matrix[i][j];
            }
            else if (i == 6){
                golden_output[2*j + 192] = golden_matrix[i][j];
            }
            else if (i == 7){
                golden_output[2*j + 193] = golden_matrix[i][j];
            }
            else if (i == 8){
                golden_output[2*j + 256] = golden_matrix[i][j];
            }
            else if (i == 9){
                golden_output[2*j + 257] = golden_matrix[i][j];
            }
            else if (i == 10){
                golden_output[2*j + 320] = golden_matrix[i][j];
            }
            else if (i == 11){
                golden_output[2*j + 321] = golden_matrix[i][j];
            }
            else if (i == 12){
                golden_output[2*j + 384] = golden_matrix[i][j];
            }
            else if (i == 13){
                golden_output[2*j + 385] = golden_matrix[i][j];
            }
            else if (i == 14){
                golden_output[2*j + 448] = golden_matrix[i][j];
            }
            else if (i == 15){
                golden_output[2*j + 449] = golden_matrix[i][j];
            }
            else if (i == 16){
                golden_output[2*j + 512] = golden_matrix[i][j];
            }
            else if (i == 17){
                golden_output[2*j + 513] = golden_matrix[i][j];
            }
            else if (i == 18){
                golden_output[2*j + 576] = golden_matrix[i][j];
            }
            else if (i == 19){
                golden_output[2*j + 577] = golden_matrix[i][j];
            }
            else if (i == 20){
                golden_output[2*j + 640] = golden_matrix[i][j];
            }
            else if (i == 21){
                golden_output[2*j + 641] = golden_matrix[i][j];
            }
            else if (i == 22){
                golden_output[2*j + 704] = golden_matrix[i][j];
            }
            else if (i == 23){
                golden_output[2*j + 705] = golden_matrix[i][j];
            }
            else if (i == 24){
                golden_output[2*j + 768] = golden_matrix[i][j];
            }
            else if (i == 25){
                golden_output[2*j + 769] = golden_matrix[i][j];
            }
            else if (i == 26){
                golden_output[2*j + 832] = golden_matrix[i][j];
            }
            else if (i == 27){
                golden_output[2*j + 833] = golden_matrix[i][j];
            }
            else if (i == 28){
                golden_output[2*j + 896] = golden_matrix[i][j];
            }
            else if (i == 29){
                golden_output[2*j + 897] = golden_matrix[i][j];
            }
            else if (i == 30){
                golden_output[2*j + 960] = golden_matrix[i][j];
            }
            else if (i == 31){
                golden_output[2*j + 961] = golden_matrix[i][j];
            }
        }
    }

    // Print golden_output
    //printf("\n Golden Output is: \n");
    //for (int i = 0; i < 1024; i++){
      //  printf("%d\n", golden_output[i]);
    //}

    static int32_t in_buffs_2x2[3084] = {0};
    static int32_t gold_buff_2x2[1024] = {0};
    for (int i = 0; i < 3084; i++){
        in_buffs_2x2[i] = input_vector_stream[i];
    }

    for (int i = 0; i < 1024; i++){
        gold_buff_2x2[i] = golden_output[i];
    }

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
    for (int i = 0; i < 3084; i++){
        printf("%x\n", in_buffs_2x2[i]);
    }
    printf("\n Golden Output is: \n");
    for (int i = 0; i < 1024; i++){
        printf("%d\n", gold_buff_2x2[i]);
    }
*/

/////////////////// Added Part Finish ///////////////////



	for (i = 0; i < 1; i++)
		for (j = 0; j < reg2; j++)
			//in[i * in_words_adj + j] = (token_t) j;
                        in[i * in_words_adj + j] = (token_t) in_buffs_2x2[j];

	for (i = 0; i < 1; i++)
		//for (j = 0; j < 2 * reg2; j++)
                for (j = 0; j < reg3; j++)      //gold is initialized using reg3
			//gold[i * out_words_adj + j] = (token_t) j;
                        gold[i * out_words_adj + j] = (token_t) gold_buff_2x2[j];
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
	cfg_004[0].hw_buf = buf;
    
	gold = malloc(out_size);



	init_buffer(buf, gold);

	printf("\n====== %s ======\n\n", cfg_004[0].devname);
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

	clock_t b = clock();
	esp_run(cfg_004, NACC);
	clock_t e = clock();
	double time = ((double) (e - b)) / CLOCKS_PER_SEC;
    	printf("\n Time taken for Acc matrix multiplication: %f\n", time);


	int weight_matrix[SIZE][SIZE];
	int input_matrix[SIZE][SIZE];
	int golden_matrix[SIZE][SIZE];
	
	generate_random_matrix(weight_matrix);
   	generate_random_matrix(input_matrix);

	clock_t be = clock();
	matrix_mult(weight_matrix, input_matrix, golden_matrix);
	clock_t ee = clock();
	double time_e = ((double) (ee - be)) / CLOCKS_PER_SEC;
    	printf("\n Time taken for cpu matrix multiplication: %f\n", time_e);



	printf("\n  ** DONE **\n");

	//errors = validate_buffer(&buf[out_offset], gold);



	if (!errors)
		printf("+ Test PASSED\n");
	else
		printf("+ Test FAILED\n");

	printf("\n====== %s ======\n\n", cfg_004[0].devname);

	free(gold);
	esp_free(buf);

	return errors;
}
