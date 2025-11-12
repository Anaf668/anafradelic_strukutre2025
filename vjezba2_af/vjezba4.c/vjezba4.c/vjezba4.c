//Napisati program koji iz datoteke čita postfiks izraz i zatim korištenjem stoga računa
//rezultat. Stog je potrebno realizirati preko vezane liste.

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE (50)
#define MAX_LENGTH (1024)

struct _stogElement;
typedef struct _stogElement* position;
typedef struct _stogElement {
    double number;
    position next;
} stogElement;

int readFile(char* fileName, char* buffer);
position createStogElement(double number);
int getPostfixFromFile(position head, char* fileName, double* result);
int parseStringIntoPostfix(position head, char* buffer, double* result);
int checkStogAndGetResult(position head, double* result);
int printStog(position first);
int push(position head, position newStogElement);
int pop(position head, double* result);
int popAndDoOperation(position head, char operation, double* result);

int main() {
    stogElement head = { .number = 0, .next = NULL };
    double result = 0;

    if (getPostfixFromFile(&head, "postfix.txt", &result) == EXIT_SUCCESS) {
        printf("Result is: %0.1lf\n", result);
    }
    return EXIT_SUCCESS;
}

int readFile(char* fileName, char* buffer) {
    FILE* filePointer = NULL;
    filePointer = fopen(fileName, "r");
    if (!filePointer) {
        printf("File se ne može otvoriti!\n");
        return EXIT_FAILURE;
    }
    fgets(buffer, MAX_LENGTH, filePointer);
    printf("|%s|\n", buffer); //jedan red

    fclose(filePointer);
    return EXIT_SUCCESS;
}

position createStogElement(double number)
{
    position newStogElement = NULL;

    newStogElement = (position)malloc(sizeof(stogElement));
    if (!newStogElement) {
        printf("Memorija nije alocirana!\n");
        return NULL;
    }

    newStogElement->number = number;
    newStogElement->next = NULL;

    return newStogElement;
}

int getPostfixFromFile(position head, char* fileName, double* result) {
    char buffer[MAX_LENGTH] = { 0 };
    int status = 0;

    if (readFile(fileName, buffer) != EXIT_SUCCESS || !buffer) {
        return EXIT_FAILURE;
    }
    status = parseStringIntoPostfix(head, buffer, result);
    if (status != EXIT_SUCCESS) {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int parseStringIntoPostfix(position head, char* buffer, double* result)
{
    char* currentBuffer = buffer;
    int status = 0;
    int numberOfBytes = 0; //koliko znakova smo procitali
    char operation = 0;
    double number = 0.0;
    position newStogElement = NULL;
    //lf cita double, n kolko ih je procitano
    while (strlen(currentBuffer) > 0) { //duljina niza znakova
        status = sscanf(currentBuffer, " %lf %n", &number, &numberOfBytes);
        if (status != 1) { //ako nije broj onda je operator
            sscanf(currentBuffer, " %c %n", &operation, &numberOfBytes);
            status = popAndDoOperation(head, operation, result);
            //2 broja sa stoga, i izvrsava operaciju, vraca rez 
            if (status != EXIT_SUCCESS) {
                return EXIT_FAILURE;
            }
            number = *result;
        }

        newStogElement = createStogElement(number);
        if (!newStogElement) {
            return EXIT_FAILURE;
        }
        currentBuffer += numberOfBytes; //maknemo dokle smo dosli ka
        printf("|%s| <--->", currentBuffer);
        push(head, newStogElement); //novi broj na stog
    }

    return checkStogAndGetResult(head, result); //provjera jel osta jedan broj
}

int checkStogAndGetResult(position head, double* result) {
    int status = EXIT_SUCCESS;

    status = pop(head, result); //skida el s stoga i upisuje njegovu vrijednost

    if (status != EXIT_SUCCESS) {
        return status;
    } //neispravan

    if (head->next) {
        printf("Postfix ne radi, provjerite file!\r\n");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

//skidanje elementa s vrha
int pop(position head, double* result) {
    position toDelete = NULL;

    toDelete = head->next;
    if (!toDelete) {
        printf("Stog je prazan!\n");
        return -1;
    }
    head->next = toDelete->next;
    *result = toDelete->number;
    free(toDelete);

    return EXIT_SUCCESS;
}

//upis novog elementa na vrh
int push(position head, position newStogElement) {
    newStogElement->next = head->next;
    head->next = newStogElement;
    printStog(head->next);

    return EXIT_SUCCESS;
}

int printStog(position first) {
    position current = first;

    while (current) {
        printf(" %0.1lf", current->number);
        current = current->next;
    }
    printf("\n");
    return EXIT_SUCCESS;
}

int popAndDoOperation(position head, char operation, double* result) {
    double broj1 = 0;
    double broj2 = 0;
    int status1 = 0;
    int status2 = 0;

    status1 = pop(head, &broj1); //gornji br sa stoga
    if (status1 != EXIT_SUCCESS) {
        return EXIT_FAILURE;
    }
    status2 = pop(head, &broj2); //donji broj sa stoga
    if (status2 != EXIT_SUCCESS) {
        return EXIT_FAILURE;
    }

    if (operation == '+') *result = broj2 + broj1;
    else if (operation == '-') *result = broj2 - broj1;
    else if (operation == '*') *result = broj2 * broj1;
    else if (operation == '/') *result = broj2 / broj1;
    else {
        printf("Operacija %c nije podržana!\r\n", operation); //ako je krivi zapis u fileu
        return EXIT_FAILURE;
    } return EXIT_SUCCESS;
}