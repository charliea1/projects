//GameState
/*
Goal of this class is to facilitate interactions between:
-entities and entities
-entities and the environment
It also manages the environment (board of "spots")
*/

#include "Entity.h"
#include <vector>
using namespace std;

//tile unit of game board
struct spot
{
    //keeps track of entities on tile
    vector<Entity *> occupants;
    //amount of food on tile
    int food;
};

class GameState
{
public:
    GameState(int scarcity, int board_size, int num_entities);
    ~GameState();

    //generate size x size board of spots
    void makeBoard();

    //reproduce entities that meet criteria
    void reproduceEntities();

    //adds entity to entities vector
    void addEntity(Entity *ent);

    //prompts entity to search randomly and check each spot for food
    void randSearch(Entity *ent);

    //when an entity finds food, link it to the spot it is on by:
    //adding entity to spot's vector of occupants, adding spot to vector of spots with found food
    //allows only checking occupied spots with food for eating/confrontation
    void linkToBoard(Entity *ent);

    //process eating/fighting at end of each movement cycle
    void processConfrontations();
    //helper for above function
    Entity *confrontation(vector<Entity *> ents);

    //process a turn cycle of 20 "clock-ticks"
    //allows for diversity of entity speeds
    //entities can reproduce during turn cycle, and will die if they do not find food at its end
    //1 food maintains survival, 2 needed to reproduce
    void ProcessTurn();

    void displayGen(ostream &outf);

private:
    //not used often in simulation, but for stats and tracking of game status
    vector<Entity *> entities;

    vector<spot *> occupied;

    //amount of food per 10 tiles
    int m_scarcity;

    //size of board = size*size
    int m_board_size;

    //number of entities at beginning of simulation
    int m_num_entities;

    //array of spots in forming board
    spot **board;
};