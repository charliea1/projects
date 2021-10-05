#include "../lib/EndStatement.h"

EndStatement::EndStatement() {}

void EndStatement::execute(ProgramState* state, std::ostream& outf) {
    // set counter to -1, terminating program
    state->setCounter(-1);
}