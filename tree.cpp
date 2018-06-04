#include <iostream>
#include <assert.h>
#include "tree.h"

Tree::Tree(): _root(0) {}

Tree::Tree(unsigned int data)
{
    _root = new TreeNode(data);
}

Tree::~Tree()
{
    deleteHelper(_root);
}

void Tree::deleteHelper(TreeNode *node)
{
    if (node != NULL)
    {
        deleteHelper(node->_left);
        deleteHelper(node->_right);
        delete node;
    }
}


TreeNode *Tree::root()
{
    assert(_root != NULL);
    return _root;
}

void Tree::print() const
{
    printHelper(_root, 0);
}

void Tree::printHelper(TreeNode *node, int spaces) const
{
    while (node != 0)
    {
        printHelper(node->_right, spaces + 5);

        for (int i = 1; i < spaces; ++i)
            std::cout << ' ';

        std::cout << node->_data << std::endl;

        node = node->_left;
        spaces += 5;
    }
}
