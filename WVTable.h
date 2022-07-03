/*
 * WVTable.h
 * by: Valerie Zhang
 *
 * Purpose: The WVTable class stores all wordVars to be filled in a crossword
 *
 */

#ifndef _WVTABLE_H_
#define _WVTABLE_H_

#include "WordVar.h"

using namespace std;

static const int initial_capacity = 100;

class WVTable {
public:
    WVTable();
    ~WVTable();

    wordVar* getWordEntry(int num, bool direction); 
    int size();
    void addVar(int num, bool orientation, int row, int col);
    bool complete();
    wordVar* selectVariable(wordVar *prev);

    void printDomainandVal();

private:
    int capacity;
    int numVars;
    wordVar *table;

    wordVar* mostConstraining(int index1, int index2);

    /* maintaining the table */
    void expand();
    void loadFactorCheck();

    /* hash functions */
    int hashIndex(int key);
    int linearProbeIndex(int key, bool direction, int index);

};
#endif