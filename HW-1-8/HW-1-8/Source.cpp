#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct WordsList {
	char * word;
	int count;
	WordsList * prev;
	WordsList * next;
};

int no_file() {
	printf_s("\nCan't open the file!\n");
	return 1;
}

int no_memory() {
	printf_s("\nCan't reserve memory!\n");
	return 2;
}

int initWordsList(WordsList ** words, char * word = "") {
	(*words) = (WordsList *)malloc(sizeof(WordsList));
	if (*words == NULL) return no_memory();

	(*words)->word = (char *)malloc(strlen(word) + 1);
	if ((*words)->word == NULL) {
		free(*words);
		*words = NULL;
		return no_memory();
	}
	strcpy_s((*words)->word, strlen(word) + 1, word);
	(*words)->count = 1;
	(*words)->prev = NULL;
	(*words)->next = NULL;

	return 0;
}

int addWord(WordsList * words, char * word) {
	if (strcmp(words->word, "") != 0) {
		while (words->next != NULL) words = words->next;

		words->next = (WordsList *)malloc(sizeof(WordsList));
		if (words->next == NULL) return no_memory();

		words->next->word = (char *)malloc(strlen(word) + 1);
		if (words->next->word == NULL) {
			free(words->next);
			words->next = NULL;
			return no_memory();
		}
		strcpy_s(words->next->word, strlen(word) + 1, word);
		words->next->count = 1;
		words->next->prev = words;
		words->next->next = NULL;
	}
	else {
		free(words->word);
		words->word = (char *)malloc(strlen(word) + 1);
		if (words->word == NULL) return no_memory();
		strcpy_s(words->word, strlen(word) + 1, word);
		words->count = 1;
		words->prev = NULL;
		words->next = NULL;
	}

	return 0;
}

int findWord(WordsList * words, char * word) {
	int i = 0;
	do {
		if (strcmp(words->word, word) == 0) return i;
		words = words->next;
		i++;
	} while (words != NULL);

	return 0;
}

int incWord(WordsList * words, int number) {
	for (int i = 0; i < number; i++) {
		words = words->next;
	}
	words->count++;

	return 0;
}

int printWords(WordsList * words) {
	while (words != NULL) {
		printf_s("Word \"%s\" was %d times.\n", words->word, words->count);
		words = words->next;
	}

	return 0;
}

int freeWordsList(WordsList * words) {
	WordsList * current;
	WordsList * next = words;

	while (next != NULL) {
		current = next;
	
		next = current->next;
		free(current->word);
		free(current);
	};

	return 0;
}

int main() {
	char buf[256];
	FILE * file;
	WordsList * words;

	printf_s("Enter path to file:\n> ");
	gets_s(buf, 255);
	fopen_s(&file, buf, "r");
	 
	if (file == NULL) return no_file();

	if (initWordsList(&words) != 0) return 3;

	while (!feof(file)) {
		fscanf_s(file, "%s", buf, 255);
		int i = findWord(words, buf);
		if (i == 0) {
			addWord(words, buf);
		}
		else {
			incWord(words, i);
		};
	}

	printWords(words);

	freeWordsList(words);
	fclose(file);
	system("pause");
	return 0;
}