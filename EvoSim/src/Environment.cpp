#include "../lib/Environment.h"
#include <iostream>

Environment::Environment(int scarcity, int board_size, int num_entities) : m_scarcity(scarcity),
                                                                           m_board_size(board_size), m_num_entities(num_entities) {}

//create size x size board of spots
void Environment::makeBoard()
{
    //make board array of spots
    board = new spot *[m_board_size * m_board_size];
    for (int i = 0; i < m_board_size * m_board_size; i++)
    {
        spot *tile = new spot;
        board[i] = tile;
        tile->food = 0;
    }
    int scaler = (m_board_size * m_board_size) / 10;
    for (int i = 0; i < m_scarcity * scaler; i++)
    {
        //place food on random spots according to scarcity
        int placed = (rand() % (m_board_size * m_board_size));
        board[placed]->food += 1;
    }
}

void Environment::reproduceEntities()
{
    //iterate through entities--if reproduce = true, reproduce with chance to mutate
    int size = this->entities.size();
    vector<Entity *> children;
    for (int i = 0; i < size; i++)
    {
        Entity *parent = entities.at(i);
        if (parent->get("rep") && !(parent->get("dead")))
        {
            //if more than two entities/spot, prevent further reproduction
            if ((children.size() + entities.size()) < 2 * (m_board_size * m_board_size))
            {
                Entity *child;
                child = new Entity();
                child = parent;
                //give child chance to mutate with parent's variance
                child->mutate();

                children.push_back(child);
            }
        }
    }
    for (int i = 0; i < children.size(); i++)
    {
        entities.push_back(children.at(i));
    }
}

void Environment::addEntity(Entity *ent)
{
    entities.push_back(ent);
}

void Environment::linkToBoard(Entity *ent)
{
    //find occupied spot
    int pos = ent->get("pos");
    spot *occ = board[pos];

    //only do if spot has food
    if (occ->food > 0)
    {
        //add entity to spot's vector of occupants
        occ->occupants.push_back(ent);

        //if no other entities on this tile,
        //push spot to occupied spots stack
        int num_entities = occ->occupants.size();
        if (num_entities == 1)
        {
            occupied.push_back(occ);
        }
    }

    //the entity is now linked to the board
    //and its interaction is ready for processing
}

void Environment::ProcessTurn()
{
    //20 "ticks" per turn
    int counter = 0;
    for (int j = 0; j < 20; j++)
    {
        //for each entity
        for (int i = 0; i < entities.size(); i++)
        {
            int dead = entities.at(i)->get("dead");
            //if not dead, randomly search one tile and check for food
            if (!dead)
            {
                randSearch(entities.at(i));
            }
        }
        //process eating and confrontations

        processConfrontations();
    }
    for (int i = 0; i < entities.size(); i++)
    {
        if (entities.at(i)->get("food") >= 1)
        {
            entities.at(i)->set("rep", 1);
        }
        else
        {
            entities.at(i)->set("dead", 1);
        }
    }
    reproduceEntities();
}

void Environment::processConfrontations()
{
    //iterate through occupied food spots
    for (int i = 0; i < occupied.size(); i++)
    {
        Entity *winner;
        int consumed = occupied.at(i)->food;
        //if spot has one entity, it eats
        if (occupied.at(i)->occupants.size() == 1)
        {
            Entity *occupant = occupied.at(i)->occupants.at(0);
            //find amount of food on spot, feed it to this entity, and remove it
            occupant->eat(consumed);
        }
        //if spot has multiple entities, confrontation occurs
        else if (occupied.at(i)->occupants.size() > 1)
        {
            winner = confrontation(occupied.at(i)->occupants);
            winner->eat(consumed);
        }
        //clear occupants vector for on this spot
        for (int j = 0; j < (occupied[i]->occupants.size()); j++)
        {
            occupied.at(i)->occupants.pop_back();
        }
        //clear food
        occupied.at(i)->food = 0;
    }

    //clear Environment's occupied vector
    for (int i = 0; i < occupied.size(); i++)
    {
        occupied.pop_back();
    }
}

