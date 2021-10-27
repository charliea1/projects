#include "MinHeap.h"
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>
using namespace std;

void toLower(string *input);

// struct stored in graph holding name, id, index, distance/heuristic value, and adjacency list
struct node
{
    string name;
    vector<node *> *adj;
    int d = 0;
    int h;
    int id;
    int index;
};

// function to form graph
void connectNodes(node *in, vector<node *> *graph);

int main(int argc, char *argv[])
{
    // read in start and target words
    string start = argv[1];
    string target = argv[2];
    // convert to lowercase
    toLower(&start);
    toLower(&target);

    // set up words filestream
    string filename = argv[3];
    ifstream infile(filename.c_str());
    int size;
    infile >> size;

    vector<node *> graph;

    // read words, convert to lowercase, add to map of strings to adjacency lists
    for (int i = 0; i < size; i++)
    {
        string input;
        infile >> input;
        toLower(&input);
        // if a word is not length of start/target, do not add it
        if (input.length() != start.length())
        {
            continue;
        }

        // create node and its adjacency list
        node *out = new node;
        out->name = input;
        out->adj = new vector<node *>;

        // determine heuristic value
        // assume no matching letters and decrease h as they are found
        int h = input.length();
        for (int i = 0; i < input.length(); i++)
        {
            if (target[i] == input[i])
            {
                h--;
            }
        }
        out->h = h;

        // push node to graph vector
        graph.push_back(out);
    }

    // generate words differing by one letter, find in map original list
    // if word found, add to current word's adjacency list
    for (int i = 0; i < graph.size(); i++)
    {
        graph[i]->id = i;
        connectNodes(graph[i], &graph);
    }

    // graph is generated, implement A*
    // find start
    int firstNode;
    for (int i = 0; i < graph.size(); i++)
    {
        if (graph[i]->name == start)
        {
            firstNode = i;
        }
    }
    graph[firstNode]->d = 0;
    int idCount = 1;
    int steps = 0;
    int expansions = 0;

    // same naming conventions as notes sheet--
    // d[] holss distances of each node and p[] holds predecessors
    int *d = new int[size];
    int *p = new int[size];
    // initalize all but start to -1
    for (int i = 0; i < size; i++)
    {
        p[i] = -1;
        d[i] = -1;
    }

    // initialize heap and start node
    MinHeap<node *> pq(2);
    d[firstNode] = 0;
    p[firstNode] = 0;
    pq.add(graph[firstNode], 0);
    graph[firstNode]->index = 0;

    // bool to detect if target never found
    bool end = false;

    // implement A*
    while (!pq.isEmpty())
    {
        expansions++;
        node *v = pq.peek();
        pq.remove();

        // for all outgoing nodes from v
        for (int i = 0; i < v->adj->size(); i++)
        {
            // if edge of current node hasn't been visited or it is more easily reached through current node
            node *w = v->adj->at(i);
            if ((p[w->id] == -1) || ((d[v->id] + v->h) < (d[w->id] + w->h)))
            {

                // remember if first visit to this node
                bool first = false;
                if (d[w->id] == -1)
                {
                    first = true;
                }

                // update w's distance
                d[w->id] = d[v->id] + 1;
                w->d = d[w->id];

                // if final node, break and count steps
                if (w->name == target)
                {
                    steps = d[w->id];
                    end = true;
                    break;
                }

                // update w's previous node to v
                p[w->id] = v->id;

                // if w's first visit (v has no predecessors), add w to queue

                if (first || (w->name == start))
                {
                    pq.add(w, d[w->id]);
                    w->index = idCount;
                    idCount++;
                }
                // otherwise update w's priority in heap/queue
                else
                {
                    pq.update(w->index, d[w->id]);
                }
            }
        }
    }
    // if not found, print message, otherwise print number of steps
    if (!end)
    {
        cout << "No transformation" << endl;
    }
    else
    {
        cout << steps << endl;
    }
    cout << expansions << endl;

    // memory cleanup
    for (int i = 0; i < graph.size(); i++)
    {
        delete graph[i]->adj;
        delete graph[i];
    }

    delete[] d;
    delete[] p;

    return 0;
}

// takes a string in original list as input, generates words differing by one letter,
// searches map and adds found string to original string's adjacency list
void connectNodes(node *in, vector<node *> *graph)
{
    // generate all possible changes of one letter for in
    for (int i = 0; i < in->name.length(); i++)
    {
        for (int j = 0; j < 26; j++)
        {
            string temp = in->name;
            char swapped = 97 + j;
            swap(temp[i], swapped);
            if (temp == in->name)
            {
                continue;
            }

            // find specific combination in nodes and add node to adjacency list
            for (int i = 0; i < graph->size(); i++)
            {
                if (temp == graph->at(i)->name)
                {
                    in->adj->push_back(graph->at(i));
                }
            }
        }
    }
}

// convert string to lowercase by iterating through chars
// and converting with ASCII calculation
void toLower(string *input)
{
    for (int i = 0; i < input->length(); i++)
    {
        if ((*input)[i] < 90)
        {
            (*input)[i] += 32;
        }
    }
}