#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main() {
	int n = 0, m = 0, a = 0, num = 0, p = 0;
	printf_s("Enter number of elements\nn > ");
	scanf_s("%d", &n);
	printf_s("m > ");
	scanf_s("%d", &m);
	double* b = (double*) malloc(sizeof(int) * n);
	printf_s("Enter elements:\n");
	for (int j = 0; j < n; j++) {
		p = 1;
		num = 0;
		for (int i = 0; i < m; i++) {
			printf_s("a[%d][%d] > ", j, i);
			scanf_s("%d", &a);
			p *= a;
			num++;
		};
		b[j] = pow(p, 1./num);
	};
	for (int j = 0; j < n; j++) {
		printf_s("b%d = %lf\n", j, b[j]);
	}
	system("pause");
}