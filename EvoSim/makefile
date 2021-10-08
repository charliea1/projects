CXX = g++

all: bin/.dirstamp simulator

simulator: src/Simulator.cpp bin/game.o bin/ent.o
	$(CXX) $^ -o $@

bin/game.o: src/GameState.cpp lib/GameState.h lib/Entity.h
	$(CXX) -c $< -o $@

bin/ent.o: src/Entity.cpp lib/Entity.h
	$(CXX) -c $< -o $@

bin/.dirstamp:
	mkdir -p bin
	touch bin/.dirstamp

.PHONY: clean
clean:
	rm -rf bin
	