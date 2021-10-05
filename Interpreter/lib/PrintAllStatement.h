#include "ProgramState.h"
#include "Statement.h"

class PrintAllStatement : public Statement {
public:
    PrintAllStatement();
    virtual void execute(ProgramState* state, std::ostream& outf);
};