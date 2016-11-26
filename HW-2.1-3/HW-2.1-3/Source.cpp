#include <stdlib.h>
#include <stdio.h>
#include "Queue.h"

int main() {
	Queue * queue = NULL;
	Queue * queue_copy = NULL;
	data data[2];
	data[0] = 1;
	data[1] = 2;

	createQueue(&queue, data, 2);
	addLastData(&queue, 3);
	addLastData(&queue, 4);
	printf_s("First: %d\n", getFirstData(queue));
	printf_s("Last: %d\n", getLastData(queue));
	deleteFirstData(&queue);
	copyQueue(&queue_copy, queue);
	deleteQueue(&queue);
	
	if (findData(queue_copy, 3) != NULL)
		printf_s("3 is found.\n");
	else
		printf_s("3 not found!\n");

	if (findData(queue_copy, 1) != NULL)
		printf_s("1 if found.\n");
	else
		printf_s("1 not found!\n");

	printf_s("Count: %d\n", countData(queue_copy));
	saveQueue(queue_copy, "queue.txt");
	openQueue(&queue, "queue.txt");
	printf_s("First: %d\n", getFirstData(queue));

	system("pause");
	return 0;
}