#include "ProgramState.h"
#include "Statement.h"

class DivStatement : public Statement {
private:
    // variable on which operation is performed
    std::string m_var;

    // variable/int in string form to be divided by
    std::string m_divisor;

public:
    DivStatement(std::string var, std::string divisor);
    virtual void execute(ProgramState* state, std::ostream& outf);
};