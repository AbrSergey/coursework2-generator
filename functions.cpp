#include "functions.h"
#include "tree.h"
#include <iostream>
#include <string>
#include "bf.h"

unsigned int reverseBits (unsigned int input, int lenInput);

unsigned int *generator(int lenRezult, bf F1, unsigned int *setStates1, unsigned int cond1, bf F2, unsigned int *setStates2, unsigned int cond2)
{
    int unitBaseResult = lenRezult / NUM_BIT_IN_BASE;
    int remainder = lenRezult % NUM_BIT_IN_BASE;

    if (remainder != 0) unitBaseResult++;

    unsigned int * result = new unsigned int [unitBaseResult];

    for (int i = 0; i < unitBaseResult; i++)
        for (int j = 0; j < NUM_BIT_IN_BASE; j++)
        {
            cond2 <<= 1;
            cond2 |= F1[cond1];
            cond1 = setStates1[cond1];
            result[i] <<= 1;
            result[i] |= F2[cond2];
            cond2 = setStates2[cond2];
        }

    if (remainder != 0)
    {
        result[unitBaseResult - 1] <<= NUM_BIT_IN_BASE - remainder;
        result[unitBaseResult - 1] >>= NUM_BIT_IN_BASE - remainder;
    }

    return result;
}

void fillHashTable(int lenResult, int lenArg1, bf F1, unsigned int *setStates1, std::list<unsigned int> *hashTable)
{
    // input validation
    assert (lenArg1 <= 31);

    // initialization
    unsigned int numberCond = 1 << lenArg1;

    // computation
    int unitBaseSeq = lenRezult / NUM_BIT_IN_BASE;
    int quintBaseSeq = unitBaseSeq;
    int remainder = lenRezult % NUM_BIT_IN_BASE;

    if (remainder != 0) unitBaseSeq++;

    unsigned int contrlSeq = new unsigned int [unitBaseSeq];

    for (unsigned int condInit = 0; condInit < numberCond; condInit++){
        int condTmp = condInit;

        // get result from generator A1
        int i = 0, b = 0;
        while (i < lenResult)
        {
            for (int j = 0; j < NUM_BIT_IN_BASE && i < lenResult; j++, i++)
            {
                contrlSeq[b] <<= 1;
                contrlSeq[b] |= F1[condTmp];
                condTmp = setStates1[condTmp];
            }
            b++;
            assert (b < unitBaseSeq);
        }

        // create list and add element
        unsigned int h = hash(resPolZheg, quintBaseSeq, lenArg1);

        if (hashTable[h].empty() == true){
            std::list<unsigned int> tmpList;
            tmpList.push_back(condInit);
            hashTable[h] = tmpList;
        }
        // add element to the end of list
        else{
            std::list<unsigned int> tmpList;
            tmpList = hashTable[h];
            tmpList.push_back(condInit);
            hashTable[h] = tmpList;
        }
    }
}

unsigned int hash(unsigned int * data, unsigned int lenData, unsigned int lenResult)
{
    unsigned int * hRes = new unsigned int [lenData];

    for (int z = 0; z < lenData; z++)
    {
        unsigned int l, r, res = data[z];

        // i - quantity of bases
        for (int i = 1, k = 1; i <= 4; i <<= 1, k++)
        {
            // compute b
            unsigned int b = 0;
            for (int t = 0; t < k; t++)
                b += 16 >> t;

            // compute into bases
            for (int j = 0; j < i; j++)
            {
                l = ((res << ((32 >> k) * j + (32 >> k))) >> b);
                r = ((res << (32 >> (k - 1)) * j) >> b);
                unsigned int sum = l + r;
                unsigned int min = l - r;
                sum <<= (32 >> k);
                sum >>= (32 >> k);
                min <<= (32 >> k);
                min >>= (32 >> k);

                unsigned int tmp = sum + (min << (32 >> k));
                res += tmp << (32 >> (k - 1)) * i;
            }
        }

        hRes[z] = res;
    }
    unsigned int result;

    for (int i = 0; i < lenData; i++) result += hRes[i];

    if (data > 1) return result;
    else result % (1 << lenResult);
}

int DSS(int lenResult, unsigned int *result, unsigned int initState, bf F2, unsigned int *setStates2, unsigned int *controlSequence)
{
    assert (lenResult > 0 && lenResult <= 31);

    // computation

    // create a tree whose root is initState (cond2)
    Tree tree(initState);
    TreeNode * root = tree.root();

    int stage = 1;  // stage of tree
    result = reverseBits (result, lenResult); // reverse bit srquence of result
    dssHelper (stage, initState, lenResult, result, F2, setStates2, root); // start of recursion

    // check whether the tree is empty or not
    if (root->_left == NULL && root->_right == NULL)
    {
        root = NULL;
        return 0;
    }

    // if the tree is not empty, then we calculate all possible control sequences
    int countSeq = 0;
    countSeq = root->printBits(controlSequence);

    return countSeq;
}

void dssHelper(int stage, unsigned int state, int lenRes, unsigned int * res, bf F2, unsigned int *setStates2, TreeNode *node)
{
    if (stage > lenRes) return;
    stage++;

    bool bitRes;    // one result bit of array result bits

    if ((res & 1) == 1) bitRes = 1;
    else bitRes = 0;

    res >>= 1;

    state <<= 1;
    state |= 0;
    bool resF2 = F2[state];
    if (resF2 == bitRes)    // check if output of A1 generator is 0
    {
        unsigned int nextCond = setStates2[state];
        node->insert(nextCond, false);      // add left branch, _data = setStates(cond2)
        dssHelper(stage, nextCond, lenRes, res, F2, setStates2, node->_left);
    }

    state |= 1;
    resF2 = F2[state];
    if (resF2 == bitRes)    // check if output of A1 generator is 1
    {
        unsigned int nextCond = setStates2[state];
        node->insert(nextCond, true);       // add right branch, _data = setStates(cond2)
        dssHelper(stage, nextCond, lenRes, res, F2, setStates2, node->_right);
    }

    if (node->_left == NULL && node->_right == NULL && (stage - 1) <= lenRes)
        node->deleteBranch();
}

unsigned int reverseBits (unsigned int input, int lenInput)
{
    unsigned int output = input & 1;

    for (int i = 1; i < lenInput; i++)
    {
        output <<= 1;
        output |= (input & (1 << i)) >> i;
    }
    return output;
}
