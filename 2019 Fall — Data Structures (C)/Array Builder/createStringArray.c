/*  Collection of functions written by James Singaram on January 22, 2019
    Functions are designed to dynamically allocate memory for a one dimensional
    array of pointers, allocate a char array to store an input string that is
    pointed to by the array of pointers, retrieve the string from a given char
    array, and free all char arrays and pointers that have been allocated
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function creates an array of char pointers
char **createStringArray(int number){
    char **array;

    array = malloc(sizeof(char*) * number);

    return(array);
}

// Function creates a char array to store a string
void setStringArray(char **array, int index, char *string){
    array[index] = malloc(sizeof(char) * (strlen(string) + 1));
    strcpy(array[index], string);
}

// Function returns a specific char array that is requested
char *getStringArray(char **array, int index){
    return(array[index]);
}

// Function frees all allocated char arrays and pointers
void freeStringArray(int **array, int number){
    int i;

    // Frees char arrays
    for (i=0; i<number; i++) free(array[i]);

    // Frees pointers
    free(array);
}
