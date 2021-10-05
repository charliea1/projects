BASIC Interpreter: Read in BASIC commands in .txt file, interpret and output to text or cout.

Compiling:
executing "make" from Interpreter directory:
-creates /bin directory for binary files
-creates executable "interpreter" (run with ./interpreter)

-individual targets can be compiled as seen in makefile (e.g. "make bin/print.o")
-clear and remove bin with "make clean"

Running:
-run executable with $./interpreter in Interpreter directory
-sample input file "sample.txt" included for testing
-".txt" input files must be in "src" directory, when prompted must be specified as "src/<filename>"
-output ofstream is cout by default;
to redirect output to file, input file name as first command line argument after executable
-input must be legal BASIC program numbered in order using supported statements listed below

Simple legal program example:
1 LET FOO 5
2 LET BAR 3
3 ADD FOO BAR
4 PRINTALL
5 END

Structure:
There are two main classes for this program: an abstract "Statement" class and "ProgramState". All statements in a BASIC file are represented by inherited Statement classes, e.g. LetStatement, PrintStatement, etc.
ProgramState manages variables, a program counter, and a stack to manage GOSUB/RETURN statements. Interpreter.cpp contains functions for parsing and running the program, which are called in its main() function.

Supported Statements: ADD, DIV, END, GOSUB, GOTO, IF, LET, MULT, PRINT, PRINTALL, RETURN, END ("." can replace "END")
