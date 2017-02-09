#include <stdio.h>
#include <stdlib.h>

void printVector(double ** vector, int m) {
	printf_s("Result Vector:\n");
	for (int j = 0; j < m; j++) {
		printf_s("%.2lf\n", *(vector[j]));
	}
}

void printMatrix(double ** matrix, int n, int m) {
	printf_s("Original Matrix:\n");
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			printf_s("%.2lf ", matrix[i][j]);
		}
		printf_s("\n");
	}
}

void freeMatrix(double ** matrix, int n, int m) {
	for (int i = 0; i < m; i++) {
		free(matrix[i]);
	}
	free(matrix);
}

void freeVector(double ** vector, int m) {
	for (int i = 0; i < m; i++) {
		free(vector[i]);
	}
	free(vector);
}

double avr(double * vector, int n) {
	double res = 0;
	for (int i = 0; i < n; i++)
		res += vector[i];
	res /= n;
	return res;
}

void * avr_d(void ** vector, int n) {
	double * res_d;
	if ((res_d = (double *)malloc(sizeof(double) * n)) == NULL) {
		printf_s("Impossible allocate memory!");
		return NULL;
	}
	*res_d = avr((double *)vector, n);
	return res_d;
}

void * avr_i(void ** vector, int n) {
	int * res_d;
	if ((res_d = (int *)malloc(sizeof(int) * n)) == NULL) {
		printf_s("Impossible allocate memory!");
		return NULL;
	}
	for (int i = 0; i < n; i++)
		*res_d += ((int *)vector)[i];
	*res_d /= n;
	return res_d;
}

void ** Transform(void *** matrix, int n, int m, int typeSize, void * (*proc)(void **, int)) {
	void ** result = (void **)malloc(typeSize * m);
	for (int i = 0; i < m; i++) {
		result[i] = proc(matrix[i], n);
	}
	return result;
}

int main() {
	int n = 0, m = 0;
	double ** matrix = NULL;

	printf_s("Enter number of columns:\n> ");
	scanf_s("%d", &n);
	while (n < 1) {
		printf_s("Enter CORRECT number of columns:\n> ");
		scanf_s("%d", &n);
	}

	printf_s("Enter number of rows:\n> ");
	scanf_s("%d", &m);
	while (m < 1) {
		printf_s("Enter CORRECT number of rows:\n> ");
		scanf_s("%d", &m);
	}

	if ((matrix = (double **)malloc(m * sizeof(double *))) == NULL) {
		printf_s("Impossible allocate memory!");
		system("pause");
		return 0;
	}
	for (int i = 0; i < m; i++) {
		if ((matrix[i] = (double *)malloc(n * sizeof(double))) == NULL) {
			printf_s("Impossible allocate memory!");
			system("pause");
			return 0;
		}
	}

	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			matrix[i][j] = rand() % 10;
		}
	}

	system("cls");
	printMatrix(matrix, n, m);

	double ** vector = (double **)Transform((void ***)matrix, n, m, sizeof(double), avr_d);

	printf_s("\n");
	printVector(vector, m);

	freeMatrix(matrix, n, m);
	freeVector(vector, m);

	system("pause");
	return 0;
}