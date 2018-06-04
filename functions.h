#ifndef FUNCTIONS
#define FUNCTIONS
#include <list>
#include "bf.h"
#include "treenode.h"

unsigned int *generator (int lenRezult,
                          bf F1, unsigned int * setStates1, unsigned int cond1,
                          bf F2, unsigned int * setStates2, unsigned int cond2);

void fillHashTable(int lenResult, int lenArg1, bf F1, unsigned int *setStates1,
                             std::list<unsigned int> *hashTable);

unsigned int hash (unsigned int *data, unsigned int lenData, unsigned int lenResult);

int DSS (int lenResult, unsigned int * result, unsigned int initState,
          bf F2, unsigned int * setStates2, unsigned int ** controlSequence);

void dssHelper(int stage, unsigned int state, int lenRes, unsigned int * res,
                bf F2, unsigned int * setStates2, TreeNode *node);

#endif // FUNCTIONS

