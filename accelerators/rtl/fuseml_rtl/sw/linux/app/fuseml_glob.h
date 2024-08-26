/////// Added Functions and generate code ///////////
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
/* 

conf = 0 -> unsigned int 2x2
conf = 1 -> unsigned int 4x4
conf = 2 -> unsigned int 8x2
conf = 3 -> unsigned int 8x8

conf = 4 -> signed int 2x2
conf = 5 -> signed int 4x4
conf = 6 -> signed int 8x2
conf = 7 -> signed int 8x8

*/

// Function to generate a random input matrix
void generate_random_input_matrix(int conf, int matrix_size, int matrix[matrix_size][matrix_size]) {
    // Seed the random number generator with millisecond precision
    //srand (time(NULL) * 1000 + clock());
    // srand((unsigned int)time(NULL));
    // give a test seed for random number generation
    srand(1);

    // Fill the matrix with random values between -2 and 1
    if (conf == 0) {
    	for (int i = 0; i < matrix_size; i++) {
    	    for (int j = 0; j < matrix_size; j++) {
    	        matrix[i][j] = rand() % 4;
    	    }
    	}
    }
    else if (conf == 1) {
    	for (int i = 0; i < matrix_size; i++) {
    	    for (int j = 0; j < matrix_size; j++) {
    	        matrix[i][j] = rand() % 16;
    	    }
    	}
    }
    else if (conf == 2 || conf == 3) {
    	for (int i = 0; i < matrix_size; i++) {
    	    for (int j = 0; j < matrix_size; j++) {
    	        matrix[i][j] = rand() % 256;
    	    }
    	}
    }
    else if (conf == 4) {
    	for (int i = 0; i < matrix_size; i++) {
    	    for (int j = 0; j < matrix_size; j++) {
    	        matrix[i][j] = rand() % 4 - 2;
    	    }
    	}
    }
    else if (conf == 5) {
    	for (int i = 0; i < matrix_size; i++) {
    	    for (int j = 0; j < matrix_size; j++) {
    	        matrix[i][j] = rand() % 16 - 8;
    	    }
    	}
    }
    else if (conf == 6 || conf == 7) {
    	for (int i = 0; i < matrix_size; i++) {
    	    for (int j = 0; j < matrix_size; j++) {
    	        matrix[i][j] = rand() % 256 - 128;
    	    }
    	}
    }
}

// Function to generate a random weight matrix
void generate_random_weight_matrix(int conf, int matrix_size, int matrix[matrix_size][matrix_size]) {
    // Seed the random number generator with millisecond precision
    //srand (time(NULL) * 1000 + clock());
    // srand((unsigned int)time(NULL));
    // give a test seed for random number generation
    srand(1);

    // Fill the matrix with random values between -2 and 1
    if (conf == 0 || conf == 2) {
    	for (int i = 0; i < matrix_size; i++) {
    	    for (int j = 0; j < matrix_size; j++) {
    	        matrix[i][j] = rand() % 4;
    	    }
    	}
    }
    else if (conf == 1) {
    	for (int i = 0; i < matrix_size; i++) {
    	    for (int j = 0; j < matrix_size; j++) {
    	        matrix[i][j] = rand() % 16;
    	    }
    	}
    }
    else if (conf == 3) {
    	for (int i = 0; i < matrix_size; i++) {
    	    for (int j = 0; j < matrix_size; j++) {
    	        matrix[i][j] = rand() % 256;
    	    }
    	}
    }
    else if (conf == 4 || conf == 6) {
    	for (int i = 0; i < matrix_size; i++) {
    	    for (int j = 0; j < matrix_size; j++) {
    	        matrix[i][j] = rand() % 4 - 2;
    	    }
    	}
    }
    else if (conf == 5) {
    	for (int i = 0; i < matrix_size; i++) {
    	    for (int j = 0; j < matrix_size; j++) {
    	        matrix[i][j] = rand() % 16 - 8;
    	    }
    	}
    }
    else if (conf == 7) {
    	for (int i = 0; i < matrix_size; i++) {
    	    for (int j = 0; j < matrix_size; j++) {
    	        matrix[i][j] = rand() % 256 - 128;
    	    }
    	}
    }
}

