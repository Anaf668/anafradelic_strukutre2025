#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#define PERSON_NOT_FOUND (-1)
#define ALLOCATION_ERROR (-1)
#define FILE_OPEN_ERROR (-1)
#define MAX_LINE (1024)
#define MAX_LENGTH (30)

struct _Person;
typedef struct _Person* Position;
typedef struct _Person
{
	char name[MAX_LENGTH];
	char surname[MAX_LENGTH];
	int birthYear;
	Position next; //pokazuje na sljedecu osobu u listi
}Person;

Position createPerson();
int addToBeginning(Position head);
int printPerson(Position Person);
int printList(Position firstPerson);
int addToEnd(Position head);
int deletePerson(Position head);
int findPerson(Position Head);
Position findLast(Position head);
int read(Position head);
int write(Position p);
int addInFront(Position p);
int addBehind(Position p);
int sortList(Position head);
int deleteAll(Position p);

int main() {
	Person Head = { //glava ka te nekakve liste kako ce izgledat, prazna
		.name = " ",
		.surname = " ",
		.birthYear = 0,
		.next = NULL
	};
	Position q = &Head;
	while (1) {
		int choice;
		printf("\nUnesi 1 za unos na pocetak lste, \n\t2 za ispis liste,\n\t3 za unos na kraj liste,\n\t4 za pretragu po prezimenu,\n\t5 za brisanje osoba i\n\t6 - Dodaj ispred osobe\n\t7 - Dodaj iza osobe\n\t8 - Spremi listu u datoteku\n\t9 - Ucitaj listu iz datoteke\n\t10 - Sortiraj listu po prezimenima\n\t11 - Obrisi sve osobe iz liste\n\t0 za izlaz iz programa.\n");
		scanf("%d", &choice);
		if (choice == 1) addToBeginning(q);
		else if (choice == 2) printList(q->next); //samo cita iz liste pa nesmimo od heada krenit
		else if (choice == 3) addToEnd(q);
		else if (choice == 4)findPerson(q);
		else if (choice == 5) deletePerson(q);
		else if (choice == 6) addInFront(q);
		else if (choice == 7) addBehind(q);
		else if (choice == 8) write(q->next);
		else if (choice == 9) read(q);
		else if (choice == 10) sortList(q);
		else if (choice == 11) deleteAll(q);
		else if (choice == 0) break;
		else printf("Nisi unio ispravan broj\n");
	}
	return EXIT_SUCCESS;

}

//a) null pointer na nista, alokacija na tip position, dinam dodajte->malloc(), novi el na kraj liste

Position createPerson() {
	char name[MAX_LENGTH] = " ";
	char surname[MAX_LENGTH] = " ";
	int birthYear = 0;
	Position newPerson = NULL; //pokazivace uvik na 0 postavljamo

	newPerson = (Position)malloc(sizeof(Person)); //memorija za person, vraca void pa ovo isprid
	if (!newPerson) {
		printf("nije uspjesna alokacija memorije!");
		return NULL;
	}

	//unos 
	printf("Ime: \n");
	scanf("%s", name);
	printf("Prezime: \n");
	scanf("%s", surname); //string pokazuje vec na prvi element
	printf("Godina: \n");
	scanf("%d", &birthYear); //triba adresa jer triba upisat broj na tu adresu

	//za kopiranje iz jednog u  drugi: strcpy(gdje zelimo kopirat string, odakle kopiramo string)
	//string se kopira s strcpy jer nemozemo stavit samo jednakost
	strcpy(newPerson->name, name);
	strcpy(newPerson->surname, surname);
	newPerson->birthYear = birthYear;
	newPerson->next = NULL;
	//ide -> jer je newPerson pokazivac, da smo imali samo obicno bilo bi newPerson.name, name

	return newPerson;
}

