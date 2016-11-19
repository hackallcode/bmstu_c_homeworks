#include <stdlib.h>
#include <stdio.h>

int main() {
	int B[5][5];
	int C[20];

	printf("Входной массив:\n");
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			B[i][j] = rand() % 10;
			printf("%d ", B[i][j]);
		}
		printf("\n");
	}

	int k = 0;
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			if (i != j) {
				C[k] = B[i][j];
				k++;
			}
		}
	}

	printf("\nПолученный вектор:\n");
	for (int i = 0; i < 20; i++) {
		printf("%d ", C[i]);
	}

	system("pause");
	return 0;
}