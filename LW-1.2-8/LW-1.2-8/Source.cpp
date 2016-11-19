#include <stdio.h>
#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <math.h>

int main()
{
	double a = 0, r = 0;
	printf_s("Enter length of square side:\n");
	scanf_s("%lf", &a);
	printf_s("Enter length of circle radius:\n");
	scanf_s("%lf", &r);
	double circle_area = 0, square_area = 0;
	circle_area = r*r*M_PI;
	square_area = a*a;
	if (circle_area > square_area)
		printf_s("Area of CIRCLE is bigger than area of square and is equal to %.3lf\n", circle_area);
	else if (square_area > circle_area)
		printf_s("Area of SQUARE is bigger than area of circle and is equal to %.3lf\n", square_area);
	else
		printf_s("Areas the same and equal to %.3lf\n", circle_area);
	system("pause");
	return 0;
}