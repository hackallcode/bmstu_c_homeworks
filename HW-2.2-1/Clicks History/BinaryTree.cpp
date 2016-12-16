#include "stdafx.h"
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

int e_NotFound(KEY key) {
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
int initData(DATA * data) {
	data->dataNumber = 0;
	data->data = (ME_data *)malloc(sizeof(ME_data));
	if (data->data == NULL) return e_NoMemory();
	data->data[0].id = 0;
	data->data[0].type = 0;
	data->data[0].x = 0;
	data->data[0].y = 0;
	return SUCCESS;
}

void nullData(DATA * data) {
	data->dataNumber = 0;
	data->data = NULL;
}

int clearData(DATA * data) {
	if (data->data != NULL) free(data->data);
	nullData(data);
	return SUCCESS;
}

int addData(DATA * destData, DATA * sourceData) {
	destData->data = (ME_data *)realloc(destData->data, sizeof(ME_data) * (destData->dataNumber + sourceData->dataNumber));
	if (destData->data == NULL) return e_NoMemory();
	for (int i = 0; i < sourceData->dataNumber; i++) {
		destData->data[destData->dataNumber + i].id = sourceData->data[i].id;
		destData->data[destData->dataNumber + i].type = sourceData->data[i].type;
		destData->data[destData->dataNumber + i].x = sourceData->data[i].x;
		destData->data[destData->dataNumber + i].y = sourceData->data[i].y;
	}
	destData->dataNumber += sourceData->dataNumber;
	return SUCCESS;
}

// Work with data in files
void fwriteData(DATA * data, FILE * file) {
	fwrite(&data->dataNumber, sizeof(int), 1, file);
	fwrite(data->data, sizeof(ME_data), data->dataNumber, file);
}

int freadData(DATA * data, FILE * file) {
	int return_code;
	return_code = fread(&data->dataNumber, sizeof(int), 1, file);
	if (return_code < 0) return return_code;
	return_code = fread(data->data, sizeof(ME_data), data->dataNumber, file);
	if (return_code < 0) return return_code;
	return 1; // Must return value > 0
}

/*********************************/
/*  End of changeable functions  */
/*********************************/

/* All functions */

int initTree(TREE ** newTree, KEY key, DATA * data) {
	if (*newTree != NULL) return e_TreeExist();
	(*newTree) = (TREE *)malloc(sizeof(TREE));
	if (*newTree == NULL) return e_NoMemory();

	(*newTree)->key = key;
	nullData(&(*newTree)->data);
	int return_code = addData(&(*newTree)->data, data);
	if (return_code != SUCCESS) return return_code;
	(*newTree)->left = NULL;
	(*newTree)->right = NULL;

	return SUCCESS;
}

int createTree(TREE ** newTree, KEY * key, DATA * data) {
	if (*newTree != NULL) return e_TreeExist();
	for (int i = 0; i < data->dataNumber; i++) {
		int return_code = addChildTree(newTree, key[i], data);
		if (return_code != SUCCESS) return return_code;
	}
	return SUCCESS;
}

int addChildTree(TREE ** rootTree, KEY key, DATA * data) {
	if (*rootTree == NULL) {
		int return_code = initTree(rootTree, key, data);
		return return_code;
	}
	else {
		TREE * previousTree = NULL;
		TREE * currentTree = *rootTree;
		while (currentTree != NULL) {
			if (compare(key, currentTree->key) == 0) {
				int return_code = addData(&currentTree->data, data);
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

int removeChildTree(TREE ** rootTree, KEY key) {
	if (*rootTree == NULL) return e_NoTree();

	TREE * previousTree = NULL;
	TREE * currentTree = *rootTree;
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
				TREE * minRightTree = currentTree->right;
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
				return_code = addData(&currentTree->data, &minRightTree->data);
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

int removeTree(TREE ** rootTree) {
	if (*rootTree == NULL) return SUCCESS;
	removeTree(&(*rootTree)->left);
	removeTree(&(*rootTree)->right);
	int return_code = clearData(&(*rootTree)->data);
	if (return_code != SUCCESS) return return_code;
	free(*rootTree);
	*rootTree = NULL;
	return SUCCESS;
}

int copyTree(TREE ** destTree, TREE * sourceTree) {
	if (sourceTree == NULL) {
		*destTree = NULL;
		return SUCCESS;
	}

	int return_code;

	return_code = initTree(destTree, sourceTree->key, &sourceTree->data);
	if (return_code != SUCCESS) return return_code;

	return_code = copyTree(&(*destTree)->left, sourceTree->left);
	if (return_code != SUCCESS) return return_code;
	return_code = copyTree(&(*destTree)->right, sourceTree->right);
	if (return_code != SUCCESS) return return_code;

	return SUCCESS;
}

int findChildTree(TREE * rootTree, KEY key, TREE ** destTree) {
	if (rootTree == NULL) return e_NoTree();

	TREE * currentTree = rootTree;
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

int countChildTrees(TREE * rootTree) {
	if (rootTree == NULL) return 0;
	return countChildTrees(rootTree->left) + countChildTrees(rootTree->right) + 1;
}

int findCommonParent(TREE * rootTree, KEY key_1, KEY key_2, TREE ** destTree) {
	if (rootTree == NULL) return e_NoTree();
	if (compare(key_1, rootTree->key) == 0 || compare(key_2, rootTree->key) == 0) return e_NoParent();

	TREE * previousTree = NULL;
	TREE * currentTree = rootTree;
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

int fwriteTree(TREE * rootTree, FILE * file) {
	if (rootTree == NULL) return SUCCESS;
	fwrite(&rootTree->key, sizeof(KEY), 1, file);
	fwriteData(&rootTree->data, file);
	fwriteTree(rootTree->left, file);
	fwriteTree(rootTree->right, file);
	return SUCCESS;
}

int saveTree(TREE * rootTree, TCHAR * filename) {
	if (rootTree == NULL) return e_NoTree();

	FILE * file;
#ifdef _UNICODE	
	_wfopen_s(&file, filename, L"wb");
#else 
	fopen_s(&file, filename, "wb");
#endif
	if (file == NULL) return e_NoFile();

	fwriteTree(rootTree, file);

	fclose(file);
	return SUCCESS;
}

int openTree(TREE ** rootTree, TCHAR * filename) {
	if (*rootTree != NULL) return e_TreeExist();

	FILE * file;
#ifdef _UNICODE	
	_wfopen_s(&file, filename, L"r");
#else
	fopen_s(&file, filename, "r");
#endif
	if (file == NULL) return e_NoFile();

	while (!feof(file)) {
		KEY key;
		DATA data;
		int return_code = initData(&data);
		if (return_code != 0) return return_code;
		if (fread(&key, sizeof(key), 1, file) < 1) break; // IF EOF
		if (freadData(&data, file) < 1) break;
		return_code = addChildTree(rootTree, key, &data);
		if (return_code != 0) return return_code;
	};

	fclose(file);
	return SUCCESS;
}
