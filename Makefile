###
### Makefile for Constraint Satisfaction Solver
###
### Author: Valerie Zhang
###

CXX      = clang++
CXXFLAGS = -g3 -Ofast -Wall -Wextra -std=c++11 


Dictionary.o: Dictionary.cpp
	$(CXX) $(CXXFLAGS) -c $^

unit_test: unit_test_driver.o Dictionary.o 
	$(CXX) $(CXXFLAGS) $^

clean: 
	rm *.o a.out *~ *#