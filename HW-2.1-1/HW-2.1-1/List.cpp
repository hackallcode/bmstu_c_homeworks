#include <stdlib.h>
#include <stdio.h>
#include "List.h"

List * createList(data * data, int data_number) {
	List * list = NULL;
	List * current = NULL;
	for (int i = 0; i < data_number; i++) {
		if (i == 0) {
			list = (List *)malloc(sizeof(List));
			if (list == NULL) return NULL;
			list->data = data[0];
			list->next = NULL;
			current = list;
		}
		else {
			current->next = (List *)malloc(sizeof(List));
			if (current->next == NULL) return NULL;
			current->next->data = data[i];
			current->next->next = NULL;
			current = current->next;
		}
	}
	return list;
}

int addLast(List ** list, data data) {
	if (*list == NULL) {
		*list = (List *)malloc(sizeof(List));
		if (*list == NULL) return -1;
		(*list)->data = data;
		(*list)->next = NULL;
		return 0;
	}
	List * current = *list;
	while (current->next != NULL)
		current = current->next;
	current->next = (List *)malloc(sizeof(List));
	if (current->next == NULL) return -1;
	current->next->data = data;
	current->next->next = NULL;
	return 0;
}

int add(List ** list, data data, int number) {
	if (number < 0) return -1;
	if (number == 0) {
		List * temp = (List *)malloc(sizeof(List));
		if (temp == NULL) return -1;
		temp->data = data;
		temp->next = *list;
		*list = temp;
		return 0;
	}

	List * current = *list;
	for (int i = 0; i < number - 1; i++) {
		current = current->next;
		if (current->next == NULL) break;
	}		

	List * temp = (List *)malloc(sizeof(List));
	if (temp == NULL) return -1;
	temp->data = data;
	temp->next = current->next;
	current->next = temp;
	return 0;
}

void remove(List ** list, int number) {
	if (number < 0) return;
	if (number == 0) {
		free(*list);
		*list = NULL;
		return;
	}

	List * current = *list;
	for (int i = 0; i < number - 1; i++) {
		current = current->next;
		if (current->next == NULL) break;
	}

	List * temp = current->next;
	current->next = current->next->next;
	free(temp);
}

List * join(List * a, List * b) {
	List * list = NULL;
	
	while (a != NULL) {
		int code = addLast(&list, a->data);
		if (code != 0) return NULL;
		a = a->next;
	}
	
	while (b != NULL) {
		int code = addLast(&list, b->data);
		if (code != 0) return NULL;
		b = b->next;
	}
	
	return list;
}

List * convert(List * list) {
	List * newList = NULL;

	while (list != NULL) {
		int code = add(&newList, list->data, 0);
		if (code != 0) return NULL;
		list = list->next;
	}

	return newList;
}

void removeList(List ** list) {
	List * current = *list;
	while (current != NULL)	{
		List * temp = current;
		current = current->next;
		free(temp);
	}
	*list = NULL;
}

List * copy(List * list) {
	List * newList = NULL;

	while (list != NULL) {
		int code = addLast(&newList, list->data);
		if (code != 0) return NULL;
		list = list->next;
	}

	return newList;
}

List * find(List * list, data data) {
	while (list != NULL) {
		if (data == list->data) return list;
		list = list->next;
	}
	return NULL;
}

int count(List * list) {
	int n = 0;
	while (list != NULL) {
		n++;
		list = list->next;
	}
	return n;
}

void save(List * list, char * filename) {
	FILE * file;
	fopen_s(&file, filename, "w");
	if (file == NULL) return;

	while (list != NULL) {
		fprintf_s(file, "%d ", list->data);
		list = list->next;
	}

	fclose(file);
}

List * open(char * filename) {
	FILE * file;
	fopen_s(&file, filename, "r");
	if (file == NULL) return NULL;

	List * list = NULL;
	data data = 0;

	while (!feof(file)) {
		fscanf_s(file, "%d ", &data);
		addLast(&list, data);
	}

	fclose(file);
	return list;
}