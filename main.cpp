#include <iostream>
#include <assert.h>
#include <ctime>
#include <vector>
#include "bf.h"
#include "functions.h"
#include "const.h"

int MAX_CONTROL_SEQUENCE;

int main()
{
    // WELL-KNOWN PARAMETERS

    int lenArg1 = 5; // number of variable. MAX m_var = 31
    int lenArg2 = 5; // number of variable. MAX m_var = 31

    // input data for 1 part of genertor
    int lenResult = 59; // length in bits of random number <= 31

    int unitBaseRes = lenResult / NUM_BIT_IN_BASE;
    int remBaseRes = lenResult % NUM_BIT_IN_BASE;
    if (remBaseRes != 0) unitBaseRes++;

    assert(lenArg1 < lenResult);


    // A1

    bf F1(lenArg1, FillTypeRandom);
    unsigned int numberStates1 = 1 << lenArg1;  // quantity states
    unsigned int * setStates1 = new unsigned int [numberStates1];

    for (unsigned int i = 0; i < numberStates1 - 1; i++) setStates1[i] = i + 1; // create transition function g
    setStates1[numberStates1 - 1] = 0;    // create transition function g


    // A2

    bf F2(lenArg2, FillTypeRandom);
    unsigned int  numberStates2 = 1 << lenArg2;

    unsigned int * setStates2 = new unsigned int [numberStates2];
    for (unsigned int  i = 0; i < numberStates2/2 - 1; i++) setStates2[i] = i + 1;
    setStates2[numberStates2/2 - 1] = 0;

    for (unsigned int  i = numberStates2/2; i < numberStates2 - 1; i++) setStates2[i] = i - numberStates2/2 + 1;
    setStates2[numberStates2 - 1] = 0;


    // GENERATOR

    unsigned int  nextState1 = 0; // key 1
    unsigned int  nextState2 = 0; // key 2

    assert((nextState1 < numberStates1) && (nextState2 < (numberStates2 >> 1)));

    // start stopwatch
    srand(time(0));
    clock_t start;
    double duration;
    start = std::clock();

    // start generator of pseudo-random number
    unsigned int * result = new unsigned int [1];
    result = generator(lenResult, F1, setStates1, nextState1, F2, setStates2, nextState2);

    // stop stopwatch
    duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;

    // print time and number from generator
    std::cout << "time = " << duration << std::endl;
    std::cout << "generator = " << result[0] << std::endl;


    // HACK

    // calculate the sequence of bits as a result of the work of the first part of the generator
    // and fill in the form of a hash table H[sequence_bits]=condition
    std::list<unsigned int> *hashTable = new std::list<unsigned int> [1 << lenArg1];
    fillHashTable(lenResult, lenArg1, F1, setStates1, hashTable);

    // initialize massiv for saving keys
    std::vector<std::vector<unsigned int> > keys(1 << lenArg1);

    // start stopwatch
    start = std::clock();

    int sum = 0;

    // run a loop in which for each initial state we compute control sequences with DSS
    for (unsigned int  initCondA2 = 0; initCondA2 < (numberStates2 >> 1); initCondA2++)
    {
        // for each state, all possible control sequences of a certain length write in massContSeq
        // and in countMassContrSeq write quantity of control sequences
        // for some states control sequence may not be
        MAX_CONTROL_SEQUENCE = 20;
        unsigned int **massContrSeq = new unsigned int * [MAX_CONTROL_SEQUENCE];
        for (int i = 0; i < MAX_CONTROL_SEQUENCE; i++)
            massContrSeq[i] = new unsigned int[unitBaseRes];

        unsigned int countSeq = DSS(lenResult, result, initCondA2, F2, setStates2, massContrSeq); // lenResult <= 31

        for (unsigned int  i = 0; i < countSeq; i++)
        {
            // WiTH COLLISION
            std::list<unsigned int> tmpList = hashTable[hash(massContrSeq[i], lenArg1)];

            sum += tmpList.size();
//            std::cout << "len(tmpList) = " << tmpList.size() << endl;
            for (std::list<unsigned int>::iterator it = tmpList.begin(); it != tmpList.end(); it++)
            {
//                std::cout << "Keys : " << *it << " and " << initCondA2 << std::endl;
                std::cout << "massContrSeq[i] = " << massContrSeq[i] << std::endl;
                std::cout << "hash(massContrSeq[i] = " << hash(massContrSeq[i], lenArg1) << std::endl;
                if (!(findKeys(keys, *it, initCondA2)))
                        keys[*it].insert(keys[*it].end(), initCondA2);

//                std::cout << std::endl;
            }
        }

    }

    std::cout << "sum = " << sum << std::endl;

    // stop stopwatch
    duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;

    std::cout << std::endl;
    int numberKeys = 0;

    for (unsigned int i = 0; i < keys.size(); i++)
    {
//        if (keys[i].size() > 0) std::cout << "Key1 = " << i << " Key2 = ";

        for (unsigned int j = 0; j < keys[i].size(); j++)
        {
//            std::cout << keys[i][j] << " ";
            numberKeys++;
        }
//        if (keys[i].size() > 0) std::cout << std::endl;
    }
    std::cout << std::endl << "NumberKeysWithColl = " << numberKeys << std::endl;
//              << "Time = " << duration << std::endl;

    return 0;
}

