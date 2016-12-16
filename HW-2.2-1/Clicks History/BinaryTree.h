#pragma once

/* Includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/**/

/* Defines */
#define DEBUG
#define SUCCESS 0
#define TREE_EXIST_ERROR 1
#define NO_MEMORY_ERROR 2
#define NO_TREE_ERROR 3
#define NOT_FOUND 4
#define NO_PARENT_ERROR 5
#define NO_FILE_ERROR 6
/**/

/* User Structes */
struct ME_data {
	int id;
	char type;
	int x;
	int y;
};
/**/

/* TypeDefs */
typedef int KEY; // MUST NOT BE POINTER
typedef ME_data * DataArray;
/**/

/* Structes */
struct DATA {
	int dataNumber;
	DataArray data;
};
struct TREE {
	KEY key;
	DATA data;
	TREE * left;
	TREE * right;
};
/**/

/* Tree */
int createTree(TREE ** newTree, KEY * key, DATA * data);
int addChildTree(TREE ** rootTree, KEY key, DATA * data);
int removeChildTree(TREE ** rootTree, KEY key);
int removeTree(TREE ** rootTree);
int copyTree(TREE ** destTree, TREE * sourceTree);
int findChildTree(TREE * rootTree, KEY key, TREE ** destTree = NULL);
int countChildTrees(TREE * rootTree);
int findCommonParent(TREE * rootTree, KEY key_1, KEY key_2, TREE ** destTree = NULL);
int saveTree(TREE * rootTree, TCHAR * filename);
int openTree(TREE ** rootTree, TCHAR * filename);
/**/