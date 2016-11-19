#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

void rotate(int * vector, int k, int n) {
	if (k % n == 0) return;
	
	int
		current = 0,
		next = 0,
		l = _msize(vector) / sizeof(int),
		a = 0,
		first = 0;

	bool checked_all = true;
	bool * checked = (bool *)malloc(sizeof(bool) * n);
	for (int i = 0; i < n; i++) checked[i] = false;
	
	do {
		checked_all = true;
		a = vector[first];
		do {
			current = next;
			next = ((current - k) % l + l) % l;
			vector[current] = vector[next];
			checked[current] = true;
		} while (next != first);
		vector[current] = a;
		for (int i = 0; i < n; i++) {
			if (!checked[i]) {
				next = i;
				first = i;
				checked_all = false;
				break;
			}
		}		 
	} while (!checked_all);

}

int main() {
	int * vector;
	int
		k = 0,
		n = 0;

	printf_s("Enter length of vector:\n> ");
	scanf_s("%d", &n);	
	printf_s("Enter k:\n> ");
	scanf_s("%d", &k);

	vector = (int *)malloc(sizeof(int) * n);
	if (vector == NULL) {
		printf_s("Impossible reserve memory!");
		system("pause");
		return 0;
	}
	printf_s("\nInput vector:\n");
	for (int i = 0; i < n; i++) {
		vector[i] = rand() % 10;
		printf_s("%d ", vector[i]);
	}

	rotate(vector, k, n);

	printf_s("\n\nChanged vector:\n");
	for (int i = 0; i < n; i++) {
		printf_s("%d ", vector[i]);
	}
	printf_s("\n\n");
	free(vector);
	system("pause");
	return 0;
}