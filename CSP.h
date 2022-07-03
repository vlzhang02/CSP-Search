/* 
 * CSP.h
 * by: Valerie Zhang
 *
 * Purpose: The CSP class takes in crossword puzzle files with information  
 *          about the grid and solves the puzzle. 
 */
#ifndef _CSP_H_
#define _CSP_H_

#include "Dictionary.h"
#include "WVTable.h"
#include <ios>
#include <vector>

class CSP {

public:
    CSP();
    CSP(string gridFile, string wordsFile);
    ~CSP();

    void run();
    void printCrossword();

private:
    Dictionary ref;
    WVTable vars;
    int rows;
    int cols;
    char **crossword; // crossword grid
    int numConstraints;
    int recursiveCalls;
    vector<int> across;
    vector<int> down;
    ifstream ifile;

    void openAndParse(string filename);
    void parseFile();

    void setRC();
    void addVar(string input, int row, int col);
    void addAcross(string input, int col);
    void addDown(int row);
    void removeDownX(int col);
    void addConstraints(int row, int col);
    
    void backtrackingSearch();
    bool searchHelper(wordVar *prev);
    bool constraintCheck(wordVar *curr, string value);
    void setCrossword(wordVar *curr, string value);
    
};
#endif