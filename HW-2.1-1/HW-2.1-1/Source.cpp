#include <stdlib.h>
#include <stdio.h>
#include <locale.h>
#include "List.h"

int main() {
	setlocale(LC_ALL, "rus");
	List * list;
	List * list_copy;
	List * converted_list;
	List * el;
	data data[2];
	data[0] = 1;
	data[1] = 2;

	list = createList(data, 2); // Создание списка
	addLast(&list, 4); // Добавить в конец списка
	addLast(&list, 5); // Добавить в конец списка
	add(&list, 3, 2); // Добавить в произвольное место
	remove(&list, 3); // Удаление элемента
	list_copy = copy(list); // Копировать список
	converted_list = convert(list); // Перевернуть список
	removeList(&list); // Удалить список
	list = join(list_copy, converted_list); // Соединить два списка
	
	el = find(list, 5); // Найти элемент
	if (el != NULL)
		printf_s("5 is found\n");
	else
		printf_s("5 not found\n");

	el = find(list, 4); // Найти элемент
	if (el != NULL)
		printf_s("4 is found\n");
	else
		printf_s("4 not found\n");

	printf_s("Count: %d\n", count(list)); // Кол-во элементов
	save(list, "list.txt"); // Сохранить
	removeList(&list_copy);
	list_copy = open("list.txt"); // Открыть
	printf_s("Count: %d\n", count(list_copy)); // Кол-во элементов	

	system("pause");
	return 0;
}