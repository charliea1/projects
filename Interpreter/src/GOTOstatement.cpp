#include "../lib/GOTOstatement.h"

GOTOstatement::GOTOstatement(int line) : m_line(line) {}

void GOTOstatement::execute(ProgramState* state, std::ostream& outf) {
    // set counter to line-->it will be next statement executed
    state->setCounter(m_line);
}
