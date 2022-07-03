/*
 * WVTable.cpp
 * by: Valerie Zhang
 *
 * An implementation of the WVTable class. A WVTable stores wordVar instances.
 */

#include "WVTable.h"

/*
 * default constructor
 */
WVTable::WVTable() {
    capacity = initial_capacity;
    table = new wordVar[capacity];
    wordVar w;
    for (int i = 0; i < capacity; i++) {
        table[i] = w;
    }
    numVars = 0;
}
/*
 * destructor
 */
WVTable::~WVTable() {
    delete [] table;
    table = nullptr;
    numVars = 0;
}
/* 
 * size()
 * Purpose:     get # of word variables
 * Parameters:  none
 * Returns:     total number of word variables
 */
int WVTable::size() {
    return numVars;
}
/*
 * getWordEntry()
 * Purpose:     get reference to specified word entry in table
 * Parameters:  num and direction of variable
 * Returns:     pointer to entry in table
 */
wordVar* WVTable::getWordEntry(int num, bool direction) {
    return &table[linearProbeIndex(num, direction, hashIndex(num))];
}
/*
 * addVar()
 * Purpose:     add word variable to table
 * Parameters:  num, orientation, start row and col of variable
 * Returns:     none
 */
void WVTable::addVar(int num, bool orientation, int row, int col) {
    wordVar toAdd(num, orientation, row, col);
    table[linearProbeIndex(num, orientation, hashIndex(num))] = toAdd;
    numVars++;
  
}
/*
 * complete()
 * Purpose:     determines if all variables have been assigned a value
 * Parameters:  none
 * Returns:     true if all variables have a value, false if not
 */
bool WVTable::complete() {
    for (int i = 0; i < capacity; i++) {
        if (table[i].getNum() != 0) {
            if (table[i].getValue() == "NO_VALUE") {
                return false;
            }
        }
    }
    return true;
}
/*
 * selectVariable()
 * Purpose:     selects variable to be assigned next in search
 * Parameters:  none
 * Returns:     pointer to most constrained/most constraining variable
 */
wordVar* WVTable::selectVariable(wordVar *prev) {
    wordVar *var = nullptr;
    for (int i = 0; i < capacity; i++) { // loop through table
        // if variable is unassigned
        if ((table[i].getNum() != 0) and (table[i].getValue() == "NO_VALUE")) {
        //     cerr << "Comparing: " << table[i].getNum();
        // if (table[i].getOrientation() == true) {cerr << "-down\n";} 
        // else {cerr << "-across\n";}
            if ((var == nullptr) and (prev != &table[i])) { // for first unassigned var encountered
                var = &table[i]; 
            } else {
                if (var->getDomain() > table[i].getDomain()) {
                    var = &table[i]; // assigns to most constrained var
                } else if (var->numConstraints() == table[i].numConstraints()) {
                    int num = var->getNum();
                    bool direction = var->getOrientation();
                    var = mostConstraining(linearProbeIndex(num, direction, hashIndex(num)) , i);
                }
            }   
        }
    }
    return var;
}
/*
 * mostConstraining()
 * Purpose:     determines which variables is the most constraining
 * Parameters:  indices of the two variables to compare
 * Returns:     pointer to most constraining word variable
 */
wordVar* WVTable:: mostConstraining(int index1, int index2) {
    size_t unassigned1 = 0;
    size_t unassigned2 = 0;
    // counts # of unassigned vars that share constraint with wordVar1
    for (int i = 0; i < table[index1].numConstraints(); i++) { 
        pair<int, bool> var = {table[index1].getConstraintNum(i), 
                               table[index1].getConstraintOrientation(i)};
        if (table[linearProbeIndex(var.first, var.second, hashIndex(var.first))].getValue() == "NO_VALUE") {
            unassigned1++;
        }
    }
    // counts # of unassigned vars that share constraint with wordVar2
    for (int i = 0; i < table[index2].numConstraints(); i++) { 
        pair<int, bool> var = {table[index2].getConstraintNum(i), 
                               table[index2].getConstraintOrientation(i)};
        if (table[linearProbeIndex(var.first, var.second, hashIndex(var.first))].getValue() == "NO_VALUE") {
            unassigned2++;
        }
    }
    if (unassigned1 >= unassigned2) {
        return &table[index1];
    } else {
        return &table[index2];
    }
}
/** *****************************************************************\
*                   hashtable maintaining functions                  *
\********************************************************************/
/* expand()
 * parameters: none
 * purpose: expands the capacity and rehashes all cities
 * returns: n/a
 */
void WVTable::expand()
{
    int        oldCapacity = capacity;
    wordVar *oldTable        = table; // save old table to delete it

    capacity = 2 * capacity + 2;        // update capacity
    table    = new wordVar[capacity]; // create new table
    wordVar w;
    for (int i = 0; i < capacity; i++) {
        table[i] = w;
    }
    for (int i = 0; i < oldCapacity; i++) {
        if (oldTable[i].getNum() == 0) { // if entry empty: continue,
            int key = oldTable[i].getNum();
            table[linearProbeIndex(key, oldTable[i].getOrientation(), 
                    hashIndex(key))] = oldTable[i];
        }
    }
    delete [] oldTable;
}
/* loadFactorCheck()
 * parameters: none
 * purpose: checks the load factor and rehashes if necessary
 * returns: n/a
 */
void WVTable::loadFactorCheck()
{
    double c  = capacity * 1.0;
    double nc = numVars * 1.0;
    if (nc / c > .7) {
        expand();
    }
}

/** *****************************************************************\
*                           hash functions                           *
\********************************************************************/

/* linearProbeIndex()
 * parameters:
 *  -   int index represents the original hash index for a key
 *  -   string &key is a key to find the index for
 * purpose: to find the correct index for a key using linear probing
 * returns: an integer representing the key's index
 */
int WVTable::linearProbeIndex(int key, bool direction, int index)
{
    int  attempt = 0;
    bool found   = false;
    while (!found) {
        index = (index + attempt) % capacity;
        if (table[index].getNum() == 0) { 
            return index;
        } else if (table[index].getNum() == key) {
            if (table[index].getOrientation() == direction) { // checks if orientation matches
                return index;
            }
        }
        attempt++;
    }
    return 0;
}
/* hashIndex()
 * parameters:
 *  -   string &key is a key to find an index for
 * purpose: calls the hash function on the key, then mods it to get an
 *          index
 * returns: the index corresponding to the key
 */
int WVTable::hashIndex(int key)
{
    // first get hash value for key
    // then mod by capacity to get index
    return (hash<int>{}(key) % capacity);
}
/*
 * printDomainandVal()
 * Purpose:     prints domain and initial assignment for all variables
 * Parameters:  none
 * Returns:     none
 */
void WVTable::printDomainandVal() {
    for (int i = 0; i < capacity; i++) {
        if (table[i].getNum() != 0) {
            table[i].print();
        }
    }
}