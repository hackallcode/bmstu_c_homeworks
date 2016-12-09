#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <locale.h>

enum Currency {
	Ruble = 1, Dollar, Euro
};

struct Deposit {
	char name[128];
	int sum;
	Currency currency;
	int percent;
};

struct DepositsList {
	Deposit deposit;
	DepositsList * prev;
	DepositsList * next;
};

int initDeposit(DepositsList ** deposits, char * name, int sum, Currency currency, int percent, DepositsList * prev, DepositsList * next) {
	*deposits = (DepositsList *)malloc(sizeof(DepositsList));
	if (*deposits == NULL) return -1;

	strcpy_s((*deposits)->deposit.name, strlen(name) + 1, name);
	(*deposits)->deposit.sum = sum;
	(*deposits)->deposit.currency = currency;
	(*deposits)->deposit.percent = percent;
	(*deposits)->prev = prev;
	(*deposits)->next = next;
	return 0;
}

int addDeposit(DepositsList ** deposits, char * name, int sum, Currency currency, int percent) {
	if (*deposits == NULL) {
		initDeposit(deposits, name, sum, currency, percent, NULL, NULL);
	}
	else {
		DepositsList * current = *deposits;
		while (current->next != NULL) current = current->next;

		if (initDeposit(&current->next, name, sum, currency, percent, current, NULL) != 0)
			return -1;
	}

	return 0;
}

int countDeposits(DepositsList * deposits) {
	int n = 0;
	while (deposits != NULL) {
		n++;
		deposits = deposits->next;
	}
	return n;
}

int insertDeposit(DepositsList ** deposits, int num_1, int num_2) {
	if (num_1 == num_2) return 0;
	if (num_1 > num_2) {
		int t = num_1;
		num_1 = num_2;
		num_2 = t;
	}

	DepositsList * d1 = *deposits;
	for (int i = 0; i < num_1; i++) {
		d1 = d1->next;
	}
	DepositsList * d2 = d1;
	for (int i = 0; i < num_2 - num_1; i++) {
		d2 = d2->next;
	}

	if (d1->deposit.sum < d2->deposit.sum) return 0;

	if (num_1 + 1 == num_2) {
		d2->prev = d1->prev;
		d1->prev = d2;
		if (d2->prev != NULL) d2->prev->next = d2; 
		
		d1->next = d2->next;
		d2->next = d1;
		if (d1->next != NULL) d1->next->prev = d1;
	}
	else {
		d2->prev->next = d2->next;
		if (d2->next != NULL) d2->next->prev = d2->prev;

		d2->prev = d1->prev;
		if (d2->prev != NULL) d2->prev->next = d2;
		d2->next = d1;
		d1->prev = d2;
	}
	if (num_1 == 0) *deposits = d2;
}

int sortDeposits(DepositsList ** deposits) {
	int n = countDeposits(*deposits);
	DepositsList * first = *deposits;
	for (int i = 0; i < n; i++) {
		DepositsList * current = first;
		int min = 0;
		int minSum = first->deposit.sum;

		for (int j = 0; j < n - i; j++) {
			if (current->deposit.sum < minSum) {
				min = j;
				minSum = current->deposit.sum;
			}
			current = current->next;
		}
		insertDeposit(&first, 0, min);

		if (i == 0) *deposits = first;
		first = first->next;
	}
	return 0;
}

int printDeposits(DepositsList * deposits) {
	while (deposits != NULL) {
		printf_s("Название: %s, сумма: %d, тип валюты: %d, ставка: %d%% годовых\n", deposits->deposit.name, 
			deposits->deposit.sum, deposits->deposit.currency, deposits->deposit.percent);
		deposits = deposits->next;
	}
	return 0;
}

int freeDeposits(DepositsList ** deposits) {
	DepositsList * current = *deposits;
	DepositsList * next = NULL;

	while (current != NULL) {
		next = current->next;
		free(current);
		current = next;
	};

	*deposits = NULL;

	return 0;
}

int main() {
	setlocale(LC_ALL, "rus");

	DepositsList * deposits = NULL;
	int n = 0;
	char name[128];
	int sum = 0;
	Currency currency;
	int percent = 0;
	
	printf_s("Введите число вкладов:\n> ");
	scanf_s("%d", &n);

	while (n < 1) {
		system("cls");
		printf_s("Введите корректное число вкладов:\n> ");
		scanf_s("%d", &n);
	}

	for (int i = 0; i < n; i++) {
		system("cls");
		printf_s("Введите название %d-го вклада:\n> ", i + 1);
		_flushall();
		do gets_s(name, 127); while (strcmp(name, "") == 0);
		printf_s("Введите сумму:\n> ");
		scanf_s("%d", &sum);
		printf_s("Введите тип валюты (1 - Рубль, 2 - Доллар, 3 - Евро):\n> ");
		scanf_s("%d", &currency);
		printf_s("Введите ставку в процентах годовых:\n> ");
		scanf_s("%d", &percent);
		if (addDeposit(&deposits, name, sum, currency, percent) != 0)
			return -1;
	}

	system("cls");

	printf_s("Исходный список вкладов:\n");
	printDeposits(deposits);

	sortDeposits(&deposits);

	printf_s("Отсортированный список вкладов:\n");
	printDeposits(deposits);

	freeDeposits(&deposits);
	system("pause");
	return 0;
}