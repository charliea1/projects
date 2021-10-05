#include "../lib/IfStatement.h"

IfStatement::IfStatement(std::string var, std::string operation, std::string compared, int line)
        : m_var(var), m_operation(operation), m_compared(compared), m_line(line) {}

void IfStatement::execute(ProgramState* state, std::ostream& outf) {
    int left = state->readVar(m_var);
    int right = state->readVar(m_compared);
    bool result = false;

    // determine and perform operation
    // <
    if (m_operation == "<") {
        // evaluate comparison
        if (left < right) {
            result = true;
        }
    }
    // <=
    else if (m_operation == "<=") {
        // evaluate comparison
        if (left <= right) {
            result = true;
        }
    }
    // >
    else if (m_operation == ">") {
        // evaluate comparison
        if (left > right) {
            result = true;
        }
    }
    // >=
    else if (m_operation == ">=") {
        // evaluate comparison
        if (left >= right) {
            result = true;
        }
    }
    // =
    else if (m_operation == "=") {
        // evaluate comparison
        if (left == right) {
            result = true;
        }
    }
    // <>
    else if (m_operation == "<>") {
        // evaluate comparison
        if (left != right) {
            result = true;
        }
    }

    // if comparison is true, jump to line by setting counter
    if (result) {
        state->setCounter(m_line);
    }
    // otherwise it is false, continue to next line
    else {
        state->incrementCounter();
    }
}
