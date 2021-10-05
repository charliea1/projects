// Interpreter.cpp
#include "../lib/AddStatement.h"
#include "../lib/DivStatement.h"
#include "../lib/EndStatement.h"
#include "../lib/GOSUBStatement.h"
#include "../lib/GOTOstatement.h"
#include "../lib/IfStatement.h"
#include "../lib/LetStatement.h"
#include "../lib/MultStatement.h"
#include "../lib/PrintAllStatement.h"
#include "../lib/PrintStatement.h"
#include "../lib/ProgramState.h"
#include "../lib/ReturnStatement.h"
#include "../lib/Statement.h"
#include <cstdlib>
#include <exception>
#include <fstream>
#include <map>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>

using namespace std;

// parseProgram() takes a filename as a parameter, opens and reads the
// contents of the file, and returns an vector of pointers to Statement.
void parseProgram(istream &inf, vector<Statement *> &program);

// parseLine() takes a line from the input file and returns a Statement
// pointer of the appropriate type
Statement *parseLine(string line);

// interpretProgram() reads a program from the given input stream
// and interprets it, writing any output to the given output stream.
void interpretProgram(istream &inf, ostream &outf);

int main(int argc, char *argv[])
{

    cout << "Enter BASIC program file name: ";
    string filename;
    getline(cin, filename);
    ifstream infile(filename.c_str());
    if (!infile)
    {
        cout << "Cannot open " << filename << "!" << endl;
        return 1;
    }
    //file setup for command line argument
    string ofile;
    if (argc > 1)
    {
        //read argument for filename

        ofile = argv[1];
        std::cout << ofile;

        // ofstream setup
        ofstream output(ofile, ofstream::out);

        //interpret program
        interpretProgram(infile, output);

        //close output file
        output.close();
    }
    //if no output file specified, use cout as default
    else
    {
        interpretProgram(infile, cout);
    }
}

void parseProgram(istream &inf, vector<Statement *> &program)
{
    program.push_back(NULL);

    string line;
    while (!inf.eof())
    {
        getline(inf, line);
        program.push_back(parseLine(line));
    }
}

Statement *parseLine(string line)
{
    Statement *statement;
    stringstream ss;
    string type;
    string var;
    int val;
    int lineNo;

    ss << line;
    ss >> lineNo;
    ss >> type;
    if (type == "LET")
    {
        ss >> var;
        ss >> val;
        // Note:  Because the project spec states that we can assume the file
        //	  contains a syntactically legal program, we know that
        //	  any line that begins with "LET" will be followed by a space
        //	  and then a variable and then an integer value.
        statement = new LetStatement(var, val);
    }

    // PRINTALL
    else if (type == "PRINTALL")
    {
        statement = new PrintAllStatement();
    }

    // PRINT
    else if (type == "PRINT")
    {
        ss >> var;
        statement = new PrintStatement(var);
    }

    // ADD/SUB
    // second input will be legal--either a variable or int
    else if ((type == "ADD") || (type == "SUB"))
    {
        string added;
        ss >> var;
        ss >> added;
        bool subCheck = false;

        // flag for subtraction if type sub
        if (type == "SUB")
        {
            subCheck = true;
        }

        statement = new AddStatement(var, added, subCheck);
    }

    // MULT
    else if (type == "MULT")
    {
        string multiplied;
        ss >> var;
        ss >> multiplied;

        statement = new MultStatement(var, multiplied);
    }

    // DIV
    else if (type == "DIV")
    {
        string divisor;
        ss >> var;
        ss >> divisor;

        statement = new DivStatement(var, divisor);
    }

    // GOTO
    else if (type == "GOTO")
    {
        int line;
        ss >> line;
        statement = new GOTOstatement(line);
    }

    // IF
    else if (type == "IF")
    {
        string ignore;
        string operation;
        string compared;
        int line;

        // variable being compared
        ss >> var;
        // operator
        ss >> operation;
        // comparison value
        ss >> compared;
        // ignore "THEN"
        ss >> ignore;
        // take conditional GOTO line
        ss >> line;

        statement = new IfStatement(var, operation, compared, line);
    }

    // GOSUB
    else if (type == "GOSUB")
    {
        // same as GOTO for input but executed differently
        int line;
        ss >> line;
        statement = new GOSUBStatement(line);
    }

    // RETURN
    else if (type == "RETURN")
    {
        statement = new ReturnStatement();
    }

    // END or "."
    else if ((type == "END") || (type == "."))
    {
        // delete allocated memory and close program
        statement = new EndStatement();
    }

    return statement;
}

void interpretProgram(istream &inf, ostream &outf)
{
    vector<Statement *> program;
    parseProgram(inf, program);

    // Incomplete;  TODO:  Finish this function!

    // remove leading and trailing null statements
    if (program.front() == nullptr)
    {
        program.erase(program.begin());
    }
    if (program.back() == nullptr)
    {
        program.pop_back();
    }

    // initialize new program state
    int lines = program.size();
    ProgramState *state;
    state = new ProgramState(lines);

    // while program counter not -1, use it to access line indices
    // by default increment counter by 1 at the end of each statement
    while (state->getCounter() != -1)
    {
        // statements indexed starting at 0 -> subract 1 from counter to execute correct line
        int currentLine = (state->getCounter()) - 1;
        program.at(currentLine)->execute(state, outf);
        // each statement will adjust counter accordingly
    }

    // memory cleanup
    for (int i = 0; i < program.size(); i++)
    {
        delete program[i];
        program[i] = nullptr;
    }
    delete state;
}
