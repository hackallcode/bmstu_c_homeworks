#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

int min(int ** mat, int n, int m) {
	int min = INT_MAX;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			if (mat[i][j] < min) min = mat[i][j];
		}
	}
	return min;
}

void freeMat(int *** mat, int n) {
	if (*mat == NULL) return; // Если память под массив указателей выделена

	for (int i = 0; i < n; i++) {
		if ((*mat)[i] != NULL) free((*mat)[i]); // Если память под массив чисел выделена
	}
	free(*mat);
	*mat = NULL;
}

bool initMat(int *** mat, int n, int m) {
	*mat = (int **)malloc(sizeof(int *) * n);
	if (*mat == NULL) return false;
	for (int i = 0; i < n; i++) {
		(*mat)[i] = (int *)malloc(sizeof(int) * m);
		if ((*mat)[i] == NULL) {
			freeMat(mat, n);
			return false;
		}
		for (int j = 0; j < m; j++) {
			(*mat)[i][j] = rand() % 10;
		}
	}
	return true;
}

void printMat(int ** mat, int n, int m) {
	for (int j = 0; j < m; j++) {
		for (int i = 0; i < n; i++) {
			printf_s("%d ", mat[i][j]);
		}
		printf_s("\n");
	}
}

int main() {
	setlocale(LC_ALL, "rus");

	int n1 = 0, m1 = 0;
	printf_s("Введите n1 и m1: ");
	scanf_s("%d%d", &n1, &m1);

	int ** mat1;
	if (!initMat(&mat1, n1, m1)) {
		printf_s("Невозможно выделить память!\n");
		system("pause");
	}	

	int n2 = 0, m2 = 0;
	printf_s("Введите n2 и m2: ");
	scanf_s("%d%d", &n2, &m2);
	
	int ** mat2;
	if (!initMat(&mat2, n2, m2)) {
		freeMat(&mat1, n1);
		printf_s("Невозможно выделить память!\n");
		system("pause");
	}

	int n3 = 0;
	int m3 = 0;
	printf_s("Введите n3 и m3: ");
	scanf_s("%d%d", &n3, &m3);
	
	int ** mat3;
	if (!initMat(&mat3, n3, m3)) {
		freeMat(&mat1, n1);
		freeMat(&mat2, n2);
		printf_s("Невозможно выделить память!\n");
		system("pause");
	}

	printf_s("\nПервая матрица:\n");
	printMat(mat1, n1, m1);
	
	printf_s("\nВторая матрица:\n");
	printMat(mat2, n2, m2);
	
	printf_s("\nТретья матрица:\n");
	printMat(mat3, n3, m3);

	int sum = min(mat1, n1, m1) + min(mat2, n2, m2) + min(mat3, n3, m3);
	printf_s("\nСумма минимальных элементов: %d\n", sum);

	system("pause");
	return 0;
}