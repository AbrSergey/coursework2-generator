#ifndef TREE_H
#define TREE_H

#include <iostream>
#include <assert.h>
#include "treenode.h"

class Tree
{
private:
    TreeNode *_root = NULL;

    void deleteHelper(TreeNode *);

    void printHelper(TreeNode*, int) const;

public:
    Tree ();

    Tree (unsigned int);

    ~Tree ();

    TreeNode * root();

    void print() const;
};
#endif // TREE_H
