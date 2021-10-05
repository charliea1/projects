#include "../lib/AddStatement.h"
using namespace std;

AddStatement::AddStatement(std::string var, std::string added, bool sub) : m_var(var), m_added(added), m_sub(sub) {}

void AddStatement::execute(ProgramState* state, std::ostream& outf) {
    int toAdd;
    // read in var/int and lookup/cast appropriately
    int original = state->readVar(m_var);
    toAdd = state->readVar(m_added);

    // check for subtraction flag
    if (m_sub) {
        toAdd = -(toAdd);
    }

    // perform addition
    int sum = original + toAdd;
    state->updateVar(m_var, sum);

    // move to next line
    state->incrementCounter();
}
