/*
 * CSP.cpp
 * by: Valerie Zhang
 *
 * Purpose: The CSP class solves user provided crossword grids using a 
 *          provided dictionary of possible words
 *
 */ 
#include "CSP.h"

using namespace std;
/*
 * default constructor
 */
CSP::CSP() {
    rows = cols = 10;
    numConstraints = recursiveCalls = 0;
    crossword = new char*[rows];
    for (int i = 0; i < rows; i++) {
        crossword[i] = new char[cols];
        for (int j = 0; j < cols; j++) {
            crossword[i][j] = '0';
        }
    }
}
/*
 * secondary constructor
 * Purpose: initializes CSP instance with files
 */
CSP::CSP(string gridFile, string wordsFile) {
    numConstraints = recursiveCalls = 0;
    ref.openAndParseFile(wordsFile);
    openAndParse(gridFile);
}
/*
 * destructor
 */
CSP::~CSP() {
    for (int i = 0; i < rows; i++) {
        delete [] crossword[i];
    }
    delete [] crossword;
    crossword = nullptr;
    rows = cols = 0;
}
/*******************************************************************\
*                       initialization functions                    *
\*******************************************************************/

/* 
 * openAndParseFile()
 * Purpose:     opens and parses file
 * Parameters:  filename
 * Returns:     none
 */
void CSP::openAndParse(string filename){
    ifile.open(filename);
    if (!ifile.is_open()) {
        cerr << "Error: could not open " << filename << "\n";
    }
    parseFile();
    ifile.close();
}
/*
 * parseFile()
 * Purpose:     parse grid file and initialize grid
 * Parameters:  none
 * Returns:     none
 */
void CSP::parseFile() {
    setRC(); // get # of rows and columns
    string input;
    int rowCount = 0;
    int colCount = 0;
    while (ifile >> input) {
        if (input == "X") {
            crossword[rowCount][colCount] = '*';
            removeDownX(colCount); // checks for variables who's lengths are done
        } else if (input == "_") {
            crossword[rowCount][colCount] = '_';
        } else {
            addVar(input, rowCount, colCount); // add word variable to table
            crossword[rowCount][colCount] = '#';
        }
        addConstraints(rowCount, colCount); // add constraints to variables
        addAcross(input, colCount); // increment lengths of across variables
        colCount++; 
        if (colCount == cols) {
            colCount = 0;
            addDown(rowCount); // increments length of down variables
            rowCount++;
        }
    }
}
/*
 * setRC()
 * Purpose:     gets number of rows and cols from file and creates new grid on
 *              the heap
 * Parameters:  none
 * Returns:     none
 */
void CSP::setRC() {
    string input;
    ifile >> input;
    rows = stoi(input);
    crossword = new char*[rows];
    ifile >> input;
    cols = stoi(input);
    for (int j = 0; j < rows; j++) {
        crossword[j] = new char[cols];
    }
}
/*
 * addVar()
 * Purpose:     creates new variable and adds to dictionary
 * Parameters:  string input, row # and col #
 * Returns:     none
 *
 */
void CSP::addVar(string input, int row, int col) {
    bool direction;
    if ((row == 0) or (crossword[row-1][col] == '*')) { // down var
        direction = true; 
        down.push_back(stoi(input)); // push onto down-var length check list
        vars.addVar(stoi(input), direction, row, col); // add to table
    } 
    if ((col == 0) or (crossword[row][col-1] == '*')) { // across var
        direction = false;
        across.push_back(stoi(input)); // push onto across-var length check list
        vars.addVar(stoi(input), direction, row, col); // add to table
    }

}
/* 
 * printCrossword()
 * Purpose:     prints crossword grid
 * Parameters:  none
 * Returns      none
 */
void CSP::printCrossword() {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (crossword[i][j] == '*') {
                cout << " ";
            } else {
                cout << crossword[i][j];
            }
        }
        cout << "\n";
    }
    cout << "\n";
}
/*
 * addAcross()
 * Purpose:     increments length of across variables
 * Parameters:  current input, rol, and col
 * Returns:     none
 */
void CSP::addAcross(string input, int col) {
    for (size_t i = 0; i < across.size(); i++) {
        if ((input == "X") or (col == cols-1)) {
            if (col == cols-1){
                // additional incrementation for length
                vars.getWordEntry(across[i], false)->addLength();
            }
            wordVar *w = vars.getWordEntry(across[i], false);
            w->setDomain(ref.getDomain(w->getLength()));
            across.erase(across.begin() + i); // remove from checking list
            i--;
        } else {
            vars.getWordEntry(across[i], false)->addLength(); // incremenent 
        }
    }
}
/*
 * addDown()
 * Purpose:     increments length of down variables
 * Parameters:  current input, rol, and col
 * Returns:     none
 */
