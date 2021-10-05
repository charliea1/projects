#include "ProgramState.h"
#include "Statement.h"

class GOTOstatement : public Statement {
public:
    GOTOstatement(int line);
    virtual void execute(ProgramState* state, std::ostream& outf);

private:
    int m_line;
};
