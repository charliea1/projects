#include "../lib/GOSUBStatement.h"

GOSUBStatement::GOSUBStatement(int line) : m_line(line) {}

void GOSUBStatement::execute(ProgramState* state, std::ostream& outf) {
    // push line to return to (counter + 1) to returns stack
    int returned = state->getCounter();
    returned++;
    state->pushStack(returned);

    // set counter to line-->it will be next statement executed
    state->setCounter(m_line);
}
