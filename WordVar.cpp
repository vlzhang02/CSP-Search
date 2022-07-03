/*
 * WordVar.cpp
 * by: Valerie Zhang
 *
 * An implementation of the WordVar class. The WordVar class can be used to 
 * store information about a crossword word variable.
 */

#include "WordVar.h"

using namespace std;
/*
 * default constructor
 */
wordVar::wordVar() {
    num = domain = 0;
    value = "NO_VALUE";
    orientation= false;
    start = {0,0};
}
/*
 * secondary constructor
 * Purpose:     initializes wordVar instance with num, orientation, and start 
 *              coordinates
 */
wordVar::wordVar(int n, bool direction, int row, int col) {
    num = n;
    value = "NO_VALUE";
    domain = length = 0;
    orientation = direction;
    start = {row, col};
}
/*
 * copy constructor
 */
wordVar &wordVar::operator=(const wordVar &other) {
    if (this == &other) {return *this;}
    constraints.clear(); // clear this' constraints
    // initialize variables
    num = other.num;
    value = other.value;
    domain = other.domain;
    length = other.length;
    orientation = other.orientation;
    start = other.start;
    // copy all constraints
    for (size_t i = 0; i < other.constraints.size(); i++) {
        constraints.push_back(other.constraints[i]);
    }
    return *this;
}
/*
 * destructor
 */
wordVar::~wordVar() {
    constraints.clear();
}
/*
 * getNum()
 * Purpose:     return variable's number
 * Parameters:  none
 * Returns:     variable's number
 */
int wordVar::getNum () {
    return num;
}
/*
 * getDomain()
 * Purpose:     return variable's domain
 * Parameters:  none
 * Returns:     variable's domain
 */
int wordVar::getDomain() {
    return domain;
}
/*
 * getLength()
 * Purpose:     return variable's length
 * Parameters:  none
 * Returns:     variable's length
 */
int wordVar::getLength() {
    return length;
}
/*
 * getValue()
 * Purpose:     return variable's assigned value
 * Parameters:  none
 * Returns:     variable's assigned value
 */
string wordVar::getValue() {
    return value;
}
/*
 * getOrientation()
 * Purpose:     return variable's orientation
 * Parameters:  none
 * Returns:     variable's orientation
 */
bool wordVar::getOrientation() {
    return orientation;
}
/*
 * getStart()
 * Purpose:     return variable's start coordinates
 * Parameters:  none
 * Returns:     variable's start coordinates
 */
pair<int, int> wordVar::getStart() {
    return start;
}
/*
 * addConstraint()
 * Purpose:     add constraint to list
 * Parameters:  num and direction of other variable, and the indices where
 *              this and the other variable intersects 
 * Returns:     none
 */
void wordVar::addConstraint(int num, bool orientation, int index1, int index2) {
    constraint toAdd;
    toAdd.num = num;
    toAdd.orientation = orientation;
    toAdd.thisIndex = index1;
    toAdd.otherIndex = index2;
    constraints.push_back(toAdd);
}
/*
 * numConstraints()
 * Purpose:     return total number of constraints
 * Parameters:  none
 * Returns:     total number of constraints
 */
int wordVar::numConstraints() {
    return constraints.size();
}
/*
 * getConstraintThisIndex()
 * Purpose:     return this variable's intersection index
 * Parameters:  constraint index
 * Returns:     index of intersection
 */
int wordVar::getConstraintThisIndex(int i) {
    return constraints[i].thisIndex;
}
/*
 * getConstraintOtherIndex()
 * Purpose:     return other variable's intersection index
 * Parameters:  constraint index
 * Returns:     index of intersection
 */
int wordVar::getConstraintOtherIndex(int i) {
    return constraints[i].otherIndex;
}
/*
 * getConstraintNum()
 * Purpose:     return constraining variable's num
 * Parameters:  constraint index
 * Returns:     constraining variable's num
 */
int wordVar::getConstraintNum(int index) {
    return constraints[index].num;
}
/*
 * getConstraingOrientation()
 * Purpose:     return constraining variable's orientation
 * Parameters:  constraint index
 * Returns:     constraining variable's direction
 */
bool wordVar::getConstraintOrientation(int index) {
    return constraints[index].orientation;
}
/*
 * setDomain()
 * Purpose:     set wordVar's domain
 * Parameters:  domain
 * Returns:     none
 */
void wordVar::setDomain(int num) {
    domain = num;
}
/*
 * setDomain()
 * Purpose:     set wordVar's value
 * Parameters:  word to assign
 * Returns:     none
 */
void wordVar::setVal(string s) {
    value = s;
}
/*
 * addLength()
 * Purpose:     increments length by 1
 * Parameters:  none
 * Returns:     none
 */
void wordVar::addLength() {
    length++;
}
/*
 * print()
 * Purpose:     print variable's numnber, direction, domain, and value
 * Parameters:  none
 * Returns:     none
 */
void wordVar::print() {
    cerr << num;
    if (orientation == true) {
        cerr << "-down = ";
    } else {
        cerr << "-across = ";
    }
    cerr << value << "(" << domain << " values possible)\n";
}

