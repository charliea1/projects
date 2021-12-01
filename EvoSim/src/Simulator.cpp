/*SIMULATOR:
Using entities and Environment, runs simulation with specified variables
such as board size, starting stats, number of entities, number of iterations, etc.
Results output to text file specified by first command line argument
*/
#include "../lib/Environment.h"
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <ctime>
#include <cstdlib>

using namespace std;

void runSim(ofstream &out, int scarcity, int board_size, int num_entities, int num_iterations, int num_generations);
void runGen(Environment *game, int iterations, ostream &outf);

int main(int argc, char *argv[])
{
    //seed for random number generation
    int seed;
    seed = time(0);
    srand(seed);

    //setup output file
    string filename = "SIM-";
    //if input by user, take custom file name for output
    if (argc > 1)
    {
        filename = filename + string(argv[1]);
    }
    ofstream output(filename, ofstream::out);
    runSim(output, 2, 20, 20, 5, 10);
    output.close();
    return 0;
}

void runSim(ofstream &out, int scarcity, int board_size, int num_entities, int num_iterations, int num_generations)
{
    //initialize Environment
    Environment *game;
    game = new Environment(scarcity, board_size, num_entities);

    //make board
    game->makeBoard();

    //initialize entities
    for (int i = 0; i < num_entities; i++)
    {
        //create entity and add to Environment
        Entity *ent;
        ent = new Entity();
        game->addEntity(ent);

        //assign random position on board
        int pos = (rand() % (board_size * board_size));
        ent->set("pos", pos);

        //assign variance of mutation
        ent->set("var", 10);
    }
    for (int i = 0; i < num_generations; i++)
    {
        runGen(game, num_iterations, out);
    }
}

void runGen(Environment *game, int iterations, ostream &outf)
{
    for (int i = 0; i < iterations; i++)
    {
        game->ProcessTurn();
    }
    game->displayGen(outf);
}