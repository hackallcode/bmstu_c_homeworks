#include <stdio.h>
#include <stdlib.h>

bool isSimple(int number) {
	if (number < 2) return false;
	for (int i = 2; i < number; i++) {
		if (number % i == 0) return false;
	}
	return true;
}

int main() {
	int
		m = 0,
		n = 0,
		i = 1;
	printf_s("Enter m:\n> ");
	scanf_s("%d", &m);
	
	while (n < m) {
		i++;
		if (isSimple(i)) {
			printf_s("%d ", i);
			n++;
		}		
	}

	printf_s("\n");
	system("pause");
	return 0;
}