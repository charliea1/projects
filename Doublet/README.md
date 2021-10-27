MinHeap.h -- templated d-ary minheap using "qitem" struct, has update functionality to allow for use as indexed priority queue

doublet.cpp -- takes start, target words of equal length, followed by text file as command line arguments

-performs "doublet" game on start, target words using word list in text file as "nodes" in a graph

-utilizes A-star search with number of letters not matching target as heuristic for each word

-outputs -steps- \n -expansions- (steps = distance of path, expansions = number of words expanded on)

compile with "make", run with $./doublet -start- -target- -filename-

-example start/target test cases with provided "words.txt" file: "witch fails", "feed grow", "land disk"

text file format:

n - number of words

word 1

word i+1

...

word n
