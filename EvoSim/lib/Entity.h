#include <string>
#include <fstream>
using namespace std;
class Entity
{
public:
    Entity &operator=(const Entity &);

    Entity();

    //reproduce with variance/100 chance of mutation
    void reproduce(int variance);

    //entities added may move differently, allow for expansion
    void move(string dir, int board_size);

    //base entity moves randomly
    void randMove(int board_size);

    //roll dice and potentially mutate a stat
    void mutate();

    void printStats(ostream &outf);

    //setter/getter fns for stats
    void set(string stat, int val);
    const int get(string stat);
    void eat(int food);
    void setRandRange(double lower, double upper);
    void addRandRange(double lower, double upper);
    const double rangeLower();
    const double rangeUpper();

private:
    int m_att;  //ability to win fight (0-10)
    int m_def;  //ability to survive lost fight (0-10)
    int m_agg;  //willingness to engage in fight (uncapped)
    int m_food; //food consumed on a given turn
    int m_pos;  //position on board
    int m_var;  // % chance of mutation on reproduction (variance)
    int m_rep;  //flags that this has consumed enough food to reproduce
    int m_dead; //flag that this is dead
    double m_range_upper;
    double m_range_lower; //low and high values forming winning range for die rolling
    int m_spd;
    int m_distance;
};