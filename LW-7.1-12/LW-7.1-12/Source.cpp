#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEBUG
#define SUCCESS 0
#define NO_MEMORY_ERROR 1
#define INVALID_NUMBER_ERROR 2
#define NO_FILE_ERROR 3
#define NO_CARS_ERROR 4

struct Car {
	char * model;
	int fuel[3];
	int maxV;
	int power;
};

int e_noMemory() {
#ifdef DEBUG
	printf_s("Impossible reserve memory!\n");
#endif // DEBUG	
	return NO_MEMORY_ERROR;
}

int e_invalidNumber(int n = 0) {
#ifdef DEBUG
	printf_s("Invalid number! (%d)\n", n);
#endif // DEBUG	
	return INVALID_NUMBER_ERROR;
}

int e_noFile() {
#ifdef DEBUG
	printf_s("Impossible create or open file!\n");
#endif // DEBUG	
	return NO_FILE_ERROR;
}

int e_noCars() {
#ifdef DEBUG
	printf_s("Cars not initialized!\n");
#endif // DEBUG	
	return NO_CARS_ERROR;
}

int printCars(Car * cars, int number) {
	for (int i = 0; i < number; i++) {
		printf_s("Model: %s\nFuel consumption {\n   On the road: %d\n   In the city: %d\n   Mixed: %d\n}\nMax velocity: %d\nPower: %d\n\n", 
			cars[i].model, cars[i].fuel[0], cars[i].fuel[1], cars[i].fuel[2], cars[i].maxV, cars[i].power);
	}

	return SUCCESS;
}

int freeCars(Car * cars, int number) {
	for (int i = 0; i < number; i++) {
		if (cars[i].model != NULL)free(cars[i].model);
	};
	if (cars != NULL)free(cars);

	return SUCCESS;
}

int saveCars(Car * cars, int n, char * filename) {
	if (cars == NULL) return e_noCars();
	if (n < 1) return e_invalidNumber();

	FILE * file;
	fopen_s(&file, filename, "wb");
	if (file == NULL) return e_noFile();

	fwrite(&n, sizeof(int), 1, file);
	for (int i = 0; i < n; i++) {
		int k = strlen(cars[i].model) + 1;
		fwrite(&k, sizeof(int), 1, file);
		fwrite(cars[i].model, sizeof(char), k, file);
		fwrite(&cars[i].fuel, sizeof(int), 3, file);
		fwrite(&cars[i].maxV, sizeof(int), 1, file);
		fwrite(&cars[i].power, sizeof(int), 1, file);
	}

	fclose(file);
	return SUCCESS;
}

int openCars(Car ** cars, char * filename) {
	FILE * file;
	fopen_s(&file, filename, "r");
	if (file == NULL) return e_noFile();

	int n = 0;
	fread(&n, sizeof(int), 1, file);

	if (*cars != NULL) freeCars(*cars, n);

	*cars = (Car *)malloc(sizeof(Car) * n);
	if (*cars == NULL) return e_noMemory();

	for (int i = 0; i < n; i++) {
		int k = 0;
		fread(&k, sizeof(int), 1, file);
		(*cars)[i].model = (char *)malloc(k);
		if ((*cars)[i].model == NULL) {
			freeCars((*cars), n);
			return e_noMemory();
		}
		fread((*cars)[i].model, sizeof(char), k, file);
		fread(&(*cars)[i].fuel, sizeof(int), 3, file);
		fread(&(*cars)[i].maxV, sizeof(int), 1, file);
		fread(&(*cars)[i].power, sizeof(int), 1, file);
	}

	fclose(file);
	return SUCCESS;
}

int main() {
	Car * cars;
	char buf[256];
	int n = 0;

	printf_s("Enter number of cars:\n> ");
	scanf_s("%d", &n);

	if (n < 1) return e_invalidNumber(n);

	cars = (Car *)malloc(sizeof(Car) * n);
	if (cars == NULL) return e_noMemory();

	for (int i = 0; i < n; i++) {
		system("cls");
		_flushall();
		printf_s("Enter model of %d of %d cars:\n> ", i + 1, n);
		do gets_s(buf, 255); while (strcmp(buf, "") == 0);
		cars[i].model = (char *)malloc(strlen(buf) + 1);
		if (cars[i].model == NULL) {
			freeCars(cars, i);
			return e_noMemory();
		}
		strcpy_s(cars[i].model, strlen(buf) + 1, buf);
		printf_s("Enter fuel consumption of car (on the road, in the city, mixed):\n> ");
		scanf_s("%d%d%d", &cars[i].fuel[0], &cars[i].fuel[1], &cars[i].fuel[2]);
		printf_s("Enter max velocity of car:\n> ");
		scanf_s("%d", &cars[i].maxV);
		printf_s("Enter power of car:\n> ");
		scanf_s("%d", &cars[i].power);
	}
	system("cls");

	saveCars(cars, n, "cars");
	openCars(&cars, "cars");

	printCars(cars, n);
	freeCars(cars, n);
	system("pause");
	return SUCCESS;
}