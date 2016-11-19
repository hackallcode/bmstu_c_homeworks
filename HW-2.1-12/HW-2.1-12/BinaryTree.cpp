#include <stdio.h>
#include <stdlib.h>

#define DEBUG
#define SUCCESS 0
#define NO_KEY_ERROR 1
#define NO_MEMORY_ERROR 2
#define NO_TREE_ERROR 3
#define NOT_FOUND 4
#define NO_PARENT_ERROR 5
#define NO_FILE_ERROR 6

struct Tree {
	int key;
	int data;
	Tree * left;
	Tree * right;
};

int e_NotFound(int key) {
#ifdef DEBUG
	printf_s("ERROR: Element not found (key = %d)!\n", key);
#endif // DEBUG
	return NOT_FOUND;
}

int e_NoKey() {
#ifdef DEBUG
	printf_s("ERROR: Key not entered!\n");
#endif // DEBUG
	return NO_KEY_ERROR;
}

int e_NoMemory() {
#ifdef DEBUG
	printf_s("ERROR: Impossible allocate memory!\n");
#endif // DEBUG
	return NO_MEMORY_ERROR;
}

int e_NoTree() {
#ifdef DEBUG
	printf_s("ERROR: Tree not initialized!\n");
#endif // DEBUG
	return NO_TREE_ERROR;
}

int e_NoParent() {
#ifdef DEBUG
	printf_s("ERROR: Tree hasn't parent!\n");
#endif // DEBUG
	return NO_PARENT_ERROR;
}

int e_NoFile() {
#ifdef DEBUG
	printf_s("ERROR: Impossile create or open file!\n");
#endif // DEBUG
	return NO_FILE_ERROR;
}

int initTree(Tree ** newTree, int key = NULL, int data = NULL) {
	if (key == NULL && data != NULL) return e_NoKey();

	(*newTree) = (Tree *)malloc(sizeof(Tree));
	if (*newTree == NULL) return e_NoMemory();

	(*newTree)->key = key;
	(*newTree)->data = data;
	(*newTree)->left = NULL;
	(*newTree)->right = NULL;

	return SUCCESS;
}

int addChildTree(Tree * rootTree, int key, int data = NULL) {
	if (rootTree == NULL) return e_NoTree();
	if (key == NULL) return e_NoKey();

	if (rootTree->key == NULL) {
		rootTree->key = key;
		rootTree->data = data;
		rootTree->left = NULL;
		rootTree->right = NULL;
	}
	else {
		Tree * previousTree = NULL;
		Tree * currentTree = rootTree;
		while (currentTree != NULL) {
			if (key == currentTree->key) {
				currentTree->data = data;
				return SUCCESS;
			}
			else {
				previousTree = currentTree;
				if (key < currentTree->key)
					currentTree = currentTree->left;
				else
					currentTree = currentTree->right;
			}
		}

		int return_code = initTree(&currentTree, key, data);
		if (return_code != SUCCESS) return return_code;

		if (key < previousTree->key)
			previousTree->left = currentTree;
		else
			previousTree->right = currentTree;
	}

	return SUCCESS;
}

int removeChildTree(Tree ** rootTree, int key) {
	if (*rootTree == NULL) return e_NoTree();
	if (key == NULL) return e_NoKey();

	Tree * previousTree = NULL;
	Tree * currentTree = *rootTree;
	while (currentTree != NULL) {
		if (key == currentTree->key) {
			if (currentTree->right == NULL) { // Without right child
				if (previousTree == NULL) { // If root tree
					*rootTree = currentTree->left;
				}
				else {
					if (currentTree == previousTree->left)
						previousTree->left = currentTree->left;
					else
						previousTree->right = currentTree->left;
				}

				free(currentTree);
			}
			else { // With right tree
				Tree * minRightTree = currentTree->right;
				previousTree = NULL;
				while (minRightTree->left != NULL) {
					previousTree = minRightTree;
					minRightTree = minRightTree->left;
				}

				if (previousTree != NULL)
					previousTree->left = minRightTree->right;
				else
					currentTree->right = minRightTree->right;

				currentTree->key = minRightTree->key;
				currentTree->data = minRightTree->data;

				free(minRightTree);
			}
			return SUCCESS;
		}
		else {
			previousTree = currentTree;
			if (key < currentTree->key)
				currentTree = currentTree->left;
			else
				currentTree = currentTree->right;
		}
	}

	return e_NotFound(key);
}

