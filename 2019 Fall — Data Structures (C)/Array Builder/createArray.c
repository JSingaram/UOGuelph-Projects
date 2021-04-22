/*  Collection of functions written by James Singaram on January 21, 2019
    Functions are designed to dynamically allocate memory for a two dimensional
    int array and an array of pointers referencing each row, and free the int
    array and pointer array
*/

#include <stdio.h>
#include <stdlib.h>

// Function creates an array of pointers and a 2D int array
int **createArray(int rows, int cols){
    int i;
    int **array;

    // Allocates array of pointers
    array = malloc (sizeof(int*) * rows);

    // Allocates arrays of integers for each pointer
    for (i=0; i<rows; i++) array[i] = malloc(sizeof(int) * cols);

    return (array);
}

// Function frees array of pointers and 2D int array
void freeArray(int **array, int rows, int cols){
    int i;

    // Frees arrays of integers
    for (i=0; i<=rows; i++) free(array[i]);

    // Frees array of pointers
    free(array);
}
