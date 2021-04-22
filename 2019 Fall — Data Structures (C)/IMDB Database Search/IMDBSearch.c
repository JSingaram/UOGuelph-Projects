#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct contact{
    unsigned long phone_number;
    long first_name_posn;
    long last_name_posn;
    long company_name_posn;
    long email_posn;
    long next;
};
typedef struct contact contact;

struct strings{
    char *first_name;
    char *last_name;
    char *company_name;
    char *email;
    unsigned long phone_number;
};
typedef struct strings strings;

/* Opens file and saves database to array */
int openFile (strings **arr, int arrSize){
    FILE *filePtr = fopen("myContactList.db", "r+b");
    /* If file does not exist, return 0 for size of array*/
    if (filePtr == NULL) return(0);

    /* Get location of EOF, return to start of file */
    fseek(filePtr, 0, SEEK_END);
    int endLoc = ftell(filePtr);
    fseek(filePtr, 0, SEEK_SET);

    contact posn;
    long next_posn;

    while (ftell(filePtr) < endLoc){
        *arr = realloc(*arr, sizeof(strings) * (arrSize + 1));

        (*arr)[arrSize].first_name = malloc(1);
        (*arr)[arrSize].last_name = malloc(1);
        (*arr)[arrSize].company_name = malloc(1);
        (*arr)[arrSize].email = malloc(1);

        /* Set posn to zero */
        posn.phone_number = 0;
        posn.first_name_posn = 0;
        posn.last_name_posn = 0;
        posn.company_name_posn = 0;
        posn.email_posn = 0;
        posn.next = 0;

        /* Read posn from file */
        fread(&(posn), sizeof(contact), 1, filePtr);

        /* Read first name from file */
        if (posn.last_name_posn > 0) next_posn = posn.last_name_posn;
        else if (posn.company_name_posn > 0) next_posn = posn.company_name_posn;
        else if (posn.email_posn > 0) next_posn = posn.email_posn;
        else next_posn = posn.next;
        if (posn.first_name_posn != 0){
            fseek(filePtr, posn.first_name_posn, SEEK_SET);
            (*arr)[arrSize].first_name = realloc((*arr)[arrSize].first_name, (next_posn - posn.first_name_posn));
            fread((*arr)[arrSize].first_name, (next_posn - posn.first_name_posn), 1, filePtr);
        }
        else (*arr)[arrSize].first_name[0] = '\0';

        /* Read Last name from file */
        if (posn.company_name_posn > 0) next_posn = posn.company_name_posn;
        else if (posn.email_posn > 0) next_posn = posn.email_posn;
        else next_posn = posn.next;
        if (posn.last_name_posn != 0){
        fseek(filePtr, posn.last_name_posn, SEEK_SET);
        (*arr)[arrSize].last_name = realloc((*arr)[arrSize].last_name, (next_posn - posn.last_name_posn));
        fread((*arr)[arrSize].last_name, (next_posn - posn.last_name_posn), 1, filePtr);
        }
        else (*arr)[arrSize].last_name[0] = '\0';

        /* Read company name from file */
        if (posn.email_posn > 0) next_posn = posn.email_posn;
        else next_posn = posn.next;
        if (posn.company_name_posn != 0){
        fseek(filePtr, posn.company_name_posn, SEEK_SET);
        (*arr)[arrSize].company_name = realloc((*arr)[arrSize].company_name, (next_posn - posn.company_name_posn));
        fread((*arr)[arrSize].company_name, (next_posn - posn.company_name_posn), 1, filePtr);
        }
        else (*arr)[arrSize].company_name[0] = '\0';

        /* Read email from file */
        next_posn = posn.next;
        if (posn.email_posn != 0){
        fseek(filePtr, posn.email_posn, SEEK_SET);
        (*arr)[arrSize].email = realloc((*arr)[arrSize].email, (next_posn - posn.email_posn));
        fread((*arr)[arrSize].email, (next_posn - posn.email_posn), 1, filePtr);
        }
        else (*arr)[arrSize].email[0] = '\0';

        (*arr)[arrSize].phone_number = posn.phone_number;
        arrSize++;
    }

    /* Close file */
    fclose(filePtr);

    return(arrSize);
}


