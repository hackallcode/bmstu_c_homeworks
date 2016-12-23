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

struct BookList {
	Book b;
	BookList * next;
};

bool addBook(BookList ** books, Book * book) {
	BookList * last = NULL;
	if (*books == NULL) {
		*books = (BookList *)malloc(sizeof(BookList));
		if (*books == NULL) return false;
		last = *books;
	}
	else {
		last = *books;
		while (last->next != NULL) last = last->next;
		last->next = (BookList *)malloc(sizeof(BookList));
		if (last->next == NULL) return false;
		last = last->next;
	}
	strcpy_s(last->b.fio, 128, book->fio);
	strcpy_s(last->b.name, 32, book->name);
	strcpy_s(last->b.publ, 32, book->publ);
	last->b.year = book->year;
	last->b.pages = book->pages;
	last->next = NULL;
	return true;
}

void freeBooks(BookList ** books) {
	BookList * current = *books;
	BookList * next = NULL;
	while (next != NULL) {
		next = current->next;
		free(current);
		current = next;
	}
	*books = NULL;
}

bool save(BookList * books, int n, char * filename) {
	FILE * file;
	fopen_s(&file, filename, "wb"); // wb - запись в бинарном виде
	if (file == NULL) return false;

	fwrite(&n, sizeof(int), 1, file);

	BookList * current = books;
	for (int i = 0; i < n; i++) {
		fwrite(&current->b, sizeof(Book), 1, file);
		current = current->next;
	}

	fclose(file);
	return true;
}

int main() {
	setlocale(LC_ALL, "rus");

	int n = 0;
	BookList * books = NULL;
	Book book;

	printf_s("Введите количество книг:\n");
	scanf_s("%d", &n);
	while (n < 1) {
		printf_s("Введите КОРРЕКТНОЕ количество книг:\n");
		scanf_s("%d", &n);
	}

	for (int i = 0; i < n; i++) {
		system("cls");
		printf_s("Введите ФИО:\n");
		do gets_s(book.fio, 127); while (strcmp(book.fio, "") == 0);
		printf_s("Введите название:\n");
		do gets_s(book.name, 31); while (strcmp(book.name, "") == 0);
		printf_s("Введите издательство:\n");
		do gets_s(book.publ, 31); while (strcmp(book.publ, "") == 0);
		printf_s("Введите год издания:\n");
		scanf_s("%d", &book.year);
		printf_s("Введите количество страниц:\n");
		scanf_s("%d", &book.pages);
		if (!addBook(&books, &book)) printf_s("Невозможно добавить книгу!\n");
	}

	if (!save(books, n, "books")) printf_s("Невозможно сохранить в файле!\n");
	freeBooks(&books);
	system("pause");
	return 0;
}