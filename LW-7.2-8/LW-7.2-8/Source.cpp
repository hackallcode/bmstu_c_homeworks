#include <stdlib.h>
#include <stdio.h>
#include <locale.h>
#include <string.h>

struct Book {
	char fio[128];
	char name[32];
	char publ[32];
	int year;
	int pages;
};

bool open(Book ** books, int * n, char * filename) {
	FILE * file;
	fopen_s(&file, filename, "rb"); // rb - чтение в бинарном виде
	if (file == NULL) return false;

	fread(n, sizeof(int), 1, file);
	*books = (Book *)malloc(sizeof(Book) * (*n));

	for (int i = 0; i < *n; i++)
		fread(&(*books)[i], sizeof(Book), 1, file);

	fclose(file);
	return true;
}

void printBooks(Book * books, int n) {
	for (int i = 0; i < n; i++) {
		printf_s("ФИО: %s\nНазвание: %s\nИздательство: %s\nГод: %d\nКоличество страниц: %d\n\n",
			books[i].fio, books[i].name, books[i].publ, books[i].year, books[i].pages);
	}
}

int main() {
	setlocale(LC_ALL, "rus");

	Book * books = NULL;
	int n = 0;
	open(&books, &n, "books");
	printBooks(books, n);

	free(books);
	system("pause");
	return 0;
}