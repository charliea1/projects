CXX = g++

all: bin/.dirstamp simulator

simulator: src/Simulator.cpp bin/env.o bin/entity.o
	$(CXX) $^ -o $@

bin/env.o: src/Environment.cpp lib/Environment.h lib/Entity.h
	$(CXX) -c $< -o $@

bin/entity.o: src/Entity.cpp lib/Entity.h
	$(CXX) -c $< -o $@

bin/.dirstamp:
	mkdir -p bin
	touch bin/.dirstamp

.PHONY: clean
clean:
	rm -rf bin
	rm simulator
	