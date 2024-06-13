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

///////////////////// Added Functions /////////////////////
#define SIZE 8

// Function to generate a random matrix of size 16x16
void generate_random_matrix(int matrix[SIZE][SIZE]) {
    // Seed the random number generator with millisecond precision
    //srand (time(NULL) * 1000 + clock());
    // srand((unsigned int)time(NULL));

    // Fill the matrix with random values between 0 and 15
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            matrix[i][j] = rand() % 16;
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

// Function to print the matrix in binary
void print_matrix_b(char matrix[SIZE][SIZE][4]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            printf("At position %d, %d: \n", i, j);
            for (int k = 0; k < 4; k++){
                printf("%d", matrix[i][j][k]);
            }
            printf("\n");
        }
    }
}

// Function to print the transformed matrix in binary
void print_transformed_matrix_b(char matrix[SIZE][SIZE][8]) {
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

// Function to convert decimal to binary
void d2b(int decimal, char binary[]){
    int i = 0;
    // Convert decimal to binary
    for (i = 3; i >= 0; i--){
        binary[i] = decimal % 2;
        decimal = decimal / 2;
    }
}

// Function to convert matrix from decimal to binary
void matrix_d2b(int decimal_matrix[SIZE][SIZE], char binary_matrix[SIZE][SIZE][4]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
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

// Input_0 it contains 8 elements and each element is composed of 4 entries of each column
/*
Let input matrix be a1 a2 a3 a4 
                    b1 b2 b3 b4
                    c1 c2 c3 c4
                    d1 d2 d3 d4

so input_1 will be d1c1b1a1
                   d2c2b2a2
                   d3c3b3a3
                   d4c4b4a4

Exterpolate this for 8x8 matrix
*/


int generate() {
    int weight_matrix[SIZE][SIZE];
    int input_matrix[SIZE][SIZE];
    int golden_matrix[SIZE][SIZE];

    // For Weight matrix
    char output_weight_matrix[SIZE][SIZE][4];
    char transformed_weight_matrix[SIZE][SIZE][8];

    // For Input matrix
    int transpose_input_matrix[SIZE][SIZE];
    char output_input_matrix[SIZE][SIZE][4];
    char transformed_input_matrix[SIZE][SIZE][8];

    // Generate the random matrix
    generate_random_matrix(weight_matrix);
    generate_random_matrix(input_matrix);

    // Do transpose of input matrix
    transpose(input_matrix, transpose_input_matrix);

    // Generate Golden Output matrix
    matrix_mult(weight_matrix, input_matrix, golden_matrix);
    
/*
    // Print the random matrix and golden matrix
    printf("Weight Matrix is: \n");
    print_matrix(weight_matrix);
    printf("\n Input Matrix is: \n");
    print_matrix(input_matrix);
    printf("\n Transpose Input Matrix is: \n");
    print_matrix(transpose_input_matrix);
    printf("\n Golden Matrix is: \n");
    print_matrix(golden_matrix);
*/

    // Convert the weight matrix to binary
    //printf("\n Output Matrix is: \n");
    matrix_d2b(weight_matrix, output_weight_matrix);
    //print_matrix_b(output_weight_matrix);
    
    // Transform the weight matrix
    for (int i = 0; i < SIZE; i++){
        for (int j = 0; j < SIZE; j++){
            weight_transform(output_weight_matrix[i][j], transformed_weight_matrix[i][j]);
        }
    }

    // Print the transformed weight matrix
    //printf("\n Transformed Weight Matrix is: \n");
    //print_transformed_matrix_b(transformed_weight_matrix);


    // Convert the input matrix to binary
    matrix_d2b(transpose_input_matrix, output_input_matrix);
    //print_matrix_b(output_input_matrix);

    // Transform the input matrix
    for (int i = 0; i < SIZE; i++){
        for (int j = 0; j < SIZE; j++){
            input_transform(output_input_matrix[i][j], transformed_input_matrix[i][j]);
        }
    }

    // Print the transformed input matrix
    //printf("\n Transformed Input Matrix is: \n");
    //print_transformed_matrix_b(transformed_input_matrix);

    ////////////////////////////// Weight_0 //////////////////////////////
    char weight_input_0[4][32];
    weight_0(transformed_weight_matrix[0], weight_input_0[0]);
    weight_0(transformed_weight_matrix[2], weight_input_0[1]);
    weight_0(transformed_weight_matrix[4], weight_input_0[2]);
    weight_0(transformed_weight_matrix[6], weight_input_0[3]);

/*
    // Print weight_input_0
    printf(" \n Weight_0 is: \n");
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 32; j++){
            printf("%d", weight_input_0[i][j]);
        }
        printf("\n");
    }
*/

    // Convert the weight_input_0 to hexadecimal note the binary is not in 2's complement
    unsigned int weight_hexadecimal_input_0[4];
    for (int i = 0; i < 4; i++){
        weight_hexadecimal_input_0[i] = 0;
        for (int j = 0; j < 32; j++){
            weight_hexadecimal_input_0[i] += weight_input_0[i][j] * pow(2, 31-j);
        }
    }

    // Print weight_decimal_input_0
/*
    printf(" \n Weight_0 in hexadecimal is: \n");
    for (int i = 0; i < 4; i++){
        printf("%x\n", weight_hexadecimal_input_0[i]);
    }
*/

    ////////////////////////////// WBUFF_0 //////////////////////////////
    // This function will take weight_hexadecimal_input_0 and then make it such that it can be fed into the WBUFF_0 directly
    /*
        For WBUFF_0 each entry in weight_hexadecimal_input_0 will be copied for 7 times and last two entries will be padded with 0
    */
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

    // Print wbuff_0
/*
    printf(" \n WBUFF_0 is: \n");
    for (int i = 0; i < 34; i++){
        printf("%x\n", wbuff_0[i]);
    }
*/

    ////////////////////////////// Weight_2 //////////////////////////////
    char weight_input_2[4][32];
    weight_2(transformed_weight_matrix[0], weight_input_2[0]);
    weight_2(transformed_weight_matrix[2], weight_input_2[1]);
    weight_2(transformed_weight_matrix[4], weight_input_2[2]);
    weight_2(transformed_weight_matrix[6], weight_input_2[3]);

    // Print weight_input_2
/*
    printf(" \n Weight_2 is: \n");
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 32; j++){
            printf("%d", weight_input_2[i][j]);
        }
        printf("\n");
    }
*/

    // Convert the weight_input_2 to decimal note the binary is not in 2's complement
    unsigned int weight_hexadecimal_input_2[4];
    for (int i = 0; i < 4; i++){
        weight_hexadecimal_input_2[i] = 0;
        for (int j = 0; j < 32; j++){
            weight_hexadecimal_input_2[i] += weight_input_2[i][j] * pow(2, 31-j);
        }
    }

    // Print weight_hexadecimal_input_2
/*
    printf(" \n Weight_2 in hexadecimal is: \n");
    for (int i = 0; i < 4; i++){
        printf("%x\n", weight_hexadecimal_input_2[i]);
    }
*/

    ////////////////////////////// WBUFF_2 //////////////////////////////
    // This function will take weight_hexadecimal_input_2 and then make it such that it can be fed into the WBUFF_2 directly
    /*
        For WBUFF_0 each entry in weight_hexadecimal_input_2 will be copied for 7 times and first and last entries will be padded with 0
    */
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

    // Print wbuff_2
/*
    printf(" \n WBUFF_2 is: \n");
    for (int i = 0; i < 34; i++){
        printf("%x\n", wbuff_2[i]);
    }
*/
    ////////////////////////////// Weight_1 //////////////////////////////
    char weight_input_1[4][32];
    weight_0(transformed_weight_matrix[1], weight_input_1[0]);
    weight_0(transformed_weight_matrix[3], weight_input_1[1]);
    weight_0(transformed_weight_matrix[5], weight_input_1[2]);
    weight_0(transformed_weight_matrix[7], weight_input_1[3]);

    // Print weight_input_1
/*
    printf("\n Weight_1 is: \n");
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 32; j++){
            printf("%d", weight_input_1[i][j]);
        }
        printf("\n");
    }
*/

    // Convert the weight_input_1 to hexadecimal note the binary is not in 2's complement
    unsigned int weight_hexadecimal_input_1[4];
    for (int i = 0; i < 4; i++){
        weight_hexadecimal_input_1[i] = 0;
        for (int j = 0; j < 32; j++){
            weight_hexadecimal_input_1[i] += weight_input_1[i][j] * pow(2, 31-j);
        }
    }

    // Print weight_hexadecimal_input_1
/*
    printf(" \n Weight_1 in hexadecimal is: \n");
    for (int i = 0; i < 4; i++){
        printf("%x\n", weight_hexadecimal_input_1[i]);
    }
*/

    ////////////////////////////// WBUFF_1 //////////////////////////////
    // This function will take weight_hexadecimal_input_1 and then make it such that it can be fed into the WBUFF_1 directly
    /*
        For WBUFF_0 each entry in weight_hexadecimal_input_1 will be copied for 7 times and first and last entries will be padded with 0
    */
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

    // Print wbuff_1
/*
    printf(" \n WBUFF_1 is: \n");
    for (int i = 0; i < 34; i++){
        printf("%x\n", wbuff_1[i]);
    }
*/

    ////////////////////////////// Weight_3 //////////////////////////////
    char weight_input_3[4][32];
    weight_2(transformed_weight_matrix[1], weight_input_3[0]);
    weight_2(transformed_weight_matrix[3], weight_input_3[1]);
    weight_2(transformed_weight_matrix[5], weight_input_3[2]);
    weight_2(transformed_weight_matrix[7], weight_input_3[3]);

    // Print weight_input_3
/*
    printf("\n Weight_3 is: \n");
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 32; j++){
            printf("%d", weight_input_3[i][j]);
        }
        printf("\n");
    }
*/

    // Convert the weight_input_1 to hexadecimal note the binary is not in 2's complement
    unsigned int weight_hexadecimal_input_3[4];
    for (int i = 0; i < 4; i++){
        weight_hexadecimal_input_3[i] = 0;
        for (int j = 0; j < 32; j++){
            weight_hexadecimal_input_3[i] += weight_input_3[i][j] * pow(2, 31-j);
        }
    }

    // Print weight_hexadecimal_input_3
/*
    printf(" \n Weight_3 in hexadecimal is: \n");
    for (int i = 0; i < 4; i++){
        printf("%x\n", weight_hexadecimal_input_3[i]);
    }
*/

    ////////////////////////////// WBUFF_3 //////////////////////////////
    // This function will take weight_hexadecimal_input_3 and then make it such that it can be fed into the WBUFF_3 directly
    /*
        For WBUFF_0 each entry in weight_hexadecimal_input_3 will be copied for 7 times and first two entries will be padded with 0
    */
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

    // Print wbuff_1
/*
    printf(" \n WBUFF_3 is: \n");
    for (int i = 0; i < 34; i++){
        printf("%x\n", wbuff_3[i]);
    }
*/

    ////////////////////////////// Input_0 //////////////////////////////
    char input_0[8][32];
    weight_0(transformed_input_matrix[0], input_0[0]);
    weight_0(transformed_input_matrix[1], input_0[1]);
    weight_0(transformed_input_matrix[2], input_0[2]);
    weight_0(transformed_input_matrix[3], input_0[3]);
    weight_0(transformed_input_matrix[4], input_0[4]);
    weight_0(transformed_input_matrix[5], input_0[5]);
    weight_0(transformed_input_matrix[6], input_0[6]);
    weight_0(transformed_input_matrix[7], input_0[7]);

    // Print input_0
/*
    printf("\n Input_0 is: \n");
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 32; j++){
            printf("%d", input_0[i][j]);
        }
        printf("\n");
    }
*/

    // Convert the input_input_0 to hexadecimal note the binary is not in 2's complement
    unsigned int input_hexadecimal_input_0[8];
    for (int i = 0; i < 8; i++){
        input_hexadecimal_input_0[i] = 0;
        for (int j = 0; j < 32; j++){
            input_hexadecimal_input_0[i] += input_0[i][j] * pow(2, 31-j);
        }
    }

    // Print input_hexadecimal_input_0
/*
    printf(" \n Input_0 in hexadecimal is: \n");
    for (int i = 0; i < 8; i++){
        printf("%x\n", input_hexadecimal_input_0[i]);
    }
*/

    ////////////////////////////// IBUFF_0 //////////////////////////////
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

   // Print ibuff_0
/*
    printf(" \n IBUFF_0 is: \n");
    for (int i = 0; i < 34; i++){
        printf("%x\n", ibuff_0[i]);
    }
*/

////////////////////////////// Input_1 //////////////////////////////
    char input_1[8][32];
    weight_2(transformed_input_matrix[0], input_1[0]);
    weight_2(transformed_input_matrix[1], input_1[1]);
    weight_2(transformed_input_matrix[2], input_1[2]);
    weight_2(transformed_input_matrix[3], input_1[3]);
    weight_2(transformed_input_matrix[4], input_1[4]);
    weight_2(transformed_input_matrix[5], input_1[5]);
    weight_2(transformed_input_matrix[6], input_1[6]);
    weight_2(transformed_input_matrix[7], input_1[7]);

    // Print input_1
/*
    printf("\n Input_1 is: \n");
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 32; j++){
            printf("%d", input_1[i][j]);
        }
        printf("\n");
    }
*/

    // Convert the input_input_1 to hexadecimal note the binary is not in 2's complement
    unsigned int input_hexadecimal_input_1[8];
    for (int i = 0; i < 8; i++){
        input_hexadecimal_input_1[i] = 0;
        for (int j = 0; j < 32; j++){
            input_hexadecimal_input_1[i] += input_1[i][j] * pow(2, 31-j);
        }
    }

    // Print input_hexadecimal_input_0
/*
    printf(" \n Input_1 in hexadecimal is: \n");
    for (int i = 0; i < 8; i++){
        printf("%x\n", input_hexadecimal_input_1[i]);
    }
*/

    ////////////////////////////// IBUFF_1 //////////////////////////////
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

   // Print ibuff_0
/*
    printf(" \n IBUFF_1 is: \n");
    for (int i = 0; i < 34; i++){
        printf("%x\n", ibuff_1[i]);
    }
*/

    ////////////////////////////// Input_vector stream for FPGA //////////////////////////////
    /*
        First the Ibuff_0 and Ibuff_1 will be fed and their values are interleaved like 
        ibuff_0[0]
        ibuff_1[0]
        ibuff_0[1]
        ibuff_1[1]
        ...
        ibuff_0[33]
        ibuff_1[33]

        Then from location 69 to 136 the weight values from WBUFF_0 and WBUFF_1 will be interleaved like above
    */

    unsigned int input_vector_stream[204];
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
/*
    printf(" \n Input_vector_stream is: \n");
    for (int i = 0; i < 204; i++){
        printf("%x\n", input_vector_stream[i]);
    }
*/

    ////////////////////////////// Golden Output //////////////////////////////
    /*
        This is interleaved like 
        golden_output[0] = golden_matrix[0][0]
        golden_output[1] = golden_matrix[1][0]
        golden_output[2] = golden_matrix[0][1]
        golden_output[3] = golden_matrix[1][1]
        golden_output[4] = golden_matrix[0][2]
        golden_output[5] = golden_matrix[1][2]
        ...
        golden_output[63] = golden_matrix[7][7]
    */
    int32_t golden_output[64];

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
    
    // Print golden_output
/*
    printf("\n Golden Matrix is: \n");
    print_matrix(golden_matrix);

    printf(" \n Golden Output is: \n");
    for (int i = 0; i < 64; i++){
        printf("%d\n", golden_output[i]);
    }
*/



    return 0;
}



static unsigned DMA_WORD_PER_BEAT(unsigned _st)
{
        return (sizeof(void *) / _st);
}


#define SLD_EXAMPLE 0x075
#define DEV_NAME "sld,example_rtl"


/* <<--params-->> */
const int32_t reg1 = 8; //Value of config reg
const int32_t reg3 = 64; //BS: contains the number of 32-bit output words from the accelerator
//const int32_t reg2 = 2; //2 64 bit values (based on NoC), so we get 4 32 bit values (Number of transactions on the NOC)
const int32_t reg2 = 204; //BS: this will contain the number of 32-bit words sent to the accelerator
//102 dma transaction

/*
int32_t in_buffs[reg2] = { 

0x88111133,
0x11116699,
0x44ffbb,
0xffff1188,
0xaaffcc99,
0xddaa77ee,
0x7700bbaa,
0x668888ff,
0x88111133,
0x11116699,
0x44ffbb,
0xffff1188,
0xaaffcc99,
0xddaa77ee,
0x7700bbaa,
0x668888ff,
0x88111133,
0x11116699,
0x44ffbb,
0xffff1188,
0xaaffcc99,
0xddaa77ee,
0x7700bbaa,
0x668888ff,
0x88111133,
0x11116699,
0x44ffbb,
0xffff1188,
0xaaffcc99,
0xddaa77ee,
0x7700bbaa,
0x668888ff,
0x00000000,
0x00000000,

0x00000000,
0x66aacc33,
0xaa6699aa,
0x77eeee33,
0x55336611,
0xee111111,
0xccff9977,
0xffee4499,
0xaa55aacc,
0x66aacc33,
0xaa6699aa,
0x77eeee33,
0x55336611,
0xee111111,
0xccff9977,
0xffee4499,
0xaa55aacc,
0x66aacc33,
0xaa6699aa,
0x77eeee33,
0x55336611,
0xee111111,
0xccff9977,
0xffee4499,
0xaa55aacc,
0x66aacc33,
0xaa6699aa,
0x77eeee33,
0x55336611,
0xee111111,
0xccff9977,
0xffee4499,
0xaa55aacc,
0x00000000,

0x00000000,
0x00000000,
0xa0a0ff,
0xa0a0ff,
0xa0a0ff,
0xa0a0ff,
0xa0a0ff,
0xa0a0ff,
0xa0a0ff,
0xa0a0ff,
0x50affaa0,
0x50affaa0,
0x50affaa0,
0x50affaa0,
0x50affaa0,
0x50affaa0,
0x50affaa0,
0x50affaa0,
0xaaa00005,
0xaaa00005,
0xaaa00005,
0xaaa00005,
0xaaa00005,
0xaaa00005,
0xaaa00005,
0xaaa00005,
0xa55affaf,
0xa55affaf,
0xa55affaf,
0xa55affaf,
0xa55affaf,
0xa55affaf,
0xa55affaf,
0xa55affaf,
};
*/


static unsigned in_words_adj;
static unsigned out_words_adj;
static unsigned in_len;
static unsigned out_len;
static unsigned in_size;
static unsigned out_size;
static unsigned out_offset;
static unsigned mem_size;

//int32_t in_buffs[34];

/* Size of the contiguous chunks for scatter/gather */
#define CHUNK_SHIFT 20
#define CHUNK_SIZE BIT(CHUNK_SHIFT)
#define NCHUNK(_sz) ((_sz % CHUNK_SIZE == 0) ?		\
			(_sz / CHUNK_SIZE) :		\
			(_sz / CHUNK_SIZE) + 1)

/* User defined registers */
/* <<--regs-->> */
#define EXAMPLE_REG1_REG 0x48
#define EXAMPLE_REG3_REG 0x44
#define EXAMPLE_REG2_REG 0x40


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
/*
        int32_t in_buffs[204] = { 
88111133, //IBUF0
00000000, //IBUF1

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
*/

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


/*
int32_t gold_buff[64] = { 

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
*/

int32_t gold_buff[64] = {
307,
//300,
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

	if (DMA_WORD_PER_BEAT(sizeof(token_t)) == 0) {
		in_words_adj = reg2;
		//out_words_adj = reg2;
		out_words_adj = reg3;
	} else {
		in_words_adj = round_up(reg2, DMA_WORD_PER_BEAT(sizeof(token_t)));
		//out_words_adj = round_up(reg2, DMA_WORD_PER_BEAT(sizeof(token_t)));
		out_words_adj = round_up(reg3, DMA_WORD_PER_BEAT(sizeof(token_t)));
	}
	in_len = in_words_adj * (1);
	out_len = out_words_adj * (1);
	in_size = in_len * sizeof(token_t);
	out_size = out_len * sizeof(token_t);
	out_offset  = in_len;
	mem_size = (out_offset * sizeof(token_t)) + out_size;
/*
        printf("in_len = %d \n",in_len); 
	printf("out_len = %d \n",out_len);
	printf("in_size = %d \n",in_size);
	printf("out_size = %d \n",out_size);
	printf("out_offset = %d \n",out_offset);
	printf("mem_size = %d \n",mem_size);

*/	// Search for the device
	printf("Scanning device tree... \n");
	espdevs_1.addr = 0x60010000;
/*
	ndev = probe(&espdevs, VENDOR_SLD, SLD_EXAMPLE, DEV_NAME);
	if (ndev == 0) {
		printf("example not found\n");
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
			//printf("  -> scatter-gather DMA is disabled. Abort.\n");
			return 0;
		}

		if (ioread32(dev, PT_NCHUNK_MAX_REG) < NCHUNK(mem_size)) {
			//printf("  -> Not enough TLB entries available. Abort.\n");
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
		iowrite32(dev, EXAMPLE_REG1_REG, reg1);
		iowrite32(dev, EXAMPLE_REG3_REG, reg3/words_per_dma_packet); //BS: The number of 64-bit DMA packets for DMA write
		//iowrite32(dev, EXAMPLE_REG2_REG, reg2);
		iowrite32(dev, EXAMPLE_REG2_REG, reg2/words_per_dma_packet); //BS: The number of 64-bit DMA packets for DMA read


			// Flush (customize coherence model here)
			//esp_flush(coherence);

			// Start accelerators
			printf("  Start...\n");
			iowrite32(dev, CMD_REG, CMD_MASK_START);

			// Wait for completion
			done = 0;
			while (!done) {
				printf("Done = %d",done);
				done = ioread32(dev, STATUS_REG);
				done &= STATUS_MASK_DONE;
			}
			iowrite32(dev, CMD_REG, 0x0);

			printf("  Done\n");
			printf("  validating...\n");

			/* Validation */
			errors = validate_buf(&mem[out_offset], gold);
			if (errors)
				printf("  ... FAIL %d \n", errors);
			else
				printf("  ... PASS\n");
		}
		aligned_free(ptable);
		aligned_free(mem);
		aligned_free(gold);
	}

	return 0;
}
