#include <stdio.h>
#include <stdlib.h>

int main() {
	int n = 0, s = 0;
	printf_s("Enter number of elements\n> ");
	scanf_s("%d", &n);
	int* a = (int*)malloc(sizeof(int) * n);
	printf_s("Enter elements:\n");
	for (int i = 0; i < n; i++) {
		printf_s("%d > ", i);
		scanf_s("%d", &a[i]);
	};
	for (int i = 0; i < n; i++) {
		s += a[i];
	}
	printf_s("Summary is %d\n", s);
	free(a);
	system("pause");
}