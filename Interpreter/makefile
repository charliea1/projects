BIN_DIR = bin
CXX = g++
CPPFLAGS = -Wall -g

all: $(BIN_DIR)/.dirstamp interpreter

interpreter: src/Interpreter.cpp $(BIN_DIR)/progState.o $(BIN_DIR)/let.o $(BIN_DIR)/print.o $(BIN_DIR)/gosub.o $(BIN_DIR)/return.o\
	$(BIN_DIR)/printAll.o $(BIN_DIR)/add.o $(BIN_DIR)/goto.o $(BIN_DIR)/end.o $(BIN_DIR)/mult.o $(BIN_DIR)/div.o $(BIN_DIR)/if.o
	$(CXX) $(CPPFLAGS) $^ -o $@

$(BIN_DIR)/progState.o:  src/ProgramState.cpp lib/ProgramState.h lib/Statement.h
	$(CXX) $(CPPFLAGS) -c $< -o $@

$(BIN_DIR)/let.o: src/LetStatement.cpp lib/LetStatement.h lib/Statement.h
	$(CXX) $(CPPFLAGS) -c $< -o $@

$(BIN_DIR)/print.o: src/PrintStatement.cpp lib/PrintStatement.h lib/Statement.h
	$(CXX) $(CPPFLAGS) -c $< -o $@

$(BIN_DIR)/printAll.o: src/PrintAllStatement.cpp lib/PrintAllStatement.h lib/Statement.h
	$(CXX) $(CPPFLAGS) -c $< -o $@

$(BIN_DIR)/add.o: src/AddStatement.cpp lib/AddStatement.h lib/Statement.h
	$(CXX) $(CPPFLAGS) -c $< -o $@

$(BIN_DIR)/goto.o: src/GOTOstatement.cpp lib/GOTOstatement.h lib/Statement.h
	$(CXX) $(CPPFLAGS) -c $< -o $@

$(BIN_DIR)/end.o: src/EndStatement.cpp lib/EndStatement.h lib/Statement.h
	$(CXX) $(CPPFLAGS) -c $< -o $@

$(BIN_DIR)/mult.o: src/MultStatement.cpp lib/MultStatement.h lib/Statement.h
	$(CXX) $(CPPFLAGS) -c $< -o $@

$(BIN_DIR)/div.o: src/DivStatement.cpp lib/DivStatement.h lib/Statement.h
	$(CXX) $(CPPFLAGS) -c $< -o $@

$(BIN_DIR)/if.o: src/IfStatement.cpp lib/IfStatement.h lib/Statement.h
	$(CXX) $(CPPFLAGS) -c $< -o $@

$(BIN_DIR)/gosub.o: src/GOSUBStatement.cpp lib/GOSUBStatement.h lib/Statement.h
	$(CXX) $(CPPFLAGS) -c $< -o $@

$(BIN_DIR)/return.o: src/ReturnStatement.cpp lib/ReturnStatement.h lib/Statement.h
	$(CXX) $(CPPFLAGS) -c $< -o $@

$(BIN_DIR)/.dirstamp:
	mkdir -p $(BIN_DIR)
	touch $(BIN_DIR)/.dirstamp

.PHONY: clean
clean:
	rm -rf $(BIN_DIR)