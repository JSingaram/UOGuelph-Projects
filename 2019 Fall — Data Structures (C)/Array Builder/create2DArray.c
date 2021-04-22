/*  Collection of functions written by James Singaram on January 21, 2019
    Functions are designed to dynamically allocate memory for a two dimensional
    array based on input dimensions, save and retrieve values from specific
    positions, and free the array
*/

#include <stdio.h>
#include <stdlib.h>

// Function allocates memory for an array, saves dimensions in first row
int *create2DArray(int rows, int cols){
    int *array;

    array = malloc(sizeof(int) * cols * (rows + 1));

    // Save number of rows in [0][0]
    array[0] = rows;
    // Save number of columns in [0][1]
    array[1] = cols;

    return (array);
}

// Function changes a value in the array, using number of columns saved in [0][1]
void set2DElement (int *array, int row, int col, int value){
    int cols = array[1];

    array[((cols * (row + 1)) + col)] = value;
}

// Function retrieves value saved in array, using number of columns saved in [0][1]
int get2DElement (int *array, int row, int col){
    int cols = array[1];

    return array[((cols * (row + 1)) + col)];
}

// Function frees array
void free2DArray(int *array){
    free(array);
}
