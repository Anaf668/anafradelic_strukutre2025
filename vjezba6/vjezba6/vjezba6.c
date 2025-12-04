/*
Napisati program koji cita datoteku racuni.txt u kojoj su zapisani nazivi svih datoteka koji
predstavljaju pojedini racun. Na pocetku svake datoteke je zapisan datum u kojem vremenu je
racun izdat u formatu YYYY-MM-DD. Svaki sljedeci red u datoteci predstavlja artikl u formatu
naziv, kolicina, cijena. Potrebno je formirati vezanu listu racuna sortiranu po datumu. Svaki cvor
vezane liste sadrzava vezanu listu artikala sortiranu po nazivu artikla. Nakon toga potrebno je
omoguciti upit kojim ce korisnik saznati koliko je novaca sveukupno potroseno na specificni
artikl u odredenom vremenskom razdoblju i u kojoj je kolicini isti kupljen.
*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 1024

typedef struct nebitno_ {
	char product[MAX_LINE];
	int quantity, price;
	struct nebitno_* next;
}bill_;

typedef struct nebitno {
	int day, month, year;
	bill_* billhead;
	struct nebitno* next;
}receipts_;

typedef receipts_* receipts;
typedef bill_* bill;

int readfromfile(receipts head);
int rowcounter();
int openbills(receipts head, int rownumber);
int openandinsert(receipts head, char* billname);
int readbills(bill head, char* billname);
int billinsert(bill head, char* tempname, int tprice, int tquan);
int sortdates(receipts head);
int menu(receipts head);

int main()
{
	int test = 0;

	receipts_ head;
	head.next = NULL;

	test = readfromfile(&head);
	if (test == -1)
	{
		printf("Datoteka se nije uspjela otvoriti");
		return -1;
	}

	sortdates(&head);
	menu(head.next);

	return 0;
}

int readfromfile(receipts head)
{
	int test = 0;
	int rownumber;

	rownumber = rowcounter();
	if (rownumber == -1)
	{
		return -1;
	}

	test = openbills(head, rownumber);
	if (test == -1)
	{
		return -1;
	}
}

int rowcounter()
{
	char buffer[MAX_LINE];
	int rowsnumber = 0;

	FILE* fptr = fopen("racuni.txt", "r");

	if (fptr == NULL)
	{
		return -1;
	}

	while (!feof(fptr))
	{
		fgets(buffer, MAX_LINE, fptr);
		rowsnumber++;
	}

	fclose(fptr);

	return rowsnumber;
}

int openbills(receipts head, int rownumber)
{
	int test = 0;
	char buffer[MAX_LINE];

	FILE* fptr = fopen("racuni.txt", "r");

	if (fptr == NULL)
	{
		return -1;
	}

	for (int i = 0; i < rownumber; i++)
	{
		fgets(buffer, MAX_LINE, fptr);
		test = openandinsert(head, buffer);
		if (test == -1)
		{
			return -1;
		}
	}

	fclose(fptr);
}

int openandinsert(receipts head, char* billname)
{
	int test = 0;
	int dayt = 0, montht = 0, yeart = 0;

	char tbuffer[MAX_LINE];
	strcpy(tbuffer, billname);

	if (tbuffer[strlen(tbuffer) - 1] == '\n')
		tbuffer[strlen(tbuffer) - 1] = '\0';

	FILE* fptr = fopen(tbuffer, "r");
	if (fptr == NULL)
	{
		return -1;
	}
	receipts ptr = malloc(sizeof(receipts_));

	fscanf(fptr, "%d:%d:%d", &dayt, &montht, &yeart);
	ptr->day = dayt;
	ptr->month = montht;
	ptr->year = yeart;

	ptr->billhead = malloc(sizeof(bill_));
	ptr->billhead->next = NULL;

	test = readbills(ptr->billhead, tbuffer);
	if (test == -1)
	{
		return -1;
	}

	ptr->next = head->next;
	head->next = ptr;

	fclose(fptr);
}

int readbills(bill head, char* billname)
{
	char tbuffer[MAX_LINE];
	strcpy(tbuffer, billname);
	FILE* fptr = fopen(tbuffer, "r");
	if (fptr == NULL)
	{
		return -1;
	}

	while (fgetc(fptr) != '\n');

	char tempname[MAX_LINE];
	int tquan, tprice;

	while (fscanf(fptr, "%s %d %d", tempname, &tprice, &tquan) != EOF)
	{
		billinsert(head, tempname, tprice, tquan);
	}

	fclose(fptr);
}

int billinsert(bill head, char* tempname, int tprice, int tquan)
{
	bill ptr = malloc(sizeof(bill_));

	strcpy(ptr->product, tempname);
	ptr->price = tprice;
	ptr->quantity = tquan;

	ptr->next = head->next;
	head->next = ptr;
}

int sortdates(receipts head)
{
	receipts temp, j, j_prev, end;
	end = NULL;

	while (head->next != end)
	{
		j_prev = head;
		j = head->next;
		while (j->next != end)
		{
			if (j->year < j->next->year)
			{
				temp = j->next;
				j_prev->next = temp;
				j->next = temp->next;
				temp->next = j;

				j = temp;
			}
			else if (j->month < j->next->month && j->year == j->next->year)
			{
				temp = j->next;
				j_prev->next = temp;
				j->next = temp->next;
				temp->next = j;

				j = temp;
			}
			else if (j->day < j->next->day && j->month == j->next->month && j->year == j->next->year)
			{
				temp = j->next;
				j_prev->next = temp;
				j->next = temp->next;
				temp->next = j;

				j = temp;
			}
			j_prev = j;
			j = j->next;
		}
		end = j;
	}

}

int menu(receipts head)
{
	char buffer[MAX_LINE];
	int test;

	receipts temp = head;
	bill tempp = head->billhead;

	do {
		test = 1;
		printf("Koji produkt zelite istraziti(0 za prekid):\n");
		scanf("%s", buffer);

		if (strcmp(buffer, "0") == 0)
		{
			return 0;
		}

		head = temp;
		head->billhead = tempp;
		while (head != NULL)
		{
			while (head->billhead->next != NULL)
			{
				if (strcmp(head->billhead->next->product, buffer) == 0)
				{
					printf("Product %s je kupljen na datum %d:%d:%d u kolicini %d\n", buffer, head->day, head->month, head->year, head->billhead->next->quantity);
					test = 2;
					break;
				}
				head->billhead = head->billhead->next;
			}
			head = head->next;
		}

		if (test == 1)
		{
			printf("Produkt koji ste unijeli ne postoji na listi\n");
		}

	} while (test);
}