int removeTree(Tree ** rootTree) {
	if (*rootTree == NULL) return SUCCESS;
	removeTree(&(*rootTree)->left);
	removeTree(&(*rootTree)->right);
	free(*rootTree);
	*rootTree = NULL;
	return SUCCESS;
}

int copyTree(Tree ** destTree, Tree * sourceTree) {
	if (sourceTree == NULL) {
		*destTree = NULL;
		return SUCCESS;
	}

	int return_code;

	return_code = initTree(destTree, sourceTree->key, sourceTree->data);
	if (return_code != SUCCESS) return return_code;

	return_code = copyTree(&(*destTree)->left, sourceTree->left);
	if (return_code != SUCCESS) return return_code;
	return_code = copyTree(&(*destTree)->right, sourceTree->right);
	if (return_code != SUCCESS) return return_code;

	return SUCCESS;
}

int findChildTree(Tree * rootTree, int key, Tree ** destTree = NULL) {
	if (rootTree == NULL) return e_NoTree();
	if (key == NULL) return e_NoKey();

	Tree * currentTree = rootTree;
	while (currentTree != NULL) {
		if (key == currentTree->key) {
			if (destTree != NULL) *destTree = currentTree;
			return SUCCESS;
		}
		else {
			if (key < currentTree->key)
				currentTree = currentTree->left;
			else
				currentTree = currentTree->right;
		}
	}
	return e_NotFound(key);
}

int countChildTrees(Tree * rootTree) {
	if (rootTree == NULL) return 0;
	return countChildTrees(rootTree->left) + countChildTrees(rootTree->right) + 1;
}

int findCommonParent(Tree * rootTree, int key_1, int key_2, Tree ** destTree = NULL) {
	if (rootTree == NULL) return e_NoTree();
	if (key_1 == NULL || key_2 == NULL) return e_NoKey();
	if (key_1 == rootTree->key || key_2 == rootTree->key) return e_NoParent();

	Tree * previousTree = NULL;
	Tree * currentTree = rootTree;
	while (currentTree != NULL) {
		if (key_1 == currentTree->key || key_2 == currentTree->key) {
			if (findChildTree(currentTree, key_1) == NOT_FOUND || findChildTree(currentTree, key_2) == NOT_FOUND) return NOT_FOUND;
			*destTree = previousTree;
			return SUCCESS;
		}
		else {
			previousTree = currentTree;
			if ((key_1 < currentTree->key && currentTree->key < key_2) || (key_2 < currentTree->key && currentTree->key < key_1)) {
				if (findChildTree(currentTree, key_1) == NOT_FOUND || findChildTree(currentTree, key_2) == NOT_FOUND) return NOT_FOUND;
				*destTree = currentTree;
				return SUCCESS;
			}
			else if (key_1 < currentTree->key)
				currentTree = currentTree->left;
			else
				currentTree = currentTree->right;
		}
	}
}

int fprintTree(Tree * rootTree, FILE * file) {
	if (rootTree == NULL) return SUCCESS;
	fprintf(file, "%d %d\n", rootTree->key, rootTree->data);
	fprintTree(rootTree->left, file);
	fprintTree(rootTree->right, file);
	return SUCCESS;
}

int saveTree(Tree * rootTree, char * filename) {
	if (rootTree == NULL) return e_NoTree();

	FILE * file;
	fopen_s(&file, filename, "w");
	if (file == NULL) return e_NoFile();

	fprintTree(rootTree, file);

	fclose(file);
	return SUCCESS;
}

int openTree(Tree ** rootTree, char * filename) {
	FILE * file;
	fopen_s(&file, filename, "r");
	if (file == NULL) return e_NoFile();

	if (*rootTree != NULL) removeTree(rootTree);
	int return_code = initTree(rootTree);
	if (return_code != SUCCESS) return return_code;

	int key;
	int data;
	while (!feof(file)) {
		fscanf_s(file, "%d %d", &key, &data);
		addChildTree(*rootTree, key, data);
	}

	fclose(file);
	return SUCCESS;
}
