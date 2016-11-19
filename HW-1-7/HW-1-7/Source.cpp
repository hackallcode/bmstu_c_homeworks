#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool isPalindrome(char * string) {
	for (int i = 0; i < strlen(string) / 2; i++) {
		if (string[i] != string[strlen(string) - i - 1]) return false;
	}
	return true;
}

int main() {
	char str[128];
	printf_s("To exit enter \"0\" without quotes.\n\nEnter string to check:\n> ");
	while (true) {
		gets_s(str, 128);
		if (strcmp(str, "0") == 0) break;
		if (isPalindrome(str)) {
			system("cls");
			printf_s("To exit enter \"0\" without quotes.\n\nVerifiable string: \"%s\"\nIt's palindrome!\n\nEnter next string to check:\n> ", str);
		}
		else {
			system("cls");
			printf_s("To exit enter \"0\" without quotes.\n\nVerifiable string: \"%s\"\nIt's NOT palindrome!\n\nEnter next string to check:\n> ", str);
		}
	}
	return 0;
}