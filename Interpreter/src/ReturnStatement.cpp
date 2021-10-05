#include "../lib/ReturnStatement.h"

ReturnStatement::ReturnStatement() {}

void ReturnStatement::execute(ProgramState* state, std::ostream& outf) {
    // if returns stack is empty, treat like End Statement
    if (state->emptyStack()) {
        // set counter to -1, terminating program
        state->setCounter(-1);
        return;
    }

    // if not empty, peek/pop, set counter to popped line
    int line = state->popStack();
    state->setCounter(line);
}