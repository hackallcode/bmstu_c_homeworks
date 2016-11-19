#include <stdio.h>
#include <stdlib.h>

int main() {
	bool result = false;
	double a = 0, b = 0, c = 0, r = 0, s = 0, min = 0, mid = 0;
	printf("Sides of brick is a, b anc c. Sides of hole is r and s. Input a, b, c, r and s:\n");
	scanf_s("%lf%lf%lf%lf%lf", &a, &b, &c, &r, &s);
	if (a <= b && a <= c) {
		min = a;
		mid = (b <= c) ? b : c;
	}
	else if (b <= a && b <= c) {
		min = b;
		mid = (a <= c) ? a : c;
	}
	else {
		min = c;
		mid = (a <= b) ? a : b;
	}
	((min <= r && mid <= s) || (min <= s && mid <= r)) ?
		printf("\nBrick will go through the hole.\n\n") :
		printf("\nBrick will NOT go through the hole!\n\n");
	system("pause");
}