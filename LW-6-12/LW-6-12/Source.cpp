#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEBUG
#define SUCCESS 0
#define NO_MEMORY_ERROR 1
#define INVALID_NUMBER_ERROR 2

enum Currency {
	Ruble = 1, Dollar, Euro
};

struct Deposit {
	char * name;
	int sum;
	Currency currency;
	int percent;
};

struct DepositsList {
	Deposit deposit;
	DepositsList * prev;
	DepositsList * next;
};

int e_noMemory() {
#ifdef DEBUG
	printf_s("Impossible reserve memory!\n");
#endif // DEBUG	
	return NO_MEMORY_ERROR;
}

int e_invalidNumber(int n = 0) {
#ifdef DEBUG
	printf_s("Invalid number! (%d)\n", n);
#endif // DEBUG	
	return INVALID_NUMBER_ERROR;
}

int initDeposit(DepositsList ** deposits, char * name = "", int sum = 0, Currency currency = Ruble, int percent = 0) {
	*deposits = (DepositsList *)malloc(sizeof(DepositsList));
	if (*deposits == NULL) return e_noMemory();

	(*deposits)->deposit.name = (char *)malloc(strlen(name) + 1);
	if ((*deposits)->deposit.name == NULL) {
		free(*deposits);
		*deposits = NULL;
		return e_noMemory();
	}
	strcpy_s((*deposits)->deposit.name, strlen(name) + 1, name);
	(*deposits)->deposit.sum = sum;
	(*deposits)->deposit.currency = currency;
	(*deposits)->deposit.percent = percent;
	(*deposits)->prev = NULL;
	(*deposits)->next = NULL;

	return SUCCESS;
}

int addDeposit(DepositsList * deposits, char * name, int sum, Currency currency, int percent) {
	if (strcmp(deposits->deposit.name, "") != 0) {
		while (deposits->next != NULL) deposits = deposits->next;

		deposits->next = (DepositsList *)malloc(sizeof(DepositsList));
		if (deposits->next == NULL) return e_noMemory();

		deposits->next->deposit.name = (char *)malloc(strlen(name) + 1);
		if (deposits->next->deposit.name == NULL) {
			free(deposits->next);
			deposits->next = NULL;
			return e_noMemory();
		}
		strcpy_s(deposits->next->deposit.name, strlen(name) + 1, name);
		deposits->next->deposit.sum = sum;
		deposits->next->deposit.currency = currency;
		deposits->next->deposit.percent = percent;
		deposits->next->prev = deposits;
		deposits->next->next = NULL;
	}
	else {
		free(deposits->deposit.name);
		deposits->deposit.name = (char *)malloc(strlen(name) + 1);
		if (deposits->deposit.name == NULL) return e_noMemory();
		strcpy_s(deposits->deposit.name, strlen(name) + 1, name);
		deposits->deposit.sum = sum;
		deposits->deposit.currency = currency;
		deposits->deposit.percent = percent;
	}

	return SUCCESS;
}

int countDeposits(DepositsList * deposits) {
	int n = 0;
	while (deposits != NULL) {
		n++;
		deposits = deposits->next;
	}
	return n;
}

int sortTwoDepositsByName(DepositsList ** deposits, int num_1, int num_2) {
	if (num_1 == num_2) return SUCCESS;
	if (num_1 > num_2) {
		int t = num_1;
		num_1 = num_2;
		num_2 = t;
	}

	DepositsList * d1 = *deposits;
	for (int i = 0; i < num_1; i++) {
		d1 = d1->next;
		if (d1 == NULL) return e_invalidNumber(num_1);
	}
	DepositsList * d2 = *deposits;
	for (int i = 0; i < num_2; i++) {
		d2 = d2->next;
		if (d2 == NULL) return e_invalidNumber(num_2);
	}

	if (strcmp(d1->deposit.name, d2->deposit.name) > 0) {
		if (num_1 + 1 == num_2) {
			DepositsList * t;
			t = d1->prev;
			d1->prev = d1->next;
			d1->next = d2->next;
			d2->next = d2->prev;
			d2->prev = t;

			if (d1->next != NULL) d1->next->prev = d1;
			if (d2->prev != NULL) d2->prev->next = d2;
		}
		else {
			DepositsList * t;
			t = d1->prev;
			d1->prev = d2->prev;
			d2->prev = t;
			t = d1->next;
			d1->next = d2->next;
			d2->next = t;

			d1->prev->next = d1;
			if (d1->next != NULL) d1->next->prev = d1;
			if (d2->prev != NULL) d2->prev->next = d2;
			d2->next->prev = d2;
		}
		if (num_1 == 0) *deposits = d2;
	}

	return SUCCESS;
}

int sortDeposisByName(DepositsList ** deposits) {
	int n = countDeposits(*deposits);

	for (int i = 0; i < n - 1; i++) {
		for (int j = 0; j < n - i - 1; j++) {
			sortTwoDepositsByName(deposits, j, j + 1);
		}
	}

	return SUCCESS;
}

int printDeposits(DepositsList * deposits) {
	while (deposits != NULL) {
		char * currency;
		switch (deposits->deposit.currency) {
		case(1):
			currency = "Ruble";
			break;
		case(2):
			currency = "Dollar";
			break;
		case(3):
			currency = "Euro";
			break;
		default:
			currency = "Unknown";
			break;
		}
		printf_s("Name: %s, sum: %d, currency: %s, percent: %d\n", deposits->deposit.name,
			deposits->deposit.sum, currency, deposits->deposit.percent);
		deposits = deposits->next;
	}

	return SUCCESS;
}

int freeDeposits(DepositsList * deposits) {
	DepositsList * current;
	DepositsList * next = deposits;

	while (next != NULL) {
		current = next;
		next = current->next;
		free(current->deposit.name);
		free(current);
	};

	return SUCCESS;
}

int main() {
	DepositsList * deposits;
	char buf[256];
	int n = 0;
	int sum = 0;
	Currency currency;
	int percent = 0;
	int return_code = 0;

	return_code = initDeposit(&deposits);
	if (return_code != SUCCESS) return return_code;

	printf_s("Enter number of deposits:\n> ");
	scanf_s("%d", &n);

	if (n < 1) {
		freeDeposits(deposits);
		return e_invalidNumber();
	}

	for (int i = 0; i < n; i++) {
		system("cls");
		_flushall();
		printf_s("Enter name of %d of %d deposits:\n> ", i + 1, n);
		gets_s(buf, 255);
		printf_s("Enter sum of deposit:\n> ");
		scanf_s("%d", &sum);
		printf_s("Enter currency of deposit (1 - Ruble, 2 - Dollar, 3 - Euro):\n> ");
		scanf_s("%d", &currency);
		printf_s("Enter percent of deposit:\n> ");
		scanf_s("%d", &percent);
		return_code = addDeposit(deposits, buf, sum, currency, percent);
		if (return_code != SUCCESS) return return_code;
	}

	sortDeposisByName(&deposits);

	system("cls");
	printf_s("Sorted deposits:\n");
	printDeposits(deposits);
	freeDeposits(deposits);
	system("pause");
	return SUCCESS;
}