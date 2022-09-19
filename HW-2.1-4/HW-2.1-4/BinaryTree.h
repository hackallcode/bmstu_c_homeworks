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

/* TypeDefs */
typedef int key; // MUST NOT BE POINTER
typedef int data;
/**/

/* Structes */
struct Data {
	key key;
	data data;
};
struct Tree {
	key key;
	data data;
	Tree * left;
	Tree * right;
};
/**/

/* Tree */
int createTree(Tree ** newTree, Data * data, int dataNumber);
int addChildTree(Tree ** rootTree, key key, data data = NULL);
int removeChildTree(Tree ** rootTree, key key);
int removeTree(Tree ** rootTree);
int copyTree(Tree ** destTree, Tree * sourceTree);
int findChildTree(Tree * rootTree, key key, Tree ** destTree = NULL);
int countChildTrees(Tree * rootTree);
int findCommonParent(Tree * rootTree, key key_1, key key_2, Tree ** destTree = NULL);
int saveTree(Tree * rootTree, char * filename);
int openTree(Tree ** rootTree, char * filename);
/**/