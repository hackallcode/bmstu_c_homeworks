#include <stdio.h>
#include <stdlib.h>

int main()
{
	double fuel = 0;
	printf_s("Enter the quantity of fuel:\n");
	scanf_s("%lf", &fuel);
	double economy = 0, middle_value = 0;
	middle_value = fuel*0.5 - fuel*0.5*0.3;
	economy = middle_value - middle_value*0.2;
	if (economy != 0)
		printf_s("Economy is %.3lf\n", economy);
	else
		printf_s("You create the infinity fuel!\n");
	system("pause");
	return 0;
}