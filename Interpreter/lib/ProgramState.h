// ProgramState.h
//
// CS 104 / Fall 2021
//
// The ProgramState class encapsulates the state of an executing Facile
// program.  The state of a BASIC program consists of three parts:
//
//    * The program counter, which specifies the line number of the
//      statement that will execute next.
//    * A map to store variable names and values
//    * A stack of integers, which is used to hold return lines for GOSUB
//      statements.

#include <map>
#include <stack>
#include <string>
using namespace std;

#ifndef PROGRAM_STATE_INCLUDED
#define PROGRAM_STATE_INCLUDED

class ProgramState
{
public:
    ProgramState(int numLines);

    // see if var exists and update or add it to map
    void updateVar(string name, int val);

    // return value of stored var--return 0 if not existent
    int lookUp(string name);

    // print all variable names and values to output file
    void printVars(std::ostream &outf);

    // check for existence of variable in map
    bool verify(string variable);

    // set program counter to specific line
    void setCounter(int line);

    // increment program counter by one
    void incrementCounter();

    // returns counter
    int getCounter();

    // either find value of variable or cast to int
    // useful for reading in arithmetic statements
    int readVar(string input);

    // push line number to returns stack
    void pushStack(int line);

    // return top and pop from returns stack
    int popStack();

    bool emptyStack();

private:
    int m_numLines;
    int counter;
    stack<int> returns;
    map<string, int> vars;
};

#endif
