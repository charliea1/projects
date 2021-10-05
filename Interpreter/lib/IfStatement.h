#include "ProgramState.h"
#include "Statement.h"

class IfStatement : public Statement {
private:
    // variable compared on
    std::string m_var;
    // comparison operation
    std::string m_operation;
    // compared value/var
    std::string m_compared;
    // line to conditionally GOTO
    int m_line;

public:
    IfStatement(std::string var, std::string operation, std::string compared, int line);
    virtual void execute(ProgramState* state, std::ostream& outf);
};