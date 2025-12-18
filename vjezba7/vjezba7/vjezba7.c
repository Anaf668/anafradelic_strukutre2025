/*
Napisati program koji pomocu vezanih listi (stabala) predstavlja strukturu direktorija.
Omoguciti unos novih direktorija i pod-direktorija, ispis sadrzaja direktorija i
povratak u prethodni direktorij. Tocnije program treba preko menija simulirati
koristenje DOS naredbi: 1- "md", 2 - "cd dir", 3 - "cd..", 4 - "dir" i 5 – izlaz.
*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_SIZE 50

struct _directory;
typedef struct _directory* PositionDirectory;
typedef struct _directory {
    char name[MAX_NAME_SIZE];
    PositionDirectory subDirectories;
    PositionDirectory next;
} Directory;

struct _levelStack;
typedef struct _levelStack* PositionLevelStack;
typedef struct _levelStack {
    PositionDirectory directoryLevel;
    PositionLevelStack next;
} LevelStack;

PositionDirectory createDirectory(char name[MAX_NAME_SIZE]);
PositionDirectory createSubdirectory(char name[MAX_NAME_SIZE], PositionDirectory currentDirectory);
PositionDirectory changeDirectory(char name[MAX_NAME_SIZE], PositionDirectory currentDirectory);
int listDirectoryContents(PositionDirectory currentDirectory);

PositionDirectory pop(PositionLevelStack headLevelStack);
int push(PositionLevelStack headLevelStack, PositionDirectory directoryLevel);
PositionLevelStack createNewLevelStackElement(PositionDirectory directoryLevel);

int main() {
    Directory headDirectory = {
        .name = {0},
        .subDirectories = NULL,
        .next = NULL
    };
    PositionDirectory rootDirectory = createDirectory("C:");
    headDirectory.next = rootDirectory;

    PositionDirectory currentDirectory = rootDirectory;

    LevelStack headLevelStack = {
        .directoryLevel = NULL,
        .next = NULL
    };
    push(&headLevelStack, currentDirectory);

    while (1) {
        printf("Menu:\n");
        printf("1 - md (Create Directory)\n");
        printf("2 - cd dir (Change Directory)\n");
        printf("3 - cd.. (Go Up)\n");
        printf("4 - dir (List Contents)\n");
        printf("5 - exit\n");

        char choice[10];
        printf("Unesi broj: ");
        scanf("%s", choice);

        if (strcmp(choice, "1") == 0) {
            char directoryName[MAX_NAME_SIZE];
            printf("Unesi ime direktorija: ");
            scanf("%s", directoryName);
            createSubdirectory(directoryName, currentDirectory);
        }
        else if (strcmp(choice, "2") == 0) {
            char directoryName[MAX_NAME_SIZE];
            printf("Enter directory name: ");
            scanf("%s", directoryName);
            currentDirectory = changeDirectory(directoryName, currentDirectory);
            push(&headLevelStack, currentDirectory);
        }
        else if (strcmp(choice, "3") == 0) {
            if (currentDirectory != rootDirectory)
            {
                currentDirectory = pop(&headLevelStack);
                printf("Trenutna lokacija je '%s'\n", currentDirectory->name);
            }
            else {
                printf("Lokacija je root direktorij.\n");
                return currentDirectory;
            }
        }
        else if (strcmp(choice, "4") == 0) {
            listDirectoryContents(currentDirectory);
        }
        else if (strcmp(choice, "5") == 0) {
            printf("Izlaz iz programa.\n");
            break;
        }
        else {
            printf("Neuspjesan unos, pokusajte ponovno.\n");
        }
    }
    // cistit alociranu memoriju
    free(rootDirectory);

    return 0;
}

PositionDirectory createDirectory(char name[MAX_NAME_SIZE]) {
    PositionDirectory newDirectory = NULL;
    newDirectory = (PositionDirectory)malloc(sizeof(Directory));
    if (!newDirectory) {
        printf("Neuspjesna alokacija memorije!\n");
        return NULL;
    }
    strcpy(newDirectory->name, name);
    newDirectory->subDirectories = NULL;
    newDirectory->next = NULL;
    return newDirectory;
}

PositionDirectory createSubdirectory(char name[MAX_NAME_SIZE], PositionDirectory currentDirectory) {
    PositionDirectory newDirectory = NULL;
    newDirectory = createDirectory(name);
    if (!newDirectory) {
        printf("Neuspjesno kreiranje direktorija!\n");
        return NULL;
    }
    newDirectory->next = currentDirectory->subDirectories;
    currentDirectory->subDirectories = newDirectory;
    return newDirectory;
}

PositionDirectory changeDirectory(char name[MAX_NAME_SIZE], PositionDirectory currentDirectory) {
    PositionDirectory subdirectory = currentDirectory->subDirectories;
    while (subdirectory != NULL) {
        if (strcmp(subdirectory->name, name) == 0) {
            return subdirectory;
        }
        subdirectory = subdirectory->next;
    }
    printf("Direktorij '%s' nije pronaden.\n", name);
    return currentDirectory;
}

int listDirectoryContents(PositionDirectory currentDirectory) {
    printf("Sadrzaji direktorija '%s':\n", currentDirectory->name);
    PositionDirectory subdirectory = currentDirectory->subDirectories;
    while (subdirectory != NULL) {
        printf(" - %s\n", subdirectory->name);
        subdirectory = subdirectory->next;
    }
    if (currentDirectory->subDirectories == NULL) {
        printf("   (prazno)\n");
    }
    return EXIT_SUCCESS;
}

PositionDirectory pop(PositionLevelStack headLevelStack) {
    PositionLevelStack toDelete = NULL;
    PositionDirectory directoryLevel = NULL;

    toDelete = headLevelStack->next;
    if (!toDelete) {
        printf("Stog je prazan, nije moguc pop!\n");
        return NULL;
    }

    headLevelStack->next = toDelete->next;
    directoryLevel = toDelete->directoryLevel;
    free(toDelete);

    return directoryLevel;
}

int push(PositionLevelStack headLevelStack, PositionDirectory directoryLevel) {
    PositionLevelStack newLevelStackElement = NULL;

    newLevelStackElement = createNewLevelStackElement(directoryLevel);
    if (!newLevelStackElement) {
        perror("Greska!\n");
        return NULL;
    }

    newLevelStackElement->next = headLevelStack->next;
    headLevelStack->next = newLevelStackElement;
}

PositionLevelStack createNewLevelStackElement(PositionDirectory directoryLevel) {
    PositionLevelStack newLevelStackElement = NULL;

    newLevelStackElement = (PositionLevelStack)malloc(sizeof(LevelStack));
    if (!newLevelStackElement) {
        perror("Neuspjesna alokacija memorije!\n");
        return NULL;
    }

    newLevelStackElement->directoryLevel = directoryLevel;
    newLevelStackElement->next = NULL;

    return newLevelStackElement;
}