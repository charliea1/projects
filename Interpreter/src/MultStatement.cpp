#include "../lib/MultStatement.h"
using namespace std;

MultStatement::MultStatement(std::string var, std::string factor) : m_var(var), m_factor(factor) {}

void MultStatement::execute(ProgramState* state, std::ostream& outf) {
    int factor;
    // read in var/int and lookup/cast appropriately
    factor = state->readVar(m_factor);
    int original = state->readVar(m_var);

    // perform multiplication
    int product = original * factor;
    state->updateVar(m_var, product);

    // move to next line
    state->incrementCounter();
}
