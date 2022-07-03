/* 
 * Dictionary.h
 * by: Valerie Zhang
 *
 * Purpose: The Dictionary class stores all words that can be used to solve
 *          the crossword
 */
#ifndef _DICTIONARY_H_
#define _DICTIONARY_H_

#include <fstream>
#include <iostream>
#include <functional>
#include <string>
#include <vector>
#include <utility>

using namespace std;

static const int INITIAL_CAPACITY = 100;

class Dictionary {
public:
    /* constructors and destructor */
    Dictionary();
    Dictionary(string filename);
    ~Dictionary();

    /* getters */
    int size();
    int getDomain(int key);
    string getWord(int numIndex, int wordIndex);

    void openAndParseFile(string filename);

private:

    struct Entry {
        bool empty;
        int length;
        vector<string> words;
    };

    int capacity;
    int numWords;
    Entry *table;

    ifstream infile;

    /* setting up dictionary */
    void parseFile();
    /* maintaining the dictionary */
    void expand();
    void loadFactorCheck();

    /* hash functions */
    int hashIndex(int key);
    int linearProbeIndex(int key, int index);
};

#endif