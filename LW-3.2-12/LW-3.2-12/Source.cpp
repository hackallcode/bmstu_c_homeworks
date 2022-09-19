#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main() {
	int n = 0, m = 0, num = 0, p = 0;
	printf_s("Enter number of elements\nn > ");
	scanf_s("%d", &n);
	printf_s("m > ");
	scanf_s("%d", &m);
	int ** a = (int **)malloc(sizeof(int *) * m);
	for (int i = 0; i < m; i++) {
		a[i] = (int *)malloc(sizeof(int) * n);
	}
	double* b = (double*) malloc(sizeof(double) * m);
	printf_s("Enter elements:\n");
	for (int j = 0; j < m; j++) {
		for (int i = 0; i < n; i++) {
			printf_s("a[%d][%d] > ", j, i);
			scanf_s("%d", &a[j][i]);
		}
	};
	for (int j = 0; j < m; j++) {
		p = 1;
		num = 0;
		for (int i = 0; i < n; i++) {
			p *= a[j][i];
			num++;
		};
		b[j] = pow(p, 1. / num);
	}
	for (int j = 0; j < m; j++) {
		printf_s("b[%d] = %lf\n", j, b[j]);
	}
	free(b);
	for (int i = 0; i < m; i++) {
		free(a[i]);
	}
	free(a);
	system("pause");
}