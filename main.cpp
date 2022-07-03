/* 
 * main.cpp
 * By: Valerie Zhang, June 2022
 *
 * Purpose: Solves crossword grid using provided list of words
 *
 */

#include "CSP.h"
#include <iostream>
#include <stdlib.h>

using namespace std;

int main(int argc, char *argv[]) {
    if (argc != 3) {
        cerr << "Usage: ./solver gridFile wordFile \n";
        exit(EXIT_FAILURE);
    }
    CSP c(argv[1], argv[2]);
    c.run();
    
    return 0;
}