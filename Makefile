###
### Makefile for Constraint Satisfaction Solver
###
### Author: Valerie Zhang
###

CXX      = clang++
CXXFLAGS = -g3 -Ofast -Wall -Wextra -std=c++11 

solver: main.o CSP.o WVTable.o WordVar.o Dictionary.o
	$(CXX) $(CXXFLAGS) -o $@ $^
CSP.o: CSP.cpp
	$(CXX) $(CXXFLAGS) -c $^

WVTable.o: WVTable.cpp
	$(CXX) $(CXXFLAGS) -c $^

WordVar.o: WordVar.cpp
	$(CXX) $(CXXFLAGS) -c $^

Dictionary.o: Dictionary.cpp
	$(CXX) $(CXXFLAGS) -c $^

unit_test: unit_test_driver.o Dictionary.o WordVar.o WVTable.o CSP.o
	$(CXX) $(CXXFLAGS) $^

clean: 
	rm *.o a.out *~ *#