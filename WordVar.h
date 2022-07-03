/*
 * WordVar.h
 * by: Valerie Zhang
 *
 * Purpose: The WordVar class stores all the information for a word variable
 *
 */
#ifndef _WORDVAR_H_
#define _WORDVAR_H_

#include <iostream>
#include <string>
#include <vector>
#include <utility>

using namespace std;

class wordVar {
public:
    wordVar();
    wordVar(int num, bool direction, int row, int col);
    ~wordVar();
    wordVar &operator=(const wordVar &other);

    void addLength();
    void setDomain(int num);
    void setVal(string s);
    
    int getNum();
    int getDomain();
    int getLength();
    string getValue();
    bool getOrientation();
    pair<int, int> getStart();

    int numConstraints();
    int getConstraintThisIndex(int i);
    int getConstraintOtherIndex(int i);
    int getConstraintNum(int index);
    bool getConstraintOrientation(int index);

    void addConstraint(int nume, bool orientation, int index1, int index2);
    void print();

private:
    struct constraint {
        int num;
        bool orientation; // true or false
        int thisIndex;
        int otherIndex; 
    };

    int num;
    string value;
    int domain;
    int length;
    bool orientation; // true if down, false if across
    pair<int, int> start; 
    vector<constraint> constraints;
};
#endif