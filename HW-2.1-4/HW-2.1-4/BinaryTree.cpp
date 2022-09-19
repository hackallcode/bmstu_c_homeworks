#include "BinaryTree.h"

/* Error functions */

int e_TreeExist() {
#ifdef DEBUG
	printf_s("ERROR: Tree is already initialized!\n");
#endif // DEBUG
	return TREE_EXIST_ERROR;
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

int e_NotFound(key key) {
#ifdef DEBUG
	printf_s("ERROR: Element not found (key = %d)!\n", key);
#endif // DEBUG
	return NOT_FOUND;
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

/***************************************************************/
/*  Change this functions, if type of key or data has changed  */
/***************************************************************/

// Keys compare function
int compare(double x, double y) {
	if (x < y)
		return -1;
	else if (x > y)
		return 1;
	else
		return 0;
}

int compare(char * x, char * y) {
	return strcmp(x, y);
}

// Data setting function
void nullData(data * data) {
	*data = NULL;
}

int clearData(data * data) {
	nullData(data);
	return SUCCESS;
}

int setData(data * destData, data sourceData) {
	int return_code = clearData(destData);
	if (return_code != SUCCESS) return return_code;
	*destData = sourceData;
	return SUCCESS;
}

// Work with data in files
void fwriteData(data data, FILE * file) {
	fwrite(&data, sizeof(int), 1, file);
}

int freadData(data * data, FILE * file) {
	return fread(data, sizeof(int), 1, file);
}

/*********************************/
/*  End of changeable functions  */
/*********************************/

/* All functions */

int initTree(Tree ** newTree, key key, data data) {
	if (*newTree != NULL) return e_TreeExist();
	(*newTree) = (Tree *)malloc(sizeof(Tree));
	if (*newTree == NULL) return e_NoMemory();

	(*newTree)->key = key;
	nullData(&(*newTree)->data);
	int return_code = setData(&(*newTree)->data, data);
	if (return_code != SUCCESS) return return_code;
	(*newTree)->left = NULL;
	(*newTree)->right = NULL;

	return SUCCESS;
}

int createTree(Tree ** newTree, Data * data, int dataNumber) {
	if (*newTree != NULL) return e_TreeExist();
	for (int i = 0; i < dataNumber; i++) {
		int return_code = addChildTree(newTree, data[i].key, data[i].data);
		if (return_code != SUCCESS) return return_code;
	}
	return SUCCESS;
}

int addChildTree(Tree ** rootTree, key key, data data) {
	if (*rootTree == NULL) {
		int return_code = initTree(rootTree, key, data);
		return return_code;
	}
	else {
		Tree * previousTree = NULL;
		Tree * currentTree = *rootTree;
		while (currentTree != NULL) {
			if (compare(key, currentTree->key) == 0) {
				int return_code = clearData(&currentTree->data);
				if (return_code != SUCCESS) return return_code;
				return_code = setData(&currentTree->data, data);
				return return_code;
			}
			else {
				previousTree = currentTree;
				if (compare(key, currentTree->key) < 0)
					currentTree = currentTree->left;
				else
					currentTree = currentTree->right;
			}
		}

		int return_code = initTree(&currentTree, key, data);
		if (return_code != SUCCESS) return return_code;

		if (compare(key, previousTree->key) < 0)
			previousTree->left = currentTree;
		else
			previousTree->right = currentTree;

		return SUCCESS;
	}
}

int removeChildTree(Tree ** rootTree, key key) {
	if (*rootTree == NULL) return e_NoTree();

	Tree * previousTree = NULL;
	Tree * currentTree = *rootTree;
	while (currentTree != NULL) {
		if (compare(key, currentTree->key) == 0) {
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

				int return_code = clearData(&currentTree->data);
				if (return_code != SUCCESS) return return_code;
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
				int return_code = clearData(&currentTree->data);
				if (return_code != SUCCESS) return return_code;
				return_code = setData(&currentTree->data, minRightTree->data);
				if (return_code != SUCCESS) return return_code;

				return_code = clearData(&minRightTree->data);
				if (return_code != SUCCESS) return return_code;
				free(minRightTree);
			}
			return SUCCESS;
		}
		else {
			previousTree = currentTree;
			if (compare(key, currentTree->key) < 0)
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
	int return_code = clearData(&(*rootTree)->data);
	if (return_code != SUCCESS) return return_code;
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

int findChildTree(Tree * rootTree, key key, Tree ** destTree) {
	if (rootTree == NULL) return e_NoTree();

	Tree * currentTree = rootTree;
	while (currentTree != NULL) {
		if (compare(key, currentTree->key) == 0) {
			if (destTree != NULL) *destTree = currentTree;
			return SUCCESS;
		}
		else {
			if (compare(key, currentTree->key) < 0)
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

int findCommonParent(Tree * rootTree, key key_1, key key_2, Tree ** destTree) {
	if (rootTree == NULL) return e_NoTree();
	if (compare(key_1, rootTree->key) == 0 || compare(key_2, rootTree->key) == 0) return e_NoParent();

	Tree * previousTree = NULL;
	Tree * currentTree = rootTree;
	while (currentTree != NULL) {
		if (compare(key_1, currentTree->key) == 0 || compare(key_2, currentTree->key) == 0) {
			if (findChildTree(currentTree, key_1) == NOT_FOUND || findChildTree(currentTree, key_2) == NOT_FOUND) return NOT_FOUND;
			*destTree = previousTree;
			return SUCCESS;
		}
		else {
			previousTree = currentTree;
			if ((compare(key_1, currentTree->key) < 0 && compare(currentTree->key, key_2) < 0) || (compare(key_2, currentTree->key) < 0 && compare(currentTree->key, key_1) < 0)) {
				if (findChildTree(currentTree, key_1) == NOT_FOUND || findChildTree(currentTree, key_2) == NOT_FOUND) return NOT_FOUND;
				*destTree = currentTree;
				return SUCCESS;
			}
			else if (compare(key_1, currentTree->key) < 0)
				currentTree = currentTree->left;
			else
				currentTree = currentTree->right;
		}
	}
}

int fwriteTree(Tree * rootTree, FILE * file) {
	if (rootTree == NULL) return SUCCESS;
	fwrite(&rootTree->key, sizeof(key), 1, file);
	fwriteData(rootTree->data, file);
	fwriteTree(rootTree->left, file);
	fwriteTree(rootTree->right, file);
	return SUCCESS;
}

int saveTree(Tree * rootTree, char * filename) {
	if (rootTree == NULL) return e_NoTree();

	FILE * file;
	fopen_s(&file, filename, "wb");
	if (file == NULL) return e_NoFile();

	fwriteTree(rootTree, file);

	fclose(file);
	return SUCCESS;
}

int openTree(Tree ** rootTree, char * filename) {
	if (*rootTree != NULL) return e_TreeExist();

	FILE * file;
	fopen_s(&file, filename, "r");
	if (file == NULL) return e_NoFile();

	while (!feof(file)) {
		key key;
		data data;
		if (fread(&key, sizeof(key), 1, file) < 1) break; // IF EOF
		if (freadData(&data, file) < 1) break;
		int return_code = addChildTree(rootTree, key, data);
		if (return_code != 0) return return_code;
	};

	fclose(file);
	return SUCCESS;
}