int addToBeginning(Position head) {
	Position newPerson = NULL;
	newPerson = createPerson();

	if (!newPerson) {
		printf("neuspjela alokacija memorije!");
		return EXIT_FAILURE;
	}

	newPerson->next = head->next;
	//znaci da novi cvor pokazuje na stari prvi element
	head->next = newPerson;
	//novi cvor postaje dio liste, tj njen prvi element
	//q=&Head, head pokazuje na prvi element
	//Head->p1->NULL, q->next=p1, kreiramo novu osob position newPerson=p2, ona nema veze s listom, =NULL
	//povezat liste: p2->next pokazuje na p1, tj sacuvana je stara lista (p2->p1->NULL)
	//namisiti jos da head pokazuje na newPerson(p2), q->next=newPerson, p1 ostaje iza p2, a head pokazuje na novi cvor(Head->p2->p1->NULL)
	return EXIT_SUCCESS;
}

int printPerson(Position Person) {
	printf("%s\t%s\t %d\n", Person->name, Person->surname, Person->birthYear);
	return EXIT_SUCCESS;
}

//b)ispisuje listu
int printList(Position firstPerson) {
	Position current = firstPerson;
	if (!firstPerson) {
		printf("lista je prazna");
		return EXIT_SUCCESS;
	}

	printf("Ime:\tPrezime:\tGodina rodenja:\n");
	while (current) {
		printPerson(current);
		current = current->next;
	}
	return EXIT_SUCCESS;
}

Position findLast(Position head) {
	Position current = head;
	while (current->next != NULL) {
		current = current->next;
	}
	return current;
}

//c)dinamicki dodajte novi element na kraj liste
int addToEnd(Position head) { //head-stvarni prvi element
	Position newPerson = createPerson();
	if (!newPerson) {
		return EXIT_FAILURE;
	}
	Position last = findLast(head); //ako je lista prazna vraca samo head, last->next pokazuje na nulu u pocetku
	last->next = newPerson; //doda novi element na kraj, poveyane liste s NULL na kraju
	return EXIT_SUCCESS;
}

char* enterSurname() {
	char surname[MAX_LENGTH] = " ";
	printf("prezime osobe je:\n");
	scanf("%s", surname);
	return surname;
}

//d)pronalazi element u listi po prezimenu
int findPerson(Position head) {
	Position current = head;
	char surname[MAX_LENGTH] = " ";
	if (!head) {
		printf("Lista je prazna!");
		return EXIT_FAILURE;
	}
	strcpy(surname, enterSurname()); //iskopira string iz ove funckijr u surname
	while (current != NULL) { //dok ne dodemo do kraja
		if (strcmp(current->surname, surname) == 0) {
			//dok se dva prezimene ne poklope tj prezime s trenutne pozicije ne poklopi s trazenim prezimenom, !=0->dok nije NULL, tj kraj
			printPerson(current);
			return current;
		}
		current = current->next;
	}
	return PERSON_NOT_FOUND;
}

//e)brise element iz liste-prezime trazeno
int deletePerson(Position head) {
	char surname[MAX_LENGTH] = " ";
	strcpy(surname, enterSurname());

	Position previous = head;
	Position current = head->next;

	while (current != NULL && strcmp(current->surname, surname) != 0) {
		//nije kraj liste i prezime trenutnog razlic od trazenog
		previous = current;
		current = current->next;
	}
	//dok ne pronademo cvor s trazenin prezimenon
	if (current == NULL) {
		printf("Osoba s prezimenom %s nije pronadena", surname);
		return EXIT_FAILURE;
	}
	previous->next = current->next; //preskoci current zapravo
	printf("sljedeca osoba je izbrisana");
	printPerson(current);
	free(current);

	return EXIT_SUCCESS;
}
int addBehind(Position p) // dodajte novi element iza odredenog elementa
{
	char sur[MAX_LENGTH];

	printf("Upsii prezime ispird kojeg zelis: ");
	scanf("%s", &sur);
	printf("\n");

	Position current = p->next;
	while (current != NULL && strcmp(current->surname, sur) != 0)
	{
		current = current->next;
	}
	if (current == NULL) {
		printf("\nOsoba s tim prezimenom ne postoji!\n\n");
		return EXIT_FAILURE;
	}

	Position newPerson = createPerson();

	if (newPerson)
	{
		newPerson->next = current->next;
		current->next = newPerson;
	}

	return EXIT_SUCCESS;
}

