#include <stdio.h>
#include <stdlib.h>	
#include <math.h>


int main() {
	double x = 0, y = 0;
	int n = 0;
	printf_s("Enter X:\n");
	scanf_s("%lf", &x);
	double r = sqrt(x);
	for (int p = -2; p >= -6; p--) {
		n = 0;
		y = x;
		while (fabs(r - y) > pow(10., p)) {
			y = 0.5 * (y + x / y);
			n++;
		}
		printf_s("Accuracy of epsilon is %d. Difference is %.20lf. Number of iterations is %d\n", -p, fabs(r - y), n);
	}
	system("pause");
}