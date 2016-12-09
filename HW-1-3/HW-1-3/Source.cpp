#include <stdio.h>
#include <stdlib.h>
#include <math.h>

char * itoa_2(unsigned int value, char * str) {
	if (value == 0) {
		str[0] = '0';
		str[1] = '\0';
		return str;
	}

	bool notNull = false;
	int k = 0;
	for (int i = 0; i < sizeof(int) * 8; i++) {
		if ((value & (int)pow(2, sizeof(int) * 8 - 1 - i)) == 0) {
			if (notNull) {
				str[k] = '0';
				k++;
			}
		}
		else {
			notNull = true;
			str[k] = '1';
			k++;
		}
	}
	str[k] = '\0';
	return str;
}

int main() {
	char * str = (char *)malloc(33);
	
	str = itoa_2(18, str);
	printf_s("%s\n", str);

	free(str);
	system("pause");
	return 0;
}