void CSP::addDown(int row) {
    for (size_t i = 0; i < down.size(); i++) {
        if (row == rows-1) {
            // additional incrementation for length
            wordVar *w = vars.getWordEntry(down[i], true);
            w->addLength();
            w->setDomain(ref.getDomain(w->getLength()));
            down.erase(down.begin() + i); // remove from checking list
            i--;
        } else {
            vars.getWordEntry(down[i], true)->addLength(); // increment
        }
    }
}
/*
 * removeDown()
 * Purpose:     removes down variables from checking list when input is "X"
 * Parameters:  current col #
 * Returns:     none
 */
void CSP::removeDownX(int col) {
    for (size_t i = 0; i < down.size(); i++)
    if (vars.getWordEntry(down[i], true)->getStart().second == col) {
        wordVar *w = vars.getWordEntry(down[i], true);
        w->setDomain(w->getLength());
        down.erase(down.begin() + i); // remove from checking list
        i--;
    }
}
/*
 * addConstraints()
 * Purpose:     add constraint to intersection down/across vars to reference
 *              table
 * Parameters:  current row and col
 * Returns:     none
 */
void CSP::addConstraints(int row, int col) {
    for (size_t i = 0; i < across.size(); i ++) {
        for (size_t j = 0; j < down.size(); j ++) {
            if (vars.getWordEntry(down[j], true)->getStart().second == col) {
                wordVar *a = vars.getWordEntry(across[i], false);
                // add constraint to across word variable
                a->addConstraint(down[j], true, a->getLength(), row); 
                // add constraint to down word variable
                vars.getWordEntry(down[j], true)->addConstraint(across[i],
                false, row, a->getLength());
                numConstraints++;
            }
        }
    }
}
/******************************************************************\
*                            run functions                         *
\******************************************************************/
/*
 * run()
 * Purpose:     run backtracking search to solve the crossword
 * Parameters:  none
 * Returns:     none
 */
void CSP::run() {
    cout << vars.size() << " words\n" << numConstraints << " constraints\n\n";
    cout << "Initial assignment and domain sizes:\n";
    vars.printDomainandVal();
    cerr << "\n";
    backtrackingSearch();
}
/******************************************************************\
*                          search functions                        *
\******************************************************************/
/*
 * backtrackingSearch()
 * Purpose:     perform backtracking search and print corresponding messages
 * Parameters:  none
 * Returns:     none
 */
void CSP::backtrackingSearch() {
    if (searchHelper(nullptr)) {
        cout << "SUCCESS! Solution found after " << recursiveCalls; 
        cout << " recursive calls to search.\n\n";
        printCrossword();
    } else {
        cout << "FAIL: No solution found after " << recursiveCalls << " recursive";
        cout << " calls to search.\n\n";
    }
}
/*
 * searchHelper()
 * Purpose:     recursively searches for solution to crossword
 * Parameters:  pointer to previous word variable assigned
 * Returns:     true if solution is found, false if not
 */
bool CSP::searchHelper(wordVar *prev) {
    recursiveCalls++;
    if (vars.complete()) {
        return true;
    } else {
        wordVar *curr = vars.selectVariable(prev); // select variable
        for (int i = 0; i < ref.getDomain(curr->getLength()); i++) {
            string value = ref.getWord(curr->getLength(), i); // get possible val
            if (constraintCheck(curr, value)) {
                curr->setVal(value); // assigns value in table
                setCrossword(curr, value); // sets value in grid
                if (!searchHelper(curr)) {
                    curr->setVal("NO_VALUE"); // remove assignment
                } else {return true;}
            }
        }
        return false; // when value cannot be assigned
    }
}
/*
 * constraintCheck()
 * Purpose:     checks if word follows all constraints
 * Parameters:  string value and pointer to word variable being assigned
 * Returns:     true if value follows all constraints, false if not
 */
bool CSP::constraintCheck(wordVar *curr, string value) {
    for (int j = 0; j < curr->numConstraints(); j++) { // check all constraints
        wordVar *w = vars.getWordEntry(curr->getConstraintNum(j), 
                                       curr->getConstraintOrientation(j)); 
        if (w->getValue() != "NO_VALUE") { // only check already assigned variables
            string compare = w->getValue();
            if (value[curr->getConstraintThisIndex(j)] != 
                compare[curr->getConstraintOtherIndex(j)]) {
                return false; // if characters don't match(constraint violated)
            }
        }
    }
    return true;
}
/*
 * setCrossword()
 * Purpose:     fills crossword with a word variable's assigned value 
 * Parameters:  assigned value and pointer to word variable 
 * Returns:     none
 */
void CSP::setCrossword(wordVar *curr, string value) {
    int valIndex = 0;
    pair<int, int> start = curr->getStart();
    if (curr->getOrientation() == true) { // loop for down variables
        for (int i = start.first; i < curr->getLength(); i++) {
            crossword[i][start.second] = value[valIndex];
            valIndex++;
        }
    } else { // loop for across variable
        for (int i = start.second; i < curr->getLength(); i++) {
            crossword[start.first][i] = value[valIndex];
            valIndex++;
        }
    }
}