// Function to generate trasnpose of a matrix - Used for input matrix
void transpose(int matrix_size, int matrix[matrix_size][matrix_size], int transpose_matrix[matrix_size][matrix_size]) {
    for (int i = 0; i < matrix_size; i++) {
        for (int j = 0; j < matrix_size; j++) {
            transpose_matrix[j][i] = matrix[i][j];
        }
    }
}

// Function to multiply two matrices
void matrix_mult(int matrix_size, int matrix1[matrix_size][matrix_size], int matrix2[matrix_size][matrix_size], int result[matrix_size][matrix_size]) {
    for (int i = 0; i < matrix_size; i++) {
        for (int j = 0; j < matrix_size; j++) {
            result[i][j] = 0;
            for (int k = 0; k < matrix_size; k++) {
                result[i][j] += matrix1[i][k] * matrix2[k][j];
            }
        }
    }
}

// Function to print the matrix
void print_matrix(int matrix_size, int matrix[matrix_size][matrix_size]) {
    for (int i = 0; i < matrix_size; i++) {
        for (int j = 0; j < matrix_size; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

// Function to print the input matrix in binary
void print_input_matrix_b(int matrix_size, int char_input_size, char matrix[matrix_size][matrix_size][char_input_size]) {
    for (int i = 0; i < matrix_size; i++) {
        for (int j = 0; j < matrix_size; j++) {
            //printf("At position %d, %d: \n", i, j);
            for (int k = 0; k < char_input_size; k++){
                printf("%d ", matrix[i][j][k]);
            }
        }
        printf("\n");
    }
}

// Function to print the weight matrix in binary
void print_weight_matrix_b(int matrix_size, int char_weight_size, char matrix[matrix_size][matrix_size][char_weight_size]) {
    for (int i = 0; i < matrix_size; i++) {
        for (int j = 0; j < matrix_size; j++) {
            //printf("At position %d, %d: \n", i, j);
            for (int k = 0; k < char_weight_size; k++){
                printf("%d ", matrix[i][j][k]);
            }
        }
        printf("\n");
    }
}

// Function to convert input decimal to binary unsigned
void input_d2b_unsigned(int conf, int decimal, char binary[]){
    int i = 0;
    // Convert decimal to binary
    if (conf == 0) {
    	for (i = 1; i >= 0; i--){
        	binary[i] = decimal % 2;
        	decimal = decimal / 2;
    	}
    }
    else if (conf == 1) {
    	for (i = 3; i >= 0; i--){
        	binary[i] = decimal % 2;
        	decimal = decimal / 2;
    	}
    }
    else if (conf == 2 || conf == 3) {
    	for (i = 7; i >= 0; i--){
        	binary[i] = decimal % 2;
        	decimal = decimal / 2;
    	}
    }
    
}

// Function to convert input decimal to binary signed
void input_d2b_signed(int conf, int decimal, char binary[]){
    int i = 0;
    // Convert decimal to binary
    if (conf == 4) {
    	for (i = 1; i >= 0; i--){
        	binary[i] = (decimal & 1) ? 1 : 0;
        	decimal >>= 1;
    	}
    }
    else if (conf == 5) {
        for (i = 3; i >= 0; i--){
       	 	binary[i] = (decimal & 1) ? 1 : 0;
        	decimal >>= 1;
    	}
    }
    else if (conf == 6 || conf == 7) {
        for (i = 7; i >= 0; i--){
       	 	binary[i] = (decimal & 1) ? 1 : 0;
        	decimal >>= 1;
    	}
    }
}

// Function to convert weight decimal to binary unsigned
void weight_d2b_unsigned(int conf, int decimal, char binary[]){
    int i = 0;
    // Convert decimal to binary
    if (conf == 0 || conf == 2) {
    	for (i = 1; i >= 0; i--){
        	binary[i] = decimal % 2;
        	decimal = decimal / 2;
    	}
    }
    else if (conf == 1) {
    	for (i = 3; i >= 0; i--){
        	binary[i] = decimal % 2;
        	decimal = decimal / 2;
    	}
    }
    else if (conf == 3) {
    	for (i = 7; i >= 0; i--){
        	binary[i] = decimal % 2;
        	decimal = decimal / 2;
    	}
    }
    
}

// Function to convert weight decimal to binary signed
void weight_d2b_signed(int conf, int decimal, char binary[]){
    int i = 0;
    // Convert decimal to binary
    if (conf == 4 || conf == 6) {
    	for (i = 1; i >= 0; i--){
        	binary[i] = (decimal & 1) ? 1 : 0;
        	decimal >>= 1;
    	}
    }
    else if (conf == 5) {
        for (i = 3; i >= 0; i--){
       	 	binary[i] = (decimal & 1) ? 1 : 0;
        	decimal >>= 1;
    	}
    }
    else if (conf == 7) {
        for (i = 7; i >= 0; i--){
       	 	binary[i] = (decimal & 1) ? 1 : 0;
        	decimal >>= 1;
    	}
    }
}

// Function to convert input matrix from decimal to binary
void input_matrix_d2b(int matrix_size, int conf, int char_input_size, int decimal_matrix[matrix_size][matrix_size], char binary_matrix[matrix_size][matrix_size][char_input_size]) {
    for (int i = 0; i < matrix_size; i++) {
        for (int j = 0; j < matrix_size; j++) {
		if (conf == 0 || conf == 1 || conf == 2 || conf == 3) {	
        	    input_d2b_unsigned(conf, decimal_matrix[i][j], binary_matrix[i][j]);
		}
		else if (conf == 4 || conf == 5 || conf == 6 || conf == 7) {
        	    input_d2b_signed(conf, decimal_matrix[i][j], binary_matrix[i][j]);		
		}
        }
    }
}

// Function to convert weight matrix from decimal to binary
void weight_matrix_d2b(int matrix_size, int conf, int char_weight_size, int decimal_matrix[matrix_size][matrix_size], char binary_matrix[matrix_size][matrix_size][char_weight_size]) {
    for (int i = 0; i < matrix_size; i++) {
        for (int j = 0; j < matrix_size; j++) {
		if (conf == 0 || conf == 1 || conf == 2 || conf == 3) {	
        	    weight_d2b_unsigned(conf, decimal_matrix[i][j], binary_matrix[i][j]);
		}
		else if (conf == 4 || conf == 5 || conf == 6 || conf == 7) {
        	    weight_d2b_signed(conf, decimal_matrix[i][j], binary_matrix[i][j]);		
		}
        }
    }
}

///////////////////////////////// for int 2x2 /////////////////////////////////

// Weight_0 it contains first 16 elements of 1st, 3rd, 5th and so on row of weight matrix
// So weight_0 will have 16 entries each entry is 32 bits wide
// Weight_0 entry 1 = concatenation of binary_matrix[0][15],binary_matrix[0][14],binary_matrix[0][13]...binary_matrix[0][0]
/* 
So the below function will take output_weight_matrix and then strore 1st 16 elements of 1st row into weight_0[0]
and 1st 16 elements of 3rd row into weight_0[1] and so on
*/
void weight_0_2x2(char input[16][2], char output[32]){
    for (int i = 0; i < 16; i++){
//        for (int j = 0; j < 2; j++){
		output[(30 - 2*i) + 0] = input[i][0];
		output[(30 - 2*i) + 1] = input[i][1];
        //}
    }
}

// Weight_1 it contains second 16 elements of 1st, 3rd, 5th and so on row of weight matrix
// So weight_1 will have 16 entries each entry is 32 bits wide
// Weight_1 entry 1 = concatenation of binary_matrix[0][15],binary_matrix[0][14],binary_matrix[0][13]...binary_matrix[0][0]
/* 
So the below function will take output_weight_matrix and then strore 1st 16 elements of 1st row into weight_1[0]
and 1st 16 elements of 3rd row into weight_0[1] and so on
*/

void weight_2_2x2(char input[16][2], char output[32]){
    for (int i = 16; i < 32; i++){
        //for (int j = 0; j < 2; j++){
		output[(30 - 2*(i - 16)) + 0] = input[i][0];
		output[(30 - 2*(i - 16)) + 1] = input[i][1];
        //}
    }
}

///////////////////////////////// complete for int 2x2 /////////////////////////////////


///////////////////////////////// for int 4x4 /////////////////////////////////

// Do the transformation from MSB4 MSB3 MSB2 MSB1 ->  MSB4 MSB3 MSB4 MSB3 MSB2 MSB1 MSB2 MSB1 on Weight Matrix entries
// Function for weight transformation
void weight_transform_4x4(char input[4], char output[8]){
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
void input_transform_4x4(char input[4], char output[8]){
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
void weight_0_4x4(char input[4][8], char output[32]){
    // output[24 to 31] = input[0][8]
    // output[16 to 23] = input[1][8]
    // output[8 to 15] = input[2][8]
    // output[0 to 7] = input[3][8]
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 8; j++){
	    output[24 - (i * 8) + j] = input[i][j];
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
void weight_2_4x4(char input[4][8], char output[32]){
    // output[24 to 31] = input[0][8]
    // output[16 to 23] = input[1][8]
    // output[8 to 15] = input[2][8]
    // output[0 to 7] = input[3][8]
    for (int i = 4; i < 8; i++){
        for (int j = 0; j < 8; j++){
            output[24 - ((i - 4) * 8) + j] = input[i][j];
        }
    }
}

///////////////////////////////// complete for int 4x4 /////////////////////////////////



///////////////////////////////// for int 8x2 /////////////////////////////////

void transform_input_8x2(char number1[8], char number2[8], char number3[8], char number4[8], char output[32]){
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
void input_orchestration_8x2(char input[32], char output[32]){
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
void transform_weight_8x2(char number1[2], char number2[2], char number3[2], char number4[2], char output[8]){
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
void weight_32_8x2(char input[8], char output[32]){
    output[0] = output[4] = output[16] = output[20] = input[6];
    output[1] = output[5] = output[17] = output[21] = input[7];
    output[2] = output[6] = output[18] = output[22] = input[4];
    output[3] = output[7] = output[19] = output[23] = input[5];
    output[8] = output[12] = output[24] = output[28] = input[2];
    output[9] = output[13] = output[25] = output[29] = input[3];
    output[10] = output[14] = output[26] = output[30] = input[0];
    output[11] = output[15] = output[27] = output[31] = input[1];
}

///////////////////////////////// completed for int 8x2 /////////////////////////////////



///////////////////////////////// for int 8x8 /////////////////////////////////
/*
Do the transformation from a1a2a3a4a5a6a7a8 to a7a8a5a6a7a8a5a6_a3 a4 a1 a2 a3 a4 a1 a2_a7 a8 a5 a6 a7 a8 a5 a6_a3 a4 a1 a2 a3 a4 a1 a2
               Location -> 0 1 2 3 4 5 6 7     0 1 2 3 4 5 6 7  8  9  10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31
On all entries of the weight matrix
*/
void weight_transform_8x8(char input[8], char output[32]){
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
void input_transform_8x8(char input[8], char output[32]){
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

///////////////////////////////// complete for int 8x8 /////////////////////////////////

