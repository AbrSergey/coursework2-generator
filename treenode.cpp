#include <iostream>
#include <assert.h>
#include "treenode.h"
#include "const.h"

void TreeNode::insert(unsigned int data, const bool status)
{
    if (!status){
        _left = new TreeNode(data);
        _left->_parent = this;

        if (_right != NULL) _left->_neighbor_right = _right;
    }
    else{
        _right = new TreeNode(data);
        _right->_parent = this;

        if (_left != NULL) _left->_neighbor_right = _right;
    }
}

void TreeNode::deleteBranch()
{
    assert (_left == NULL || _right == NULL);
    deleteBranchHelper();
}

void TreeNode::deleteBranchHelper()
{
    if (_parent == NULL) return;

    if (_parent->_left == this)
    {
        _parent->_left = NULL;
        delete this;
        return;
    }
    if (_parent->_right == this)
    {
        if (_parent->_left != NULL)
        {
            _parent->_right = NULL;
            delete this;
            return;
        }
        _parent->deleteBranchHelper();
        _parent->_right = NULL;
        delete this;
        return;
    }
}

int TreeNode::printBits(unsigned int * massiv) const
{
    int lenMassiv = 0;

    printBitsHelper(massiv, lenMassiv, 0);

    return lenMassiv;
}

void TreeNode::printBitsHelper(unsigned int * massiv, int &lenMassiv, unsigned int controlSequence) const
{
    if (_left != NULL)
    {
        controlSequence <<= 1;
        _left->printBitsHelper(massiv, lenMassiv, controlSequence);
        controlSequence >>= 1;
    }
    if (_right != NULL)
    {
        controlSequence <<= 1;
        controlSequence |= 1;
        _right->printBitsHelper(massiv, lenMassiv, controlSequence);
    }
    if (_left == NULL && _right == NULL)
    {
        massiv[lenMassiv] = controlSequence;
        lenMassiv++;

        // lenMassiv must be less then MAX_CONTROL_SEQUENCE
        assert (lenMassiv < MAX_CONTROL_SEQUENCE);
    }
}
