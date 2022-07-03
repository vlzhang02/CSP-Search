/*
 * Dictionary.cpp
 * by: Valerie Zhang
 *
 * Purpose: The Dictionary class stores all the words eligible to fill out the 
 *          crossword
 */

#include "Dictionary.h"

using namespace std;

/*
 * default constructor
 */
Dictionary::Dictionary() {
    capacity = INITIAL_CAPACITY;

    table = new Entry[capacity];
    Entry e;
    e.empty = true;
    for (int i = 0; i < capacity; i++) {
        table[i] = e;
    }
    numWords = 0;
}
/*
 * secondary constructor
 * Purpose: initializes Dictionary instance with words from a file
 */
Dictionary::Dictionary(string filename) {
    capacity = INITIAL_CAPACITY;
    
    table = new Entry[capacity];
    Entry e;
    e.empty = true;
    for (int i = 0; i < capacity; i++) {
        table[i] = e;
    }
    numWords = 0;
    openAndParseFile(filename);
}
/*
 * destructor
 */
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
/*
 * parseFile()
 * Purpose:     parses file for words and add them to the dictionary
 * Parameters:  none
 * Returns:     none
 */
void Dictionary::parseFile() {
    string input = "";
    while(getline(infile, input)) {
        int length = input.length();
        int index = linearProbeIndex(length, hashIndex(length));
        table[index].length = length;
        table[index].empty = false;
        table[index].words.push_back(input);
        numWords++;
    }
}
/*
 * size()
 * Purpose:     get the number of words in the dictionary
 * Parameters:  none
 * Returns:     total number of words in dictionary
 */
int Dictionary::size() {
    return numWords;
}
/*
 * getDomain()
 * Purpose:     get number of words of a certain length
 * Parameters:  length of words
 * Returns:     number of words of specified length
 */
int Dictionary::getDomain(int key) {
    return table[linearProbeIndex(key, hashIndex(key))].words.size();
}

string Dictionary::getWord(int numIndex, int wordIndex) {
    return table[numIndex].words[wordIndex];
}
/********************************************************************\
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
    Entry e;
    for (int i = 0; i < capacity; i++) {
        table[i] = e;
    }
    for (int i = 0; i < oldCapacity; i++) {
        if (!oldTable[i].empty) { // if entry empty: continue,
            int key = oldTable[i].length;
            int index = linearProbeIndex(key, hashIndex(key));
            table[index].length = key;
            table[index].empty = false;
            for (size_t j = 0; j < table[index].words.size(); j++) {
                table[index].words.push_back(oldTable[i].words[j]);
            }
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
    double nc = numWords * 1.0;
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
int Dictionary::linearProbeIndex(int key, int index)
{
    int  attempt = 0;
    bool found   = false;
    while (!found) {
        index = (index + attempt) % capacity;
        if (table[index].empty) { 
            return index;
        } else if (table[index].length == key) {
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
int Dictionary::hashIndex(int key)
{
    // first get hash value for key
    // then mod by capacity to get index
    return (hash<int>{}(key) % capacity);
}
