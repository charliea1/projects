#include "../lib/DivStatement.h"
using namespace std;

DivStatement::DivStatement(std::string var, std::string divisor) : m_var(var), m_divisor(divisor) {}

void DivStatement::execute(ProgramState* state, std::ostream& outf) {
    int divisor;
    // read in var/int and lookup/cast appropriately
    divisor = state->readVar(m_divisor);
    int original = state->readVar(m_var);

    // deal with divisor = 0 exception
    if (divisor == 0) {
        outf << "Divide by zero exception" << endl;
        state->setCounter(-1);
        return;
    }

    // perform division
    int division = original / divisor;
    state->updateVar(m_var, division);

    // move to next line
    state->incrementCounter();
}
