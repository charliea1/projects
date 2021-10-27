FLAGS = -Wall -g

all: doublet

doublet: doublet.cpp MinHeap.h
	g++ $(FLAGS) $< -o $@

.PHONY: clean
clean:
	rm doublet
	rm -r doublet.dSYM