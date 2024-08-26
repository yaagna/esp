// Copyright (c) 2011-2024 Columbia University, System Level Design Group
// SPDX-License-Identifier: Apache-2.0
#include "libesp.h"
#include "cfg_signed_int8x2.h"
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

#define SIZE 8

// Function to generate a random Input matrix of size 8x8 with INT8 datatype
void generate_random_input_matrix(int matrix[SIZE][SIZE]) {
    // Seed the random number generator with millisecond precision
    //srand (time(NULL) * 1000 + clock());
    // srand((unsigned int)time(NULL));
    // give a test seed for random number generation
    srand(1);

    // Fill the matrix with random values between -128 and 127
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            matrix[i][j] = rand() % 256 - 128;
        }
    }
}

// Function to generate a random weight matrix of size 8x8 with INT2 datatype
void generate_random_weight_matrix(int matrix[SIZE][SIZE]) {
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

// Function to print the matrix
void print_matrix(int matrix[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

// Function to print the Input matrix in binary
void print_input_matrix_b(char matrix[SIZE][SIZE][8]) {
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

// Function to print the Weight matrix in binary
void print_weight_matrix_b(char matrix[SIZE][SIZE][2]) {
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

// Function to convert decimal to binary for input matrix 
void d2b_input(int decimal, char binary[]){
    int i = 0;
    // Convert decimal to binary
    for (i = 7; i >= 0; i--){
        binary[i] = (decimal & 1) ? 1 : 0;
        decimal >>= 1;
    }
}

// Function to convert decimal to binary for weihgt matrix
void d2b_weight(int decimal, char binary[]){
    int i = 0;
    // Convert decimal to binary
    for (i = 1; i >= 0; i--){
        binary[i] = (decimal & 1) ? 1 : 0;
        decimal >>= 1;
    }
}

// Function to convert input matrix from decimal to binary
void input_matrix_d2b(int decimal_matrix[SIZE][SIZE], char binary_matrix[SIZE][SIZE][8]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            d2b_input(decimal_matrix[i][j], binary_matrix[i][j]);
        }
    }
}

// Function to convert weight matrix from decimal to binary
void weight_matrix_d2b(int decimal_matrix[SIZE][SIZE], char binary_matrix[SIZE][SIZE][2]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            d2b_weight(decimal_matrix[i][j], binary_matrix[i][j]);
        }
    }
}

// Tranbsform input matrix. In this take the transformed input matrix and then concatenate the 8 bits of first 4 elements of a row to make a row of 32 bits
// Inputs
void transform_input(char number1[8], char number2[8], char number3[8], char number4[8], char output[32]){
    for (int i = 0; i < 8; i++){
        output[i] = number1[i];
    }
    for (int i = 0; i < 8; i++){
        output[i+8] = number2[i];
    }
    for (int i = 0; i < 8; i++){
        output[i+16] = number3[i];
    }
    for (int i = 0; i < 8; i++){
        output[i+24] = number4[i];
    }
}

/* 
Correct data orchestration for inputs
Lets say the transformed inputs are a1a2 a3a4 a5a6 a7a8 b1b2 b3 b4 b5 b6 b7 b8 c1 c2 c3 c4 c5 c6 c7 c8 d1 d2 d3 d4 d5 d6 d7 d8 it will be rearranged as
Location ->                         0 1  2 3  4 5  6 7  8 9  10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31

The new arrangement will be  d7d8 c7c8 d5d6 c5c6 b7b8 a7 a8 b5 b6 a5 a6 d3 d4 c3 c4 d1 d2 c1 c2 b3 b4 a3 a4 b1 b2 a1 a2 
Location ->                  0 1  2 3  4 5  6 7  8 9  10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31
*/
void input_orchestration(char input[32], char output[32]){
    output[0] = input[30];
    output[1] = input[31];
    output[2] = input[22];
    output[3] = input[23];
    output[4] = input[28];
    output[5] = input[29];
    output[6] = input[20];
    output[7] = input[21];
    output[8] = input[14];
    output[9] = input[15];
    output[10] = input[6];
    output[11] = input[7];
    output[12] = input[12];
    output[13] = input[13];
    output[14] = input[4];
    output[15] = input[5];
    output[16] = input[26];
    output[17] = input[27];
    output[18] = input[18];
    output[19] = input[19];
    output[20] = input[24];
    output[21] = input[25];
    output[22] = input[16];
    output[23] = input[17];
    output[24] = input[10];
    output[25] = input[11];
    output[26] = input[2];
    output[27] = input[3];
    output[28] = input[8];
    output[29] = input[9];
    output[30] = input[0];
    output[31] = input[1];
}

// Tranbsform weight matrix. In this take the weight matrix and then concatenate the 2 bits of first 4 elements of a row to make a row of 8 bits
// Inputs
void transform_weight(char number1[2], char number2[2], char number3[2], char number4[2], char output[8]){
    for (int i = 0; i < 2; i++){
        output[i] = number1[i];
    }
    for (int i = 0; i < 2; i++){
        output[i+2] = number2[i];
    }
    for (int i = 0; i < 2; i++){
        output[i+4] = number3[i];
    }
    for (int i = 0; i < 2; i++){
        output[i+6] = number4[i];
    }
}

/*
For Weights, Lets assume that the number is a1a2 b1b2 c1c2 d1d2
Location ->                                 0 1  2 3  4 5  6 7

The new arrnagement will be as d1d2 c1c2 d1d2 c1c2 b1b2 a1 a2 b1 b2 a1 a2 d1 d2 c1 c2 d1 d2 c1 c2 b1 b2 a1 a2 b1 b2 a1 a2
Location ->                    0 1  2 3  4 5  6 7  8 9  10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31
*/
void weight_32(char input[8], char output[32]){
    output[0] = output[4] = output[16] = output[20] = input[6];
    output[1] = output[5] = output[17] = output[21] = input[7];
    output[2] = output[6] = output[18] = output[22] = input[4];
    output[3] = output[7] = output[19] = output[23] = input[5];
    output[8] = output[12] = output[24] = output[28] = input[2];
    output[9] = output[13] = output[25] = output[29] = input[3];
    output[10] = output[14] = output[26] = output[30] = input[0];
    output[11] = output[15] = output[27] = output[31] = input[1];
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





    ///////// Added ///////////// 
int weight_matrix[SIZE][SIZE];
    int input_matrix[SIZE][SIZE];
    int golden_matrix[SIZE][SIZE];

    // For Weight Matrix
    char output_weight_matrix[SIZE][SIZE][2];

    // For Input Matrix
    int transpose_input_matrix[SIZE][SIZE];
    char output_input_matrix[SIZE][SIZE][8];

    // Generate random matrices
    generate_random_input_matrix(input_matrix);
    generate_random_weight_matrix(weight_matrix);

    // Do transpose of input matrix
    transpose(input_matrix, transpose_input_matrix);
    

    // Generate the golden matrix
    matrix_mult(weight_matrix, input_matrix, golden_matrix);

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
    weight_matrix_d2b(weight_matrix, output_weight_matrix);
    //printf("\n Weight Matrix in binary is: \n");
    //print_weight_matrix_b(output_weight_matrix);

    // Convert the input matrix to binary
    input_matrix_d2b(transpose_input_matrix, output_input_matrix);
    //printf("\n Input Matrix in binary is: \n");
    //print_input_matrix_b(output_input_matrix);

    ////////////////////////////// Input_0 //////////////////////////////
    
    // Transform the input matrix for Input_0
    char input_0[SIZE][32];
    for (int i = 0; i < SIZE; i++){
        transform_input(output_input_matrix[i][0], output_input_matrix[i][1], output_input_matrix[i][2], output_input_matrix[i][3], input_0[i]);
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
    char input_0_orchestrated[SIZE][32];
    for (int i = 0; i < SIZE; i++){
        input_orchestration(input_0[i], input_0_orchestrated[i]);
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
   unsigned int ibuff_0[34];
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
    char input_1[SIZE][32];
    for (int i = 0; i < SIZE; i++){
        transform_input(output_input_matrix[i][4], output_input_matrix[i][5], output_input_matrix[i][6], output_input_matrix[i][7], input_1[i]);
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
    char input_1_orchestrated[SIZE][32];
    for (int i = 0; i < SIZE; i++){
        input_orchestration(input_1[i], input_1_orchestrated[i]);
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
   unsigned int ibuff_1[34];
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
    char weight_0[4][8];
    transform_weight(output_weight_matrix[0][0], output_weight_matrix[0][1], output_weight_matrix[0][2], output_weight_matrix[0][3], weight_0[0]);
    transform_weight(output_weight_matrix[2][0], output_weight_matrix[2][1], output_weight_matrix[2][2], output_weight_matrix[2][3], weight_0[1]);
    transform_weight(output_weight_matrix[4][0], output_weight_matrix[4][1], output_weight_matrix[4][2], output_weight_matrix[4][3], weight_0[2]);
    transform_weight(output_weight_matrix[6][0], output_weight_matrix[6][1], output_weight_matrix[6][2], output_weight_matrix[6][3], weight_0[3]);
    printf("\n Weight_0 is: \n");
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 8; j++){
            printf("%d", weight_0[i][j]);
        }
        printf("\n");
    }

    // Expand the weight_0 matrix to 32 bits
    char weight_0_32[4][32];
    weight_32(weight_0[0], weight_0_32[0]);
    weight_32(weight_0[1], weight_0_32[1]);
    weight_32(weight_0[2], weight_0_32[2]);
    weight_32(weight_0[3], weight_0_32[3]); 
    printf("\n Weight_0 for 32bits is: \n");
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 32; j++){
            printf("%d", weight_0_32[i][j]);
        }
        printf("\n");
    }

    // Convert to hexadecimal
    unsigned int weight_hexadecimal_input_0[4];
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
    unsigned int wbuff_0[34];
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
    char weight_2[4][8];
    transform_weight(output_weight_matrix[0][4], output_weight_matrix[0][5], output_weight_matrix[0][6], output_weight_matrix[0][7], weight_2[0]);
    transform_weight(output_weight_matrix[2][4], output_weight_matrix[2][5], output_weight_matrix[2][6], output_weight_matrix[2][7], weight_2[1]);
    transform_weight(output_weight_matrix[4][4], output_weight_matrix[4][5], output_weight_matrix[4][6], output_weight_matrix[4][7], weight_2[2]);
    transform_weight(output_weight_matrix[6][4], output_weight_matrix[6][5], output_weight_matrix[6][6], output_weight_matrix[6][7], weight_2[3]);
    printf("\n Weight_2 is: \n");
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 8; j++){
            printf("%d", weight_2[i][j]);
        }
        printf("\n");
    }

    // Expand the weight_2 matrix to 32 bits
    char weight_2_32[4][32];
    weight_32(weight_2[0], weight_2_32[0]);
    weight_32(weight_2[1], weight_2_32[1]);
    weight_32(weight_2[2], weight_2_32[2]);
    weight_32(weight_2[3], weight_2_32[3]); 
    printf("\n Weight_2 for 32bits is: \n");
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 32; j++){
            printf("%d", weight_2_32[i][j]);
        }
        printf("\n");
    }

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
    unsigned int wbuff_2[34];
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
    char weight_1[4][8];
    transform_weight(output_weight_matrix[1][0], output_weight_matrix[1][1], output_weight_matrix[1][2], output_weight_matrix[1][3], weight_1[0]);
    transform_weight(output_weight_matrix[3][0], output_weight_matrix[3][1], output_weight_matrix[3][2], output_weight_matrix[3][3], weight_1[1]);
    transform_weight(output_weight_matrix[5][0], output_weight_matrix[5][1], output_weight_matrix[5][2], output_weight_matrix[5][3], weight_1[2]);
    transform_weight(output_weight_matrix[7][0], output_weight_matrix[7][1], output_weight_matrix[7][2], output_weight_matrix[7][3], weight_1[3]);
    printf("\n Weight_1 is: \n");
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 8; j++){
            printf("%d", weight_1[i][j]);
        }
        printf("\n");
    }

    // Expand the weight_1 matrix to 32 bits
    char weight_1_32[4][32];
    weight_32(weight_1[0], weight_1_32[0]);
    weight_32(weight_1[1], weight_1_32[1]);
    weight_32(weight_1[2], weight_1_32[2]);
    weight_32(weight_1[3], weight_1_32[3]); 
    printf("\n Weight_1 for 32bits is: \n");
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 32; j++){
            printf("%d", weight_1_32[i][j]);
        }
        printf("\n");
    }

    // Convert to hexadecimal
    unsigned int weight_hexadecimal_input_1[4];
    for (int i = 0; i < 4; i++){
        weight_hexadecimal_input_1[i] = 0;
        for (int j = 0; j < 32; j++){
            weight_hexadecimal_input_1[i] = weight_hexadecimal_input_1[i] + weight_1_32[i][j] * pow(2, 31-j);
        }
    }

    ////////////////////////////// wbuff_1 //////////////////////////////
    unsigned int wbuff_1[34];
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
    char weight_3[4][8];
    transform_weight(output_weight_matrix[1][4], output_weight_matrix[1][5], output_weight_matrix[1][6], output_weight_matrix[1][7], weight_3[0]);
    transform_weight(output_weight_matrix[3][4], output_weight_matrix[3][5], output_weight_matrix[3][6], output_weight_matrix[3][7], weight_3[1]);
    transform_weight(output_weight_matrix[5][4], output_weight_matrix[5][5], output_weight_matrix[5][6], output_weight_matrix[5][7], weight_3[2]);
    transform_weight(output_weight_matrix[7][4], output_weight_matrix[7][5], output_weight_matrix[7][6], output_weight_matrix[7][7], weight_3[3]);
    printf("\n Weight_3 is: \n");
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 8; j++){
            printf("%d", weight_3[i][j]);
        }
        printf("\n");
    }

    // Expand the weight_3 matrix to 32 bits
    char weight_3_32[4][32];
    weight_32(weight_3[0], weight_3_32[0]);
    weight_32(weight_3[1], weight_3_32[1]);
    weight_32(weight_3[2], weight_3_32[2]);
    weight_32(weight_3[3], weight_3_32[3]); 
    printf("\n Weight_1 for 32bits is: \n");
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 32; j++){
            printf("%d", weight_3_32[i][j]);
        }
        printf("\n");
    }

    // Convert to hexadecimal
    unsigned int weight_hexadecimal_input_3[4];
    for (int i = 0; i < 4; i++){
        weight_hexadecimal_input_3[i] = 0;
        for (int j = 0; j < 32; j++){
            weight_hexadecimal_input_3[i] = weight_hexadecimal_input_3[i] + weight_3_32[i][j] * pow(2, 31-j);
        }
    }

    ////////////////////////////// wbuff_3 //////////////////////////////
    unsigned int wbuff_3[34];
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
    static unsigned int input_vector_stream[204];
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

    // Print input_vector_stream
    printf(" \n Input_vector_stream is: \n");
    for (int i = 0; i < 204; i++){
        printf("%x\n", input_vector_stream[i]);
    }

    ////////////////////////////// Golden Output //////////////////////////////
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

    static int32_t in_buffs_8x2[204];
    static int32_t gold_buff_8x2[64];
    for (int i = 0; i < 204; i++){
        in_buffs_8x2[i] = input_vector_stream[i];
    }

    for (int i = 0; i < 64; i++){
        gold_buff_8x2[i] = golden_output[i];
    }

    // Print the random matrix and golden matrix
    printf("Weight Matrix is: \n");
    print_matrix(weight_matrix);
    printf("\n Input Matrix is: \n");
    print_matrix(input_matrix);
    printf("\n Transpose Input Matrix is: \n");
    print_matrix(transpose_input_matrix);
    printf("\n Golden Matrix is: \n");
    print_matrix(golden_matrix);

    printf(" \n Input_vector_stream is: \n");
    for (int i = 0; i < 204; i++){
        printf("%x\n", in_buffs_8x2[i]);
    }
    printf("\n Golden Output is: \n");
    for (int i = 0; i < 64; i++){
        printf("%d\n", gold_buff_8x2[i]);
    }

    /////////////// Added Complete ///////////////



	for (i = 0; i < 1; i++)
		for (j = 0; j < reg2; j++)
			//in[i * in_words_adj + j] = (token_t) j;
                        in[i * in_words_adj + j] = (token_t) in_buffs_8x2[j];

	for (i = 0; i < 1; i++)
		//for (j = 0; j < 2 * reg2; j++)
                for (j = 0; j < reg3; j++)      //gold is initialized using reg3
			//gold[i * out_words_adj + j] = (token_t) j;
                        gold[i * out_words_adj + j] = (token_t) gold_buff_8x2[j];
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
	cfg_006[0].hw_buf = buf;
    
	gold = malloc(out_size);

	init_buffer(buf, gold);

	printf("\n====== %s ======\n\n", cfg_006[0].devname);
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

	esp_run(cfg_006, NACC);

	printf("\n  ** DONE **\n");

	errors = validate_buffer(&buf[out_offset], gold);

	free(gold);
	esp_free(buf);

	if (!errors)
		printf("+ Test PASSED\n");
	else
		printf("+ Test FAILED\n");

	printf("\n====== %s ======\n\n", cfg_006[0].devname);

	return errors;
}
