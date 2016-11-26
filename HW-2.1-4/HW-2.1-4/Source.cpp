#include <stdlib.h>
#include "BinaryTree.h"

int main() {
	// Variables initialize
	int return_code = 0;
	Tree * tree_1 = NULL;
	Tree * tree_2 = NULL;
	Tree * temp_tree = NULL;
	Data * data = (Data *)malloc(3 * sizeof(Data));
	data[0].key = 10;
	data[0].data = 10 * 2;
	data[1].key = 5;
	data[1].data = 5 * 2;
	data[2].key = 35;
	data[2].data = 35 * 2;

	// Test of createTree()
	return_code = createTree(&tree_1, data, 3);
	printf_s("createTree(&tree_1, data, 3): %d // data = { {10, 20}, {5, 10}, {35, 70} }\n", return_code);
	if (return_code != SUCCESS) {
		system("pause");
		return return_code;
	}

	// Test of addChildTree()
	printf_s("\n");
	return_code = addChildTree(&tree_1, 1, 1 * 2);
	printf_s("addChildTree(&tree_1, 1, 1*2): %d\n", return_code);
	return_code = addChildTree(&tree_1, 20, 20 * 2);
	printf_s("addChildTree(&tree_1, 20, 20*2): %d\n", return_code);
	return_code = addChildTree(&tree_1, 99, 99 * 2);
	printf_s("addChildTree(&tree_1, 99, 99*2): %d\n", return_code);
	return_code = addChildTree(&tree_1, 4, 4 * 2);
	printf_s("addChildTree(&tree_1, 4, 4*2): %d\n", return_code);
	return_code = addChildTree(&tree_1, 17, 17 * 2);
	printf_s("addChildTree(&tree_1, 17, 17*2): %d\n", return_code);
	return_code = addChildTree(&tree_1, 31, 31 * 2);
	printf_s("addChildTree(&tree_1, 31, 31*2): %d\n", return_code);
	return_code = addChildTree(&tree_1, 53, 53 * 2);
	printf_s("addChildTree(&tree_1, 53, 53*2): %d\n", return_code);

	// Test of removeChildTree()
	printf_s("\n");
	return_code = removeChildTree(&tree_1, 53);
	printf_s("removeChildTree(&tree_1, 53): %d\n", return_code);
	return_code = removeChildTree(&tree_1, 11);
	printf_s("removeChildTree(&tree_1, 11): %d\n", return_code);

	// Test of copyTree()
	printf_s("\n");
	return_code = copyTree(&tree_2, tree_1);
	printf_s("copyTree(&tree_2, tree_1): %d\n", return_code);
	if (return_code != SUCCESS) {
		system("pause");
		return return_code;
	}

	// Test of removeTree()
	printf_s("\n");
	return_code = removeTree(&tree_1);
	printf_s("removeTree(&tree_1): %d\n", return_code);

	// Test of findChildTree()
	printf_s("\n");
	return_code = findChildTree(tree_2, 17, &temp_tree);
	printf_s("findChildTree(tree_2, 17, &temp_tree): %d\n", return_code);
	printf_s("temp_tree->key: %d, temp_tree->data: %d\n", temp_tree->key, temp_tree->data);

	// Test of countChildTrees()
	printf_s("\n");
	return_code = countChildTrees(tree_2);
	printf_s("countChildTrees(tree_2): %d\n", return_code);

	// Test of saveTree()
	printf_s("\n");
	return_code = saveTree(tree_2, "tree");
	printf_s("saveTree(tree_2, \"tree\"): %d\n", return_code);

	// Test of openTree()
	printf_s("\n");
	return_code = openTree(&tree_1, "tree");
	printf_s("openTree(&tree_1, \"tree\"): %d\n", return_code);
	if (return_code != SUCCESS) {
		system("pause");
		return return_code;
	}

	// Test of findCommonParent()
	printf_s("\n");
	return_code = addChildTree(&tree_1, 42, 42 * 2);
	printf_s("addChildTree(&tree_1, 42, 42*2): %d\n", return_code);
	return_code = findCommonParent(tree_1, 20, 42, &temp_tree);
	printf_s("findCommonParent(tree_2, 20, 42, &temp_tree): %d\n", return_code);
	printf_s("temp_tree->key: %d, temp_tree->data: %d\n", temp_tree->key, temp_tree->data);

	// Test of saveTreeInText()
	printf_s("\n");
	return_code = saveTreeInText(tree_2, "tree.txt");
	printf_s("saveTreeInText(tree_2, \"tree.txt\"): %d\n", return_code);

	// Test of openTreeInText()
	printf_s("\n");
	return_code = removeTree(&tree_1);
	printf_s("removeTree(&tree_1): %d\n", return_code);
	return_code = openTreeInText(&tree_1, "tree.txt");
	printf_s("openTreeInText(&tree_1, \"tree.txt\"): %d\n", return_code);
	if (return_code != SUCCESS) {
		system("pause");
		return return_code;
	}

	// Test of findCommonParent()
	printf_s("\n");
	return_code = addChildTree(&tree_1, 42, 42 * 2);
	printf_s("addChildTree(&tree_1, 42, 42*2): %d\n", return_code);
	return_code = findCommonParent(tree_1, 20, 42, &temp_tree);
	printf_s("findCommonParent(tree_2, 20, 42, &temp_tree): %d\n", return_code);
	printf_s("temp_tree->key: %d, temp_tree->data: %d\n", temp_tree->key, temp_tree->data);

	printf_s("\n");
	system("pause");
	return 0;
}