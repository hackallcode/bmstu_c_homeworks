#pragma once

/* Includes */
#include <stdio.h>
/**/

/* TypeDefs */
typedef int key;
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
int saveTreeInText(Tree * rootTree, char * filename);
int openTreeInText(Tree ** rootTree, char * filename);
/**/