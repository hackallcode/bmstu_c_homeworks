#include <stdio.h>
#include <stdlib.h>

int main() {
	double a = 0, b = 0, c = 0, x = 0, y = 0;
	printf("Equation has the form y=a*x^2+b*x+c. Input a, b and c:\n");
	scanf_s("%lf%lf%lf", &a, &b, &c);
	x = -b / (2. * a);
	y = a * x * x + b * x + c;
	printf("\nX of vertex = %.2f\nY of vertex = %.2f\n\n", x, y);
	system("pause");
}