/* Sorts array by last and company names */
void sortContacts2(strings* arr, int arrSize){
    int i, j;
    strings tempContact;

    char* str1;
    char* str2;

    for (i=0; i<arrSize; i++){
        for(j=0; j<(arrSize-i-1); j++){
            if (arr[j].last_name[0] == '\0') str1 = arr[j].company_name;
            else str1 = arr[j].last_name;
            if (arr[j+1].last_name[0] == '\0') str2 = arr[j+1].company_name;
            else str2 = arr[j+1].last_name;

            if (strcmp(str1, str2) > 0){
                tempContact = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = tempContact;
            }
        }
    }

    return;
}


/* Display full contact list */
 void displayContacts (strings *arr, int arrSize){
     int i;
     char firstLet, prevFirstLet= '\0';

     printf("\nNumber of Contacts = %d\n", arrSize);

     for (i=0; i<arrSize; i++){
        if (arr[i].last_name[0] == '\0') firstLet = arr[i].company_name[0];
        else firstLet = arr[i].last_name[0];

        if (firstLet != prevFirstLet) printf("%c\n", firstLet);

        if (arr[i].last_name[0] == '\0') printf("%3d.  %s\n", i+1, arr[i].company_name);
        else printf("%3d.  %s %s\n", i+1, arr[i].first_name, arr[i].last_name);

        prevFirstLet = firstLet;
     }
     return;
}


/* Get action from user and check for valid input for Full Contact List */
void inputFCL(char action[5]){
    int i;

    while(1){
        printf("Action: ");
        fgets(action, 5, stdin);

        /* Clear stdin if no newline in string */
        if (action[0] != '\n' && action[1] != '\n' && action[2] != '\n' && action[3] != '\n' && action[4] != '\n') while (getchar()!= '\n');

        /* Test if input is valid letter */
        if (action[1] == '\n' && (action[0] == 'A' || action [0] == 'X')) return;

        /* Test is input is number */
        i = 0;
        while (action[i] != '\n' && i < 5){
            if (action[0] == '0') break;
            if (action[i] >= '0' && action[i] <= '9') i++;
            else break;
        }
        if (i>0 && action[i] == '\n') {
            action[i] = '\0';
            return;
        }

        printf("\nPlease enter a valid action:\n");
        printf("   # - View more deatails on contact # (numerals only)\n");
        printf("   A - Add a new contact\n");
        printf("   X - Exit program\n\n");
    }
}


/* Display information for a selected contact */
void displayContactInfo (strings *arr, int contactNum){
    printf("\nContact #%d\n", contactNum);
    printf("First Name: %s\n", arr[contactNum-1].first_name);
    printf("Last Name: %s\n", arr[contactNum-1].last_name);
    printf("Company Name: %s\n", arr[contactNum-1].company_name);
    printf("Phone Number: %lu\n", arr[contactNum-1].phone_number);
    printf("Email: %s\n", arr[contactNum-1].email);
    return;
}


/* Get action from user and check for valid input for Display Contact Info */
void inputDCI(char action[5]){
    action[0] = '\0';
    action[1] = '\0';
    while(1){
        printf("Action: ");
        fgets(action, 5, stdin);
        /* Clear stdin if no newline in string */
        if (action[0] != '\n' && action[1] != '\n' && action[2] != '\n' && action[3] != '\n' && action[4] != '\n') while (getchar()!= '\n');
        if (action[1] == '\n' && (action[0] == 'R' || action[0] == 'E' || action[0] == 'D')) return;

        printf("\nPlease enter a valid action:\n");
        printf("   R - Return to full contacts list\n");
        printf("   E - Edit this contact\n");
        printf("   D - Delete this contact\n\n");
    }
}


/* Get action from user and check for valid input for Edit Contact */
char inputEC(){
    /* Get input */
    /* Test input for R/S/D */
    /* Return R/S/D */

    char action[5] = {'\0', '\0', '\0', '\0', '\0'};
    while(1){
        printf("Action: ");
        fgets(action, 5, stdin);
        /* Clear stdin if no newline in string */
        if (action[0] != '\n' && action[1] != '\n' && action[2] != '\n' && action[3] != '\n' && action[4] != '\n') while (getchar()!= '\n');
        if (action[1] == '\n' && (action[0] == 'R' || action[0] == 'S' || action[0] == 'D')) return(action[0]);

        printf("\nPlease enter a valid action:\n");
        printf("   R - Return to full contacts list\n");
        printf("   S - Save this contact\n");
        printf("   D - Delete this contact\n\n");
    }
}


