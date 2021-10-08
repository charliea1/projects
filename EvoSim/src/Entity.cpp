#include "../lib/Entity.h"

Entity::Entity()
{
    m_dead = 0;
    m_food = 0;
    m_rep = 0;
    m_att = 0;
    m_def = 0;
    m_agg = 0;
}

Entity &Entity::operator=(const Entity &ent)
{
    if (this != &ent)
    {
        this->m_att = ent.m_att;
        this->m_def = ent.m_def;
        this->m_agg = ent.m_agg;
        this->m_pos = ent.m_pos;
        this->m_food = 0;
        this->m_dead = 0;
        this->m_rep = 0;
    }
    return *this;
}

void Entity::mutate()
{
    //variance/100 chance to mutate
    int num = rand() % 100;

    //if random number is <= variance, mutate a stat
    if (num <= m_var)
    {
        int stat = (rand() % 4) + 1; //num used to determine mutated stat

        //handle maxed stats
        //if a stat is maxed, mutate different stat
        if ((m_spd == 10) && (num == 1))
        {
            stat = 2;
        }
        if ((m_att == 10) && (num == 2))
        {
            stat = 3;
        }
        if ((m_def == 10) && (num == 3))
        {
            stat = 1;
        }

        //perform mutation

        //speed, attack, defense are zero-sum, while aggression is independent of these stats
        int weakened = (rand() % 2) + 1;

        if (stat == 1)
        { //mutate speed
            m_spd += 1;
            if (weakened == 1)
            {
                if (m_def > 0)
                    m_def -= 1;
            }
            else
            {
                if (m_att > 0)
                    m_att -= 1;
            }
        }

        else if (stat == 2)
        { //mutate attack
            m_att += 1;
            if (weakened == 1)
            {
                if (m_spd > 0)
                    m_spd -= 1;
            }
            else
            {
                if (m_def > 0)
                    m_def -= 1;
            }
        }

        else if (stat == 3)
        { //mutate defense
            m_def += 1;
            if (weakened == 1)
            {
                if (m_spd > 0)
                    m_spd -= 1;
            }
            else
            {
                if (m_att > 0)
                    m_att -= 1;
            }
        }

        else
        { //mutate aggression
            //do not allow aggresssion to become negative
            if ((weakened == 1) && (m_agg > 0))
                m_agg -= 1;
            else
                m_agg += 1;
        }
    }
}

void Entity::move(string dir, int board_size)
{
    //movement does not allow for wrapping--if an entity tries to move
    //past an edge, they will move the opposite direction
    //hence the need to pass in board size

    //move left -- in flat array equates to pos = pos - 1
    if (dir == "L")
    {
        //if at left edge of grid, move right instead
        if (((m_pos % board_size) == 0) || (m_pos == 0))
        {
            m_pos++;
        }
        //move left
        else
        {
            m_pos--;
        }
    }
    //move right -- in flat array equates to pos = pos + 1
    else if (dir == "R")
    {
        //if at right edge of grid, move left instead
        if ((m_pos % (board_size - 1)) == 0)
        {
            m_pos--;
        }
        //move right
        else
        {
            m_pos++;
        }
    }
    //move up -- -- in flat array of size*size this equates to pos = pos - size
    else if (dir == "U")
    {
        //if at top of grid, move down instead
        if (m_pos < board_size)
        {
            m_pos += board_size;
        }
        //move up
        else
        {
            m_pos -= board_size;
        }
    }
    //move down -- in flat array equates to pos = pos + size
    else if (dir == "D")
    {
        //if at bottom of grid, move up instead
        if (m_pos >= (board_size * board_size - board_size))
        {
            m_pos -= board_size;
        }
        //move down
        else
        {
            m_pos += board_size;
        }
    }
}

void Entity::randMove(int board_size)
{

    //0-up, 1-down, 2-left, 3-right
    int direction = rand() % 4;

    //move up
    if (direction == 0)
    {
        move("U", board_size);
    }
    //move down
    else if (direction == 1)
    {
        move("D", board_size);
    }
    //move left
    else if (direction == 2)
    {
        move("L", board_size);
    }
    //move right
    else
    {
        move("R", board_size);
    }
}

void Entity::set(std::string stat, int val)
{
    if (stat == "att")
    {
        m_att = val;
        return;
    }
    else if (stat == "spd")
    {
        m_spd = val;
        return;
    }
    else if (stat == "def")
    {
        m_def = val;
        return;
    }
    else if (stat == "agg")
    {
        m_agg = val;
        return;
    }
    else if (stat == "food")
    {
        m_food = val;
        return;
    }
    else if (stat == "pos")
    {
        m_pos = val;
        return;
    }
    else if (stat == "var")
    {
        m_var = val;
        return;
    }
    else if (stat == "rep")
    {
        m_rep = val;
        return;
    }
    else if (stat == "dead")
    {
        m_dead = val;
        return;
    }
    else if (stat == "dist")
    {
        m_distance = val;
        return;
    }
    else
    {
        return;
    }
}

const int Entity::get(std::string stat)
{
    if (stat == "att")
    {
        return m_att;
    }
    else if (stat == "spd")
    {
        return m_spd;
    }
    else if (stat == "def")
    {
        return m_def;
    }
    else if (stat == "agg")
    {
        return m_agg;
    }
    else if (stat == "food")
    {
        return m_food;
    }
    else if (stat == "pos")
    {
        return m_pos;
    }
    else if (stat == "var")
    {
        return m_var;
    }
    else if (stat == "rep")
    {
        return m_rep;
    }
    else if (stat == "dead")
    {
        return m_dead;
    }
    else if (stat == "dist")
    {
        return m_distance;
    }
    else
    {
        return 0;
    }
}

void Entity::eat(int food)
{
    m_food += food;
}

void Entity::setRandRange(double lower, double upper)
{
    m_range_lower = lower;
    m_range_upper = upper;
}

void Entity::addRandRange(double lower, double upper)
{
    m_range_lower += lower;
    m_range_upper += upper;
}

const double Entity::rangeLower()
{
    return m_range_lower;
}
const double Entity::rangeUpper()
{
    return m_range_upper;
}

void Entity::printStats(ostream &outf)
{
    outf << "ATT: " << m_att << " | DEF: " << m_def << " | AGG: " << m_agg << endl;
}