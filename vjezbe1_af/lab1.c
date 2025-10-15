#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#define MAX_LINE (1024) //maksimalan broj linija u datoteci
#define MAX_SIZE (30)
#define MAX_POINTS (15)
#define FILE_NOT_OPEN (-1) //sta se dogada pri izlazu iz fje


typedef struct _student
{
	char name[MAX_SIZE];
	char surname[MAX_SIZE];
	int points;
}Student;

int readFromFile(FILE* filePointer, Student* Student, int counter);

int main() {
	int counter = 0;
	FILE* filePointer = NULL;
	filePointer = fopen("student.txt", "r");

	if (!filePointer) {
		printf("\nDatoteka se ne moze ucitati.");
		return FILE_NOT_OPEN;
	}

	while (!feof(filePointer)) { //dok ne dodemo do end of file (eof)
		if (fgetc(filePointer) == '\n')
			counter++; //fgetc gleda sljedeci znak
	}
	counter++; //zadnji student kad se upise ne stavljamo novi red, pa njega dodamo sami
	printf("Upisano je %d studenata\n", counter);

	Student* student = NULL;
	student = (Student*)malloc(counter * sizeof(Student));
	if (!student) {
		printf("\nAlokacija memorije nije uspjela");
	}

	rewind(filePointer); //vraca na pocetak datoteke
	readFromFile(filePointer, student, counter);
	fclose(filePointer); //uvik zatvarat datoteku

	return EXIT_SUCCESS;
}

int readFromFile(FILE* filePointer, Student* student, int counter) {
	int i;
	double relativePoints;
	printf("ime i prezime:\t bodovi:   postotak:\t\n");
	for (i = 0; i < counter; i++) {
		int ok = fscanf(filePointer, "%s %s %d", student[i].name, student[i].surname, &student[i].points);
		if (!ok) {
			printf("error");
			return EXIT_FAILURE;
		}
		relativePoints = ((double)student[i].points / MAX_POINTS) * 100;
		printf("%s\t%s\t %d\t %.2lf \n", student[i].name, student[i].surname, student[i].points, relativePoints);
	}
	return EXIT_SUCCESS;
}