/* Edit phone number and email */
char editContact(strings* arr, int contactNum){
    char* tempPhoneNum;
    char* tempEmail;
    int i, j, error;
    char c;

    tempPhoneNum = malloc(1);
    tempEmail = malloc(1);

    printf("\nContact #%d\n", contactNum);
    printf("First Name: %s\n", arr[contactNum-1].first_name);
    printf("Last Name: %s\n", arr[contactNum-1].last_name);
    printf("Company Name: %s\n", arr[contactNum-1].company_name);

    /* Get phone number and test for 7 or 10 digits, and all numerals */
    while (1){
        error = 1;
        i=0;
        tempPhoneNum = realloc(tempPhoneNum, i+1);
        printf("Phone Number: ");

        /* Get phone number */
        while(1){
            tempPhoneNum[i] = getchar();
            i++;
            tempPhoneNum = realloc(tempPhoneNum, i+1);
            if (tempPhoneNum[i-1] == '\n') break;
        }
        tempPhoneNum[i-1] = '\0';

        /* Test phone number */
        if (i == 8 || i == 11){
            for (j=0; j<i-1; j++){
                if (tempPhoneNum[j] >= '0' && tempPhoneNum[j] <= '9') error = 0;
                else {error = 1; break;}
            }
        }
        if (error == 0) break;
        printf("\nPlease enter a seven or ten digit phone number (numerals only).\n\n");
    }

    /* Get email and test for abc@mno.xyz format */
    while (1) {
        error = 0;
        i=0;
        tempEmail = realloc(tempEmail, i+1);
        printf("Email: ");

        /* Get email */
        while(1){
            tempEmail[i] = getchar();
            i++;
            tempEmail = realloc(tempEmail, i+1);
            if (tempEmail[i-1] == '\n') break;
        }
        tempEmail[i-1] = '\0';

        /* Test email format */
        if (tempEmail[0] == '\0') error = 2;
        for(j=1; j<i; j++){
            if (error == 0 && tempEmail[j] == '@') {
                error = 1;
                j++;
            }
            else if (error == 1 && tempEmail[j] == '.' && !(tempEmail[j+1] == '\0')) error = 2;
            if (error == 2) break;
        }
        if (error == 2) break;
        printf("\nPlease enter a valid email. Valid emails are formatted abc@mno.xyz\n\n");
    }

    /* Get next action from user */
    c = inputEC();
    printf("%c\n", c);

    if (c == 'S') {
        arr[contactNum-1].phone_number = strtoul(tempPhoneNum, NULL, 10);
        arr[contactNum-1].email = realloc(arr[contactNum-1].email, strlen(tempEmail) + 1);
        strcpy(arr[contactNum-1].email, tempEmail);
    }

    free (tempPhoneNum);
    free (tempEmail);

    /* Return 'D' if delete selected, else '\0' */
    if (c == 'D') return('D');
    else return('\0');
}


/* Demove contact from array*/
void deleteContact(strings *arr, int contactNum, int arrSize){
    int i;

    free(arr[contactNum-1].first_name);
    free(arr[contactNum-1].last_name);
    free(arr[contactNum-1].company_name);
    free(arr[contactNum-1].email);

    for(i=contactNum; i<arrSize; i++){
        arr[i-1] = arr[i];
    }
    return;
}


/* Get action from user and check for valid input for Edit Contact */
char inputAC(){
    /* Get input */
    /* Test input for R/S */
    /* Return R/S */

    char action[5] = {'\0', '\0', '\0', '\0', '\0'};
    while(1){
        printf("Action: ");
        fgets(action, 5, stdin);
        /* Clear stdin if no newline in string */
        if (action[0] != '\n' && action[1] != '\n' && action[2] != '\n' && action[3] != '\n' && action[4] != '\n') while (getchar()!= '\n');
        if (action[1] == '\n' && (action[0] == 'R' || action[0] == 'S')) return(action[0]);

        printf("\nPlease enter a valid action:\n");
        printf("   R - Return to full contacts list\n");
        printf("   S - Save this contact\n");
    }
}


