#include <stdlib.h>
#include "BinaryTree.h"

int main() {
	// Variables initialize
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
	printf_s("createTree(&tree_1, data, 3): %d\n", createTree(&tree_1, data, 3));

	// Test of addChildTree()
	printf_s("\n");
	printf_s("addChildTree(&tree_1, 1, 1*2): %d\n", addChildTree(&tree_1, 1, 1 * 2));
	printf_s("addChildTree(&tree_1, 20, 20*2): %d\n", addChildTree(&tree_1, 20, 20 * 2));
	printf_s("addChildTree(&tree_1, 99, 99*2): %d\n", addChildTree(&tree_1, 99, 99 * 2));
	printf_s("addChildTree(&tree_1, 4, 4*2): %d\n", addChildTree(&tree_1, 4, 4 * 2));
	printf_s("addChildTree(&tree_1, 17, 17*2): %d\n", addChildTree(&tree_1, 17, 17 * 2));
	printf_s("addChildTree(&tree_1, 31, 31*2): %d\n", addChildTree(&tree_1, 31, 31 * 2));
	printf_s("addChildTree(&tree_1, 53, 53*2): %d\n", addChildTree(&tree_1, 53, 53 * 2));

	// Test of removeChildTree()
	printf_s("\n");
	printf_s("removeChildTree(&tree_1, 53): %d\n", removeChildTree(&tree_1, 53));
	printf_s("removeChildTree(&tree_1, 11): %d\n", removeChildTree(&tree_1, 11));

	// Test of copyTree()
	printf_s("\n");
	printf_s("copyTree(&tree_2, tree_1): %d\n", copyTree(&tree_2, tree_1));

	// Test of removeTree()
	printf_s("\n");
	printf_s("removeTree(&tree_1): %d\n", removeTree(&tree_1));

	// Test of findChildTree()
	printf_s("\n");
	printf_s("findChildTree(tree_2, 17, &temp_tree): %d\n", findChildTree(tree_2, 17, &temp_tree));
	printf_s("temp_tree->key: %d, temp_tree->data: %d\n", temp_tree->key, temp_tree->data);

	// Test of countChildTrees()
	printf_s("\n");
	printf_s("countChildTrees(tree_2): %d\n", countChildTrees(tree_2));

	// Test of saveTree()
	printf_s("\n");
	printf_s("saveTree(tree_2, \"tree\")\n", saveTree(tree_2, "tree"));
	printf_s("saveTreeInText(tree_2, \"tree.txt\")\n", saveTreeInText(tree_2, "tree.txt"));

	// Test of openTree()
	printf_s("\n");
	tree_1 = NULL;
	printf_s("openTree(&tree_1, \"tree\")\n", openTree(&tree_1, "tree"));

	// Test of findCommonParent()
	printf_s("\n");
	printf_s("addChildTree(&tree_1, 42, 42*2): %d\n", addChildTree(&tree_1, 42, 42 * 2));
	printf_s("findCommonParent(tree_2, 20, 42, &temp_tree): %d\n", findCommonParent(tree_1, 20, 42, &temp_tree));
	printf_s("temp_tree->key: %d, temp_tree->data: %d\n", temp_tree->key, temp_tree->data);

	// Test of openTree()
	printf_s("\n");
	tree_1 = NULL;
	printf_s("openTreeInText(&tree_1, \"tree.txt\")\n", openTreeInText(&tree_1, "tree.txt"));
	
	// Test of findCommonParent()
	printf_s("\n");
	printf_s("addChildTree(&tree_1, 42, 42*2): %d\n", addChildTree(&tree_1, 42, 42 * 2));
	printf_s("findCommonParent(tree_2, 20, 42, &temp_tree): %d\n", findCommonParent(tree_1, 20, 42, &temp_tree));
	printf_s("temp_tree->key: %d, temp_tree->data: %d\n", temp_tree->key, temp_tree->data);

	printf_s("\n");
	system("pause");
	return 0;
}