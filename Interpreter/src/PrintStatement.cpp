// PrintStatement.cpp:
#include "../lib/PrintStatement.h"

PrintStatement::PrintStatement(std::string variableName) : m_variableName(variableName) {}

void PrintStatement::execute(ProgramState* state, std::ostream& outf) {
    // if uninitialized, set to 0
    if (!(state->verify(m_variableName))) {
        state->updateVar(m_variableName, 0);
    }
    // print value
    outf << state->lookUp(m_variableName) << std::endl;
    // move to next line
    state->incrementCounter();
}