int addInFront(Position p) // dodaj element ispred odredenog elementa
{
	char sur[MAX_LENGTH];

	printf("Upisi prezime osobe ispred koje zelis dodat: ");
	scanf("%s", &sur);
	printf("\n");

	Position previous = NULL;

	while (p != NULL && strcmp(p->surname, sur) != 0)
	{
		previous = p;
		p = p->next;
	}
	if (p == NULL)
		printf("\nOsoba s tim prezimenom ne postoji!\n\n");

	Position newPerson = NULL;

	newPerson = createPerson();

	if (newPerson)
	{
		newPerson->next = previous->next;
		previous->next = newPerson;
	}

	return EXIT_SUCCESS;
}

int write(Position p) //upisuje listu u datoteku
{
	FILE* fp = fopen("persons.txt", "w");

	if (fp == NULL)
	{
		printf("Greska!\n");
		return FILE_OPEN_ERROR;
	}

	while (p != NULL)
	{
		fprintf(fp, "%s\t %s\t %d\t\n", p->name, p->surname, p->birthYear);
		p = p->next;
	}

	fclose(fp);

	return 0;
}

int read(Position head) //cita listu iz datoteke
{
	char file[MAX_LENGTH];
	int b = 0;
	Person Head;
	Head.next = NULL;
	Position p = &Head;
	Position q = NULL;

	printf("Upisi ime fajla iz kojeg zelis iscitat listu: ");
	scanf(" %s", &file);
	FILE* fp = NULL;
	fp = fopen(file, "r");
	if (fp == NULL)
	{
		printf("Greska!\n");
		return FILE_OPEN_ERROR;
	}

	while (!feof(fp))
	{
		if (fgetc(fp) == '\n')
			b++;
	}
	rewind(fp);
	if (b != 0)
	{
		for (int i = 0; i < b; i++) {
			q = (Position)malloc(sizeof(Person));
			if (q == NULL)
			{
				printf("Ne moze alocirati memoriju!\n");
				return ALLOCATION_ERROR;
			}
			fscanf(fp, "%s %s %d", q->name, q->surname, &q->birthYear);
			q->next = p->next;
			p->next = q;
			p = p->next;
		}
	}
	fclose(fp);
	printList(Head.next);
	return 0;
}

int deleteAll(Position p) {
	Position q = NULL;
	while (p->next != 0) {
		q = p->next;
		p->next = q->next;
		free(q);
	}
	return 0;
}

int sortList(Position head) {
	if (!head->next || !head->next->next) {
		printf("Lista ima manje od dva elementa - nije potrebno sortiranje.\n");
		return EXIT_SUCCESS;
	}

	Position end = NULL;
	Position p = NULL;
	Position q = NULL;
	Position temp = NULL;

	while (head->next->next != end) {
		q = head;
		p = head->next;
		while (p->next != end) {
			char s1[MAX_LENGTH], s2[MAX_LENGTH];
			strcpy(s1, p->surname);
			strcpy(s2, p->next->surname);

			for (int i = 0; s1[i]; i++) s1[i] = tolower(s1[i]);
			for (int i = 0; s2[i]; i++) s2[i] = tolower(s2[i]);

			if (strcmp(s1, s2) > 0) {
				temp = p->next;
				p->next = temp->next;
				temp->next = p;
				q->next = temp;

				p = temp;
			}
			q = p;
			p = p->next;
		}
		end = p;
	}

	printf("Lista je sortirana po prezimenima.\n");
	return EXIT_SUCCESS;
}