Entity *Environment::confrontation(vector<Entity *> ents)
{
    //for entity to yield, it must have 2 less aggression than another
    //chance of winning fight is allotted evenly + (10% per 1 difference in attack for highest attack value)
    //chance of surviving lost fight is based on (this entity's) defense to winner's attack ratio

    //find highest aggression level
    int highest = ents.at(0)->get("agg");
    for (int i = 0; i < ents.size(); i++)
    {
        if ((ents.at(i)->get("agg")) > highest)
        {
            highest = ents.at(i)->get("agg");
        }
    }
    //process yielding
    for (int i = 1; i < ents.size(); i++)
    {
        int curr_aggression = ents.at(i)->get("agg");
        //if this entity's aggression is 2 less than highest, it yields
        if (curr_aggression < (highest - 1))
        {
            //using dead as placeholder value for yield
            ents.at(i)->set("dead", 1);
        }
    }

    //find highest attack among non-yielding entities
    double num_fighters = 0;
    highest = 0;
    int strongest = 0;
    for (int i = 0; i < ents.size(); i++)
    {
        //if this entity has a higher attack and has yielded
        if (((ents.at(i)->get("att")) > highest) && (ents.at(i)->get("dead") == 0))
        {
            highest = ents.at(i)->get("att");
            strongest = i;
            num_fighters++;
        }
    }
    //process fighting
    //first calculate chance of winning for each fighter
    //fighters lose 10% to the strongest ent per lower level of attack
    double base_chance = 100 / num_fighters;
    vector<Entity *> fighters;
    fighters.push_back(ents.at(strongest));
    for (int i = 0; i < ents.size(); i++)
    {
        //add all fighters to fighters vector
        if ((ents.at(i)->get("dead") == 0) && (i != strongest))
        {
            fighters.push_back(ents.at(i));
        }
    }
    //initialize roll ranges
    //each fighter has a range within 0-100; if the random number lands within
    //  this range, that fighter wins.
    //the following loop implements these steps:
    // 1) assigns equal base ranges to each fighter
    // 2) for each fighter: determines difference "dif" between its att and strongest ent's att
    //      -increases strongest ent's range by 10 * dif
    //      -decreases current fighter's range size by 10 * dif
    //      -adjusts prior fighters' lower/upper end of range in order to
    //       prevent overlap of their ranges and strongest's range while preserving range size
    //
    for (int i = 0; i < fighters.size(); i++)
    {
        //set equal base range for all fighters
        fighters.at(i)->setRandRange(i * base_chance, base_chance + (i * base_chance));
        //adjust range based on attack differential with strongest
        if (i == 0)
            continue;
        else
        {
            int dif = highest - fighters.at(i)->get("att");
            fighters.at(0)->addRandRange(0, 10 * dif);
            fighters.at(i)->addRandRange(10 * dif, 0);
            for (int j = 1; j < i; j++)
            {
                fighters.at(j)->addRandRange(10 * dif, 10 * dif);
            }
        }
    }

    //remove dead tags to prevent yielding ents from being killed
    for (int i = 0; i < ents.size(); i++)
    {
        ents.at(i)->set("dead", 0);
    }

    //generate random number and determine winner
    double num = rand() % 101;
    int winner = 0;
    for (int i = 0; i < fighters.size(); i++)
    {
        //check if num is within winning range for each entity
        if ((num > fighters.at(i)->rangeLower()) && (num < fighters.at(i)->rangeUpper()))
        {
            winner = i;
        }
        else
        {
            fighters.at(i)->set("dead", 1);
        }
    }
    //ties and exceptions go to strongest
    return fighters.at(winner);
}

//eventually implement with speed mechanic--right now each entitity moves one tile a time
void Environment::randSearch(Entity *ent)
{
    //move ent random direction
    ent->randMove(m_board_size);

    //check for food on tile
    int pos = ent->get("pos");
    if ((pos > -1) && (pos < (m_board_size * m_board_size)))
    {
        if (board[pos]->food > 0)
        {
            linkToBoard(ent);
        }
    }
}

void Environment::displayGen(ostream &outf)
{
    outf << "***************************************" << endl;
    outf << "***************************************" << endl;
    for (int i = 0; i < 20; i++)
    {
        if (!(entities.at(i)->get("dead")))
            entities.at(i)->printStats(outf);
    }
    outf << "***************************************" << endl;
    outf << "***************************************" << endl;
}

Environment::~Environment()
{
    for (int i = 0; i < m_board_size; i++)
    {
        delete board[i];
        board[i] = nullptr;
    }
    delete[] board;
    board = nullptr;

    for (int i = 0; i < entities.size(); i++)
    {
        delete entities.at(i);
    }
}