#ifndef TREENODE
#define TREENODE
#include <iostream>
#include <assert.h>

struct TreeNode{

    unsigned int _data;

    TreeNode * _left;

    TreeNode * _right;

    TreeNode * _parent;

    TreeNode * _neighbor_right;

    TreeNode(): _data(0), _left(0), _right(0), _parent(0), _neighbor_right(0) {}

    TreeNode(unsigned int data): _data(data), _left(0), _right(0), _parent(0), _neighbor_right(0) {}

    void insert (unsigned int data, const bool status);

    void deleteBranch ();

    void deleteBranchHelper();

    int printBits(unsigned int *massiv) const;

    void printBitsHelper(unsigned int * massiv, int & lenMassiv, unsigned int controlSequence) const;
};

#endif // TREENODE

