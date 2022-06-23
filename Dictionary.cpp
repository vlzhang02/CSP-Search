/*
 * Dictionary.cpp
 * by: Valerie Zhang
 *
 * Purpose: The Dictionary class stores all the words eligible to fill out the 
 *          crossword
 */

#include "Dictionary.h"

using namespace std;

Dictionary::Dictionary() {
    capacity = INITIAL_CAPACITY;

    table = new Entry[capacity];
    Entry e = {'?', 0, "NONE"};
    for (int i = 0; i < capacity; i++) {
        table[i] = e;
    }
    numWords = 0;
}

Dictionary::Dictionary(string filename) {
    capacity = INITIAL_CAPACITY;
    
    table = new Entry[capacity];
    Entry e = {'?', 0, "NONE"};
    for (int i = 0; i < capacity; i++) {
        table[i] = e;
    }
    numWords = 0;
    openAndParseFile();
}
Dictionary::~Dictionary() {
    delete [] table;
    table = nullptr;
    numWords = 0;
}
/* 
 * openAndParseFile()
 * Purpose: opens and parses file
 * Parameters: filename
 * Returns: none
 */
void Dictionary::openAndParseFile(string filename){
    infile.open(filename);
    if (!infile.is_open()) {
        cerr << "Error: could not open " << filename << "\n";
    }
    parseFile();
    infile.close();
}
void Dictionary::parseFile() {
    string input = "";
    while(cin >> input) {
        Entry toAdd = {input[0], input.length(), input};
        table[linearProbeIndex(toAdd.letter, hashIndex(toAdd.letter))] = toAdd;
        numWords++;
    }
}
int Dictionary::size() {
    return numWords;
}

char Dictionary::getChar(int index) {
    return table[index].letter;
}

int Dictionary::getLength(int index) {
    return table[index].length;
}

string Dictionary::getWord(int index) {
    return table[index].word;
}
/** *****************************************************************\
*                   hashtable maintaining functions                  *
\********************************************************************/
/* expand()
 * parameters: none
 * purpose: expands the capacity and rehashes all cities
 * returns: n/a
 */
void Dictionary::expand()
{
    int        oldCapacity = capacity;
    Entry *oldTable        = table; // save old table to delete it

    capacity = 2 * capacity + 2;        // update capacity
    table    = new Entry[capacity]; // create new table
    Entry e  = {'?', 0, "NONE"};
    for (int i = 0; i < capacity; i++) {
        table[i] = e;
    }
    for (int i = 0; i < oldCapacity; i++) {
        if (oldTable.getChar(i) != '?') { // if keywrd empty: continue,
            Entry curr = {oldTable[i].char, oldTable[i].length, 
                          oldTable[i].word}; 
            char  key  = curr.letter; // & put into new table

            // linear probe index uses new table
            table[linearProbeIndex(key, hashIndex(key))] = curr;
        }
    }
    delete [] oldTable;
}
/* loadFactorCheck()
 * parameters: none
 * purpose: checks the load factor and rehashes if necessary
 * returns: n/a
 */
void Dictionary::loadFactorCheck()
{
    double c  = capacity * 1.0;
    double nc = numCities * 1.0;
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
int Dictionary::linearProbeIndex(char key, int index)
{
    int  attempt = 0;
    bool found   = false;
    while (!found) {
        index = (index + attempt) % capacity;
        if (table.getChar(index) == '?') { 
            return index;
        } else if (table.getChar(index) == key) {
            return index;
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
int Dictionary::hashIndex(char key)
{
    // first get hash value for key
    // then mod by capacity to get index
    return (hash<char>{}(key) % capacity);
}