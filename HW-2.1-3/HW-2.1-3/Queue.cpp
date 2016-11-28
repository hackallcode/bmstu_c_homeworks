#include <stdlib.h>
#include <stdio.h>
#include "Queue.h"

int createQueue(Queue ** newQueue, data * data, int dataNumber) {
	if (*newQueue != NULL) deleteQueue(newQueue);
	
	Queue * current = NULL;
	for (int i = 0; i < dataNumber; i++) {
		if (i == 0) {
			*newQueue = (Queue *)malloc(sizeof(Queue));
			if (*newQueue == NULL) return -1;
			(*newQueue)->data = data[i];
			(*newQueue)->next = NULL;
			current = *newQueue;
		}
		else {
			Queue * temp = (Queue *)malloc(sizeof(Queue));
			if (temp == NULL) return -1;
			temp->data = data[i];
			temp->next = NULL;
			current->next = temp;
			current = temp;
		}
	}
	return 0;
}

int addLastData(Queue ** queue, data data) {
	if (*queue == NULL) {
		*queue = (Queue *)malloc(sizeof(Queue));
		if (*queue == NULL) return -1;
		(*queue)->data = data;
		(*queue)->next = NULL;
	}
	else {
		Queue * prev = NULL;
		Queue * current = *queue;
		while (current != NULL) {
			prev = current;
			current = current->next;
		}
		current = (Queue *)malloc(sizeof(Queue));
		if (current == NULL) return -1;
		current->data = data;
		current->next = NULL;
		if (prev != NULL) prev->next = current;
	}
	return 0;
}

int getFirstData(Queue * queue, data * destData) {
	if (queue == NULL) return -1;
	*destData = queue->data;
	return 0;
}

int getLastData(Queue * queue, data * destData) {
	if (queue == NULL) return -1;
	while (queue->next != NULL)
		queue = queue->next;
	*destData = queue->data;
	return 0;
}

int deleteFirstData(Queue ** queue) {
	if (*queue == NULL) return 0;
	Queue * temp = *queue;
	*queue = (*queue)->next;
	free(temp);
	return 0;
}

int deleteQueue(Queue ** queue) {
	Queue * current = *queue;
	while (current != NULL) {
		Queue * temp = current;
		current = current->next;
		free(temp);
	}
	*queue = NULL;
	return 0;
}

int copyQueue(Queue ** newQueue, Queue * sourceQueue) {
	if (*newQueue != NULL) deleteQueue(newQueue);
	Queue * current = sourceQueue;
	while (current != NULL) {
		int code = addLastData(newQueue, current->data);
		if (code != 0) return code;
		current = current->next;
	}
	return 0;
}

Queue * findData(Queue * queue, data data) {
	while (queue != NULL) {
		if (queue->data == data) return queue;
		queue = queue->next;
	}
	return NULL;
}

int countData(Queue * queue) {
	int n = 0;
	while (queue != NULL) {
		n++;
		queue = queue->next;
	}
	return n;
}

int saveQueue(Queue * queue, char * filename) {
	FILE * file;
	fopen_s(&file, filename, "w");
	if (file == NULL) return -1;

	while (queue != NULL) {
		fprintf_s(file, "%d ", queue->data);
		queue = queue->next;
	}

	fclose(file);
	return 0;
}

int openQueue(Queue ** queue, char * filename) {
	if (*queue != NULL) return deleteQueue(queue);

	FILE * file;
	fopen_s(&file, filename, "r");
	if (file == NULL) return -1;

	data data;
	while (!feof(file)) {
		fscanf_s(file, "%d", &data);
		addLastData(queue, data);
	}

	fclose(file);
	return 0;
}