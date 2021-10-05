#include "ProgramState.h"
#include "Statement.h"

class AddStatement : public Statement {
private:
    // variable on which operation is performed
    std::string m_var;

    // variable/int in string form to be added
    std::string m_added;

    // boolean to track for substitution
    bool m_sub;

public:
    AddStatement(std::string var, std::string added, bool sub);
    virtual void execute(ProgramState* state, std::ostream& outf);
};