/* Add contact to array */
int addContact(strings **arr, int arrSize, int delEle){
    strings tempContact;
    int i, j, error;
    char* tempPhoneNum;
    char c;

    tempContact.first_name = malloc(1);
    tempContact.last_name = malloc(1);
    tempContact.company_name = malloc(1);
    tempContact.email = malloc(1);
    tempPhoneNum = malloc(1);

    printf("\nNew Contact\n");

    /* Get first name */
    printf("First Name: ");
    i=0;
    while(1){
        tempContact.first_name[i] = getchar();
        i++;
        tempContact.first_name = realloc(tempContact.first_name, i+1);
        if (tempContact.first_name[i-1] == '\n') break;
    }
    tempContact.first_name[i-1] = '\0';

    while(1){
        tempContact.last_name = realloc(tempContact.last_name, 1);
        tempContact.company_name = realloc(tempContact.company_name, 1);

        /* Get last name */
        printf("Last Name: ");
        i=0;
        while(1){
            tempContact.last_name[i] = getchar();
            i++;
            tempContact.last_name = realloc(tempContact.last_name, i+1);
            if (tempContact.last_name[i-1] == '\n') break;
        }
        tempContact.last_name[i-1] = '\0';

        /* Get company name */
        printf("Company Name: ");
        i=0;
        while(1){
            tempContact.company_name[i] = getchar();
            i++;
            tempContact.company_name = realloc(tempContact.company_name, i+1);
            if (tempContact.company_name[i-1] == '\n') break;
        }
        tempContact.company_name[i-1] = '\0';

        if (!(tempContact.last_name[0] == '\0' && tempContact.company_name[0] == '\0')) break;
        printf("\nPlease enter a last name or company name.\n\n");
    }

    /* Get phone number and test for 7 or 10 digits, and all numerals */
    while (1){
        error = 1;
        i=0;
        tempPhoneNum = realloc(tempPhoneNum, i+1);
        printf("Phone Number: ");

        /* Get phone number */
        while(1){
            tempPhoneNum[i] = getchar();
            i++;
            tempPhoneNum = realloc(tempPhoneNum, i+1);
            if (tempPhoneNum[i-1] == '\n') break;
        }
        tempPhoneNum[i-1] = '\0';

        /* Test phone number */
        if (i == 8 || i == 11){
            for (j=0; j<i-1; j++){
                if (tempPhoneNum[j] >= '0' && tempPhoneNum[j] <= '9') error = 0;
                else {error = 1; break;}
            }
        }
        if (error == 0) break;
        printf("\nPlease enter a seven or ten digit phone number (numerals only).\n\n");
    }
    tempContact.phone_number = strtoul(tempPhoneNum, NULL, 10);

    /* Get email and test for abc@mno.xyz format */
    while (1) {
        error = 0;
        i=0;
        tempContact.email = realloc(tempContact.email, i+1);
        printf("Email: ");

        /* Get email */
        while(1){
            tempContact.email[i] = getchar();
            i++;
            tempContact.email = realloc(tempContact.email, i+1);
            if (tempContact.email[i-1] == '\n') break;
        }
        tempContact.email[i-1] = '\0';

        /* Test email format */
        if (tempContact.email[0] == '\0') error = 2;
        for(j=1; j<i; j++){
            if (error == 0 && tempContact.email[j] == '@') {
                error = 1;
                j++;
            }
            else if (error == 1 && tempContact.email[j] == '.' && !(tempContact.email[j+1] == '\0')) error = 2;
            if (error == 2) break;
        }
        if (error == 2) break;
        printf("\nPlease enter a valid email. Valid emails are formatted abc@mno.xyz\n\n");
    }

    c = inputAC();

    if (c == 'S'){
        if (delEle == 0) *arr = realloc(*arr, sizeof(strings) * (arrSize + 1));
        (*arr)[arrSize].first_name = malloc(strlen(tempContact.first_name) + 1);
        (*arr)[arrSize].last_name = malloc(strlen(tempContact.last_name) + 1);
        (*arr)[arrSize].company_name = malloc(strlen(tempContact.company_name) + 1);
        (*arr)[arrSize].email = malloc(strlen(tempContact.email) + 1);

        strcpy((*arr)[arrSize].first_name, tempContact.first_name);
        strcpy((*arr)[arrSize].last_name, tempContact.last_name);
        strcpy((*arr)[arrSize].company_name, tempContact.company_name);
        strcpy((*arr)[arrSize].email, tempContact.email);
        (*arr)[arrSize].phone_number =  tempContact.phone_number;
    }

    free(tempContact.first_name);
    free(tempContact.last_name);
    free(tempContact.company_name);
    free(tempContact.email);
    free(tempPhoneNum);

    if (c == 'S') return(1);
    return(0);
}


