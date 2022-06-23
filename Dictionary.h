/* 
 * Dictionary.h
 * by: Valerie Zhang
 *
 * 
 */
#ifndef _DICTIONARY_H_
#define _DICTIONARY_H_

#include <fstream>
#include <functional>
#include <string>
#include <vector>

using namespace std;

static const int INITIAL_CAPACITY = 1000;

class Dictionary {
public:
    /* constructors and destructor */
    Dictionary();
    Dictionary(string filename);
    ~Dictionary();

    /* getters */
    int size();

    char getChar(int index);
    int getLength(int index);
    string getWord(int index);


private:
    struct wordInfo {
        string word;
        int length;
    };

    struct Entry {
        char letter;
        vector<wordInfo> words;
    };

    int capacity;
    int numWords;
    Entry *table;

    ifstream infile;

    /* setting up dictionary */
    void openAndParseFile(string filename);
    void parseFile();
    /* maintaining the dictionary */
    void expand();
    void loadFactorCheck();

    /* hash functions */
    int hashIndex(string key);
    int linearProbeIndex(string key, int index);
};

#endif