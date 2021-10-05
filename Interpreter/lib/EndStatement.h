#include "ProgramState.h"
#include "Statement.h"

class EndStatement : public Statement {
public:
    EndStatement();
    virtual void execute(ProgramState* state, std::ostream& outf);
};