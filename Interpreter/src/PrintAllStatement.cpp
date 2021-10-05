#include "../lib/PrintAllStatement.h"

PrintAllStatement::PrintAllStatement() {}

void PrintAllStatement::execute(ProgramState* state, std::ostream& outf) {
    state->printVars(outf);

    // move to next line
    state->incrementCounter();
}