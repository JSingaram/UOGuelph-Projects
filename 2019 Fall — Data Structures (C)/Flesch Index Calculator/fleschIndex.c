#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv []){

    /*Check for command line arguments, error is absent*/
    if (argc == 1){
        printf("\nPlease include the file to be opened as a command line argument.\n\n");
        return(0);
    }

    /*Declare variables*/
    int numSyl, oldSyl, numWord, oldWord, inWord, numSent, i, length;
    float flesch;
    char *str;
    FILE *filePtr;

    /*Open file, error if unable to open*/
    filePtr = fopen(argv[1], "r");
    if (filePtr == '\0') {
        printf("\nUnable to open file %s. \n\n", argv[1]);
        return(0);
    }

    /*Get length of file, allocate memory to store string containing file*/
    fseek(filePtr, 0, SEEK_END);
    length = ftell(filePtr);
    str = malloc(length+1);

    /*Save file data as char array*/
    fseek(filePtr, 0, SEEK_SET);
    fread(str, sizeof(char), length, filePtr);
    str[length] = '\0';

    /*Close file*/
    fclose(filePtr);

    /*Set counters to zero*/
    numSyl = 0;
    oldSyl = 0;
    numWord = 0;
    oldWord = 0;
    inWord = 0;
    numSent = 0;

    /*Loop through characters from str[0] to str[length]*/
    for (i=0; i<length; i++){

        /*Testing for syllables*/
        switch(str[i]){
            case 'a': case 'i': case 'o': case 'u': case 'y':
                switch(str[i+1]){
                    case 'a': case 'e': case 'i': case 'o': case 'u': case 'y':
                        numSyl--;
                        break;
                }
                numSyl++;
                break;
            case 'e':
                switch(str[i+1]){
                    case ' ': case '\n': case '\0': case '.': case '!': case '?': case ':': case ';': case 'a': case 'e': case 'i': case 'o': case 'u': case 'y':
                        numSyl--;
                        break;
                }
                numSyl++;
                break;
        }

        /*Testing for words*/
        if (inWord == 0 && ((str[i] >= 'a' && str[i] <= 'z') || (str[i] >= 'A' && str[i] <= 'Z') || (str[i] >= '0' && str[i] <= '9'))) inWord = 1;
        if (inWord == 1 && (str[i] == ' ' || str[i] == '\n' || str[i] == '\0' || str[i] == '.' || str[i] == '!' || str[i] == '?' || str[i] == ':' || str[i] == ';')){
            inWord=0;
            numWord++;
            if (numSyl == oldSyl) numSyl++;
            oldSyl = numSyl;
        }

        /*Testing for sentences*/
        if ((str[i] == '\0' || str[i] == '.' || str[i] == '!' || str[i] == '?' || str[i] == ':' || str[i] == ';') && (numWord > oldWord)){
            numSent++;
            oldWord = numWord;
        }
    }

    /*Free string*/
    free(str);

    /*Calculate Flesch Index*/
    flesch = 206.835 - (84.6 * numSyl / numWord) - (1.015 * numWord / numSent);

    /*Output results*/
    printf("Flesch Index = %.0f\n", flesch);
    printf("Syllable Count = %d\n", numSyl);
    printf("Word Count = %d\n", numWord);
    printf("Sentence Count = %d\n", numSent);

}
