/**
 * arrayDefns.h - Functions required for Assignment 1
 * Do not change the provided function prototypes.
 */

/* Part 1 */
int *create2DArray ( int rows, int cols );
void set2DElement( int *array, int row, int col, int value );
int get2DElement ( int *array, int row, int col );
void free2DArray ( int *array );

/* Part 2 */
char **createStringArray ( int number );
void setStringArray ( char **array, int index, char * string );
char *getStringArray ( char **array, int index );
void freeStringArray ( char **array, int number );

/* Part 3 */
int **createArray ( int rows, int cols );
void freeArray ( int **array, int rows, int cols);
