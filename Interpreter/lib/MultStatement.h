#include "ProgramState.h"
#include "Statement.h"

class MultStatement : public Statement {
private:
    // variable on which operation is performed
    std::string m_var;

    // variable/int in string form to be multiplied
    std::string m_factor;

public:
    MultStatement(std::string var, std::string factor);
    virtual void execute(ProgramState* state, std::ostream& outf);
};