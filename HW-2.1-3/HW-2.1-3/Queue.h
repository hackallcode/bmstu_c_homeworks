#pragma once

#define DEBUG

typedef int data;

struct Queue {
	data data;
	Queue * next;
};

int createQueue(Queue ** newQueue, data * data, int dataNumber);
int addLastData(Queue ** queue, data data);
int getFirstData(Queue * queue);
int getLastData(Queue * queue);
int deleteFirstData(Queue ** queue);
int deleteQueue(Queue ** queue);
int copyQueue(Queue ** destQueue, Queue * sourceQueue);
Queue * findData(Queue * queue, data data);
int countData(Queue * queue);
int saveQueue(Queue * queue, char * filename);
int openQueue(Queue ** queue, char * filename);