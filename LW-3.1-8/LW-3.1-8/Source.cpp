#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

int main() {
	setlocale(LC_ALL, "rus");
	int * a;
	int
		n = 0,
		max = -1,
		p = 1;

	printf_s("Введите n:\n> ");
	scanf_s("%d", &n);
	a = (int *)malloc(sizeof(int) * n);
	if (a == NULL) {
		printf_s("Невозможно выделить память!");
		system("pause");
		return 0;
	}

	printf_s("Входной массив:\n");
	for (int i = 0; i < n; i++) {
		a[i] = rand() % 10;
		printf_s("%d ", a[i]);
	}

	for (int i = 0; i < n; i++) {
		p *= a[i];
		if (p > max) max = p;
	}

	printf_s("\nМаксимальное число: %d\n", max);

	free(a);
	system("pause");
	return 0;
}