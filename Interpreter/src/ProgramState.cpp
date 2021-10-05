#include "../lib/ProgramState.h"

#include <iostream>

using namespace std;

ProgramState::ProgramState(int numlines) : m_numLines(numlines) { counter = 1; }

void ProgramState::updateVar(string name, int val) {
    // accessing nonexistent index in map will create a new variable
    // therefore this works to both update and create new variables
    this->vars[name] = val;
}

void ProgramState::setCounter(int line) { counter = line; }

void ProgramState::incrementCounter() { counter++; }

int ProgramState::getCounter() { return counter; }

int ProgramState::lookUp(string name) {
    // try to find name in map
    if (verify(name)) {
        return vars[name];
    }
    // if var not found, return 0
    else {
        return 0;
    }
}

// either find value of variable or cast to int
int ProgramState::readVar(string input) {
    int output;
    // if variable for input exists
    if (verify(input)) {
        // find input in map
        output = lookUp(input);
    }
    // else input must be an int or uninitialized var
    // try casting input to int--if it fails, initialize it as var to 0
    else {
        try {
            output = stoi(input);
        } catch (exception& err) {
            updateVar(input, 0);
            output = 0;
        }
    }
    return output;
}

bool ProgramState::verify(string variable) {
    // use iterator to search for variable in map
    map<string, int>::iterator it;
    it = vars.find(variable);
    // if it != end(), variable of name found-->return true
    if (it != vars.end()) {
        return true;
    }
    return false;
}

void ProgramState::printVars(std::ostream& outf) {
    // iterate through map and write var names and values to file
    map<string, int>::iterator it;
    for (it = this->vars.begin(); it != this->vars.end(); ++it) {
        outf << it->first << " " << it->second << std::endl;
    }
}

// push line number to returns stack
void ProgramState::pushStack(int line) { returns.push(line); }

// return top and pop from returns stack
int ProgramState::popStack() {
    int out = returns.top();
    returns.pop();
    return out;
}

bool ProgramState::emptyStack() {
    if (returns.empty()) {
        return true;
    }
    return false;
}
