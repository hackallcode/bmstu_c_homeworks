#pragma once

typedef int data;

struct List {
	data data;
	List * next;
};

List * createList(data * data, int data_number); // Создание списка
int addLast(List ** list, data data); // Добавить в конец списка
int add(List ** list, data data, int number); // Добавить в произвольное место
void remove(List ** list, int number); // Удаление элемента
List * join(List * a, List * b); // Соединить два списка
List * convert(List * list); // Перевернуть список
void removeList(List ** list); // Удалить список
List * copy(List * list); // Копировать список
List * find(List * list, data data); // Найти элемент
int count(List * list); // Кол-во элементов
void save(List * list, char * filename); // Сохранить
List * open(char * filename); // Открыть