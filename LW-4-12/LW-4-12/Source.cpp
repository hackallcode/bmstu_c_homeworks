#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

struct Subject {
	char name[64];
	int mark;
};

struct Student {
	char name[128];
	int subjects_count;
	Subject * subjects;
};

void free_students(Student * students, int students_number) {
	for (int i = 0; i < students_number; i++) {
		if (students[i].subjects) free(students[i].subjects);
	}
	if (students) free(students);
}

int no_memory() {
	printf_s("Impossible reserve memory!\n");
	system("pause");
	return 0;
}

int main() {
	int n = 0;
	printf_s("Enter number of students:\n> ");
	while (true) {
		scanf_s("%d", &n);
		if (n < 1)
			printf_s("Incorrect number! Try again.\n> ");
		else
			break;
	}

	Student * students = (Student *)malloc(sizeof(Student) * n);
	if (students == NULL) return no_memory();

	for (int i = 0; i < n; i++) {
		system("cls");
		printf_s("Enter last, first and middle name of the %d of %d students:\n> ", i + 1, n);
		_flushall();
		gets_s(students[i].name, 127);

		printf_s("Enter number of subjects:\n> ");
		while (true) {
			scanf_s("%d", &students[i].subjects_count);
			if (students[i].subjects_count < 1)
				printf_s("Incorrect number! Try again.\n> ");
			else
				break;
		}

		students[i].subjects = (Subject *)malloc(sizeof(Subject) * students[i].subjects_count);
		if (students[i].subjects == NULL) {
			free_students(students, n);
			return no_memory();
		}

		for (int j = 0; j < students[i].subjects_count; j++) {
			system("cls");
			printf_s("Enter name of the %d of %d subjects of %s:\n> ", j + 1, students[i].subjects_count, students[i].name);
			fflush(stdin);
			gets_s(students[i].subjects[j].name, 63);
			printf_s("Enter mark:\n> ");
			scanf_s("%d", &students[i].subjects[j].mark);
		}
	}

	system("cls");
	printf_s("Average marks:\n");
	for (int i = 0; i < n; i++) {
		double avr = 0;
		for (int j = 0; j < students[i].subjects_count; j++)
			avr += students[i].subjects[j].mark;
		avr /= students[i].subjects_count;
		printf_s("%s - %.2lf\n", students[i].name, avr);
	}

	free_students(students, n);

	system("pause");
	return 0;
}