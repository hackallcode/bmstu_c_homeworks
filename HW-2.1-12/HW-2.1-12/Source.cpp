#include <stdlib.h>
#include "BinaryTree.h"

int main() {
	// Variables initialize
	Tree * tree_1;
	Tree * tree_2;
	Tree * temp_tree;

	// Test of initTree()
	printf_s("initTree(&tree_1): %d\n", initTree(&tree_1));

	// Test of addChildTree()
	printf_s("\n");
	printf_s("addChildTree(tree_1, 10, 10*2): %d\n", addChildTree(tree_1, 10, 10 * 2));
	printf_s("addChildTree(tree_1, 5, 5*2): %d\n", addChildTree(tree_1, 5, 5 * 2));
	printf_s("addChildTree(tree_1, 35, 35*2): %d\n", addChildTree(tree_1, 35, 35 * 2));
	printf_s("addChildTree(tree_1, 1, 1*2): %d\n", addChildTree(tree_1, 1, 1 * 2));
	printf_s("addChildTree(tree_1, 20, 20*2): %d\n", addChildTree(tree_1, 20, 20 * 2));
	printf_s("addChildTree(tree_1, 99, 99*2): %d\n", addChildTree(tree_1, 99, 99 * 2));
	printf_s("addChildTree(tree_1, 4, 4*2): %d\n", addChildTree(tree_1, 4, 4 * 2));
	printf_s("addChildTree(tree_1, 17, 17*2): %d\n", addChildTree(tree_1, 17, 17 * 2));
	printf_s("addChildTree(tree_1, 31, 31*2): %d\n", addChildTree(tree_1, 31, 31 * 2));
	printf_s("addChildTree(tree_1, 53, 53*2): %d\n", addChildTree(tree_1, 53, 53 * 2));

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
	printf_s("saveTree(tree_2, \"tree\")", saveTree(tree_2, "tree"));

	// Test of openTree()
	printf_s("\n");
	printf_s("openTree(&tree_1, \"tree\")", openTree(&tree_1, "tree"));

	// Test of findCommonParent()
	printf_s("\n");
	printf_s("addChildTree(tree_1, 42, 42*2): %d\n", addChildTree(tree_1, 42, 42 * 2));
	printf_s("findCommonParent(tree_2, 20, 42, &temp_tree): %d\n", findCommonParent(tree_1, 20, 42, &temp_tree));
	printf_s("temp_tree->key: %d, temp_tree->data: %d\n", temp_tree->key, temp_tree->data);

	printf_s("\n");
	system("pause");
	return 0;
}