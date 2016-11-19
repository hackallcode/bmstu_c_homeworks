#pragma once

/* Includes */
#include <stdio.h>

/* Structes */
struct Tree {
	int key;
	int data;
	Tree * left;
	Tree * right;
};

/* Errors */
int e_NotFound(int key);
int e_NoKey();
int e_NoMemory();
int e_NoTree();
int e_NoParent();
int e_NoFile();

/* Tree */
int initTree(Tree ** newTree, int key = NULL, int data = NULL);
int addChildTree(Tree * rootTree, int key, int data = NULL);
int removeChildTree(Tree ** rootTree, int key);
int removeTree(Tree ** rootTree);
int copyTree(Tree ** destTree, Tree * sourceTree);
int findChildTree(Tree * rootTree, int key, Tree ** destTree = NULL);
int countChildTrees(Tree * rootTree);
int findCommonParent(Tree * rootTree, int key_1, int key_2, Tree ** destTree = NULL);
int fprintTree(Tree * rootTree, FILE * file);
int saveTree(Tree * rootTree, char * filename);
int openTree(Tree ** rootTree, char * filename);