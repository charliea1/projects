#include "ProgramState.h"
#include "Statement.h"

class GOSUBStatement : public Statement {
public:
    GOSUBStatement(int line);
    virtual void execute(ProgramState* state, std::ostream& outf);

private:
    int m_line;
};
