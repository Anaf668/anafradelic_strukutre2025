#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define PERSON_NOT_FOUND (-1)
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
		printf("\nUnesi 1 za unos na pocetak lste, \n\t2 za ispis liste,\n\t3 za unos na kraj liste,\n\t4 za pretragu po prezimenu,\n\t5 za brisanje osoba i\n\t0 za izlaz iz programa.\n");
		scanf("%d", &choice);
		if (choice == 1) addToBeginning(q);
		else if (choice == 2) printList(q->next); //samo cita iz liste pa nesmimo od heada krenit
		else if (choice == 3) addToEnd(q);
		else if (choice == 4)findPerson(q);
		else if (choice == 5) deletePerson(q);
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