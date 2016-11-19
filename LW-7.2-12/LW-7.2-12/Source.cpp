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

struct CarsList {
	Car car;
	CarsList * prev;
	CarsList * next;
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

int initCar(CarsList ** cars, char * model = "", int fuel[3] = {}, int maxV = 0, int power = 0) {
	*cars = (CarsList *)malloc(sizeof(CarsList));
	if (*cars == NULL) return e_noMemory();

	(*cars)->car.model = (char *)malloc(strlen(model) + 1);
	if ((*cars)->car.model == NULL) {
		free(*cars);
		*cars = NULL;
		return e_noMemory();
	}
	strcpy_s((*cars)->car.model, strlen(model) + 1, model);
	if (fuel == 0) {
		(*cars)->car.fuel[0] = 0;
		(*cars)->car.fuel[1] = 0;
		(*cars)->car.fuel[2] = 0;
	}
	else {
		(*cars)->car.fuel[0] = fuel[0];
		(*cars)->car.fuel[1] = fuel[1];
		(*cars)->car.fuel[2] = fuel[2];
	}
	
	(*cars)->car.maxV = maxV;
	(*cars)->car.power = power;
	(*cars)->prev = NULL;
	(*cars)->next = NULL;

	return SUCCESS;
}

int addCar(CarsList * cars, char * model, int fuel[3], int maxV, int power) {
	if (strcmp(cars->car.model, "") != 0) {
		while (cars->next != NULL) cars = cars->next;

		cars->next = (CarsList *)malloc(sizeof(CarsList));
		if (cars->next == NULL) return e_noMemory();

		cars->next->car.model = (char *)malloc(strlen(model) + 1);
		if (cars->next->car.model == NULL) {
			free(cars->next);
			cars->next = NULL;
			return e_noMemory();
		}
		strcpy_s(cars->next->car.model, strlen(model) + 1, model);
		cars->next->car.fuel[0] = fuel[0];
		cars->next->car.fuel[1] = fuel[1];
		cars->next->car.fuel[2] = fuel[2];
		cars->next->car.maxV = maxV;
		cars->next->car.power = power;
		cars->next->prev = cars;
		cars->next->next = NULL;
	}
	else {
		free(cars->car.model);
		cars->car.model = (char *)malloc(strlen(model) + 1);
		if (cars->car.model == NULL) return e_noMemory();
		strcpy_s(cars->car.model, strlen(model) + 1, model);
		cars->car.fuel[0] = fuel[0];
		cars->car.fuel[1] = fuel[1];
		cars->car.fuel[2] = fuel[2];
		cars->car.maxV = maxV;
		cars->car.power = power;
	}

	return SUCCESS;
}

int printCars(CarsList * cars) {
	while (cars != NULL) {
		printf_s("Model: %s\nFuel consumption {\n   On the road: %d\n   In the city: %d\n   Mixed: %d\n}\nMax velocity: %d\nPower: %d\n\n", 
			cars->car.model, cars->car.fuel[0], cars->car.fuel[1], cars->car.fuel[2], cars->car.maxV, cars->car.power);
		cars = cars->next;
	}

	return SUCCESS;
}

int freeCars(CarsList * cars) {
	CarsList * current;
	CarsList * next = cars;

	while (next != NULL) {
		current = next;
		next = current->next;
		free(current->car.model);
		free(current);
	};

	return SUCCESS;
}

int saveCars(CarsList * cars, char * filename) {
	if (cars == NULL) return e_noCars();

	FILE * file;
	fopen_s(&file, filename, "wb");
	if (file == NULL) return e_noFile();

	while (cars != NULL) {
		int k = strlen(cars->car.model) + 1;
		fwrite(&k, sizeof(int), 1, file);
		fwrite(cars->car.model, sizeof(char), k, file);
		fwrite(&cars->car.fuel, sizeof(int), 3, file);
		fwrite(&cars->car.maxV, sizeof(int), 1, file);
		fwrite(&cars->car.power, sizeof(int), 1, file);
		cars = cars->next;
	}

	fclose(file);
	return SUCCESS;
}

int openCars(CarsList ** cars, char * filename) {
	FILE * file;
	fopen_s(&file, filename, "r");
	if (file == NULL) return e_noFile();

	if (*cars != NULL) freeCars(*cars);

	int return_code = initCar(cars);
	if (return_code != 0) return return_code;

	while (true) {
		int k = 0;
		char buf[256] = "";
		int fuel[3] = { 0, 0, 0 };
		int maxV = 0;
		int power = 0;
		int return_code = 0;
		fread(&k, sizeof(int), 1, file);
		fread(buf, sizeof(char), k, file);
		fread(&fuel, sizeof(int), 3, file);
		fread(&maxV, sizeof(int), 1, file);
		fread(&power, sizeof(int), 1, file);
		return_code = addCar(*cars, buf, fuel, maxV, power);
		if (return_code != 0) return return_code;
		if (getc(file) == EOF) // End of file
			break;
		else
			fseek(file, -1, SEEK_CUR);
	}

	fclose(file);
	return SUCCESS;
}

int main() {
	CarsList * cars;
	char buf[256] = "";
	int n = 0;
	int fuel[3] = { 0, 0, 0 };
	int maxV = 0;
	int power = 0;
	int return_code = 0;

	return_code = initCar(&cars);
	if (return_code != 0) return return_code;
	
	printf_s("Enter number of cars:\n> ");
	scanf_s("%d", &n);

	if (n < 1) {
		freeCars(cars);
		return e_invalidNumber(n);
	}

	for (int i = 0; i < n; i++) {
		system("cls");
		_flushall();
		printf_s("Enter model of %d of %d cars:\n> ", i + 1, n);
		do gets_s(buf, 255); while (strcmp(buf, "") == 0);
		printf_s("Enter fuel consumption of car (on the road, in the city, mixed):\n> ");
		scanf_s("%d%d%d", &fuel[0], &fuel[1], &fuel[2]);
		printf_s("Enter max velocity of car:\n> ");
		scanf_s("%d", &maxV);
		printf_s("Enter power of car:\n> ");
		scanf_s("%d", &power);
		return_code = addCar(cars, buf, fuel, maxV, power);
		if (return_code != 0) return return_code;
	}
	system("cls");
	saveCars(cars, "cars");
	openCars(&cars, "cars");
	printCars(cars);
	freeCars(cars);
	system("pause");
	return SUCCESS;
}