/* Open file and save contact array */
void saveContacts(strings *arr, int arrSize){
    int i;
    FILE *filePtr = fopen("myContactList.db", "w+b");
    contact posn;
    long currPosn;



    for(i=0; i<arrSize; i++){
        currPosn = ftell(filePtr);

        posn.phone_number = 0;
        posn.first_name_posn = 0;
        posn.last_name_posn = 0;
        posn.company_name_posn = 0;
        posn.email_posn = 0;
        posn.next = 0;

        posn.phone_number = arr[i].phone_number;

        fwrite(&(posn), sizeof(posn), 1, filePtr);

        /* Write first name to file */
        if (arr[i].first_name[0] == '\0'); else{
            posn.first_name_posn = ftell(filePtr);
            fwrite(arr[i].first_name, strlen(arr[i].first_name)+1, 1, filePtr);
        }

        /* Write last name to file */
        if (arr[i].last_name[0] == '\0'); else{
            posn.last_name_posn = ftell(filePtr);
            fwrite(arr[i].last_name, strlen(arr[i].last_name)+1, 1, filePtr);
        }

        /* Write copmpany name to file */
        if (arr[i].company_name[0] == '\0'); else{
            posn.company_name_posn = ftell(filePtr);
            fwrite(arr[i].company_name, strlen(arr[i].company_name)+1, 1, filePtr);
        }

        /* Write email to file */
        if (arr[i].email[0] == '\0'); else{
            posn.email_posn = ftell(filePtr);
            fwrite(arr[i].email, strlen(arr[i].email)+1, 1, filePtr);
        }

        /* Update posn in file */
        posn.next = ftell(filePtr);
        fseek(filePtr, currPosn, SEEK_SET);
        fwrite(&(posn), sizeof(posn), 1, filePtr);
        fseek(filePtr, posn.next, SEEK_SET);

    }

    /* Close file */
    fclose(filePtr);
    filePtr = NULL;

    return;
}

int main(){
    /* Create array of struct strings. struct strings will save locations of strings */
    int arrSize = 0;
    int delEle = 0;
    strings *arr;
    int i;
    char action[5] = {'\0', '\0', '\0', '\0', '\0'};
    int actionNum;

    arr = malloc(1);

    /* Open file and save database to array*/
    arrSize = openFile(&arr, arrSize);

    while(1){
        sortContacts2(arr, arrSize);

        /* Display Full Contact List */
        displayContacts(arr, arrSize);

        for (i=0; i<5; i++) action[i] = '\0';
        while(action[0] != 'A' && action[0] != 'X'){
            inputFCL(action);
            actionNum = 0;
            if (action[0] > '0' && action[0] <= '9') {
                actionNum = strtol(action, NULL, 10);
                if (actionNum <= arrSize) break;
            }
            if (actionNum > arrSize){
                printf("\nPlease enter a valid action:\n");
                printf("   # - View more deatails on contact # (numerals only)\n");
                printf("   A - Add a new contact\n");
                printf("   X - Exit program\n\n");
            }
        }

        if (actionNum > 0){
            displayContactInfo(arr, actionNum);
            inputDCI(action);
        }

        if (action[0] == 'R') action[0] = '\0';
        else if (action[0] == 'E') action[0] = editContact(arr, actionNum);
        if (action[0] == 'D'){
            deleteContact(arr, actionNum, arrSize);
            arrSize--;
            delEle++;
            action[0] = '\0';
        }

        if (action[0] == 'A') {
            i = addContact(&arr, arrSize, delEle);
            if (i == 1){
                arrSize++;
                if (delEle > 0) delEle--;
            }
            action[0] = '\0';
        }

        if (action[0] == 'X') break;
    }

    /* Save array to file "contactList.db" */
    saveContacts(arr, arrSize);

    /* free mallocs */
    for(i=0; i<(arrSize); i++){
        free (arr[i].first_name);
        free (arr[i].last_name);
        free (arr[i].company_name);
        free (arr[i].email);
    }
    free (arr);

    return(0);
}
