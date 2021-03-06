#include <sstream>

#include "Move.hpp"

using namespace std;

Move::Move(int moveNr, bool gold): moveNr_(moveNr), gold_(gold)
{}

ostream &operator<<(ostream &os, const Move &move)
{
    os << move.getMoveNr();
    if (move.isGold())
        os << "g";
    else
        os << "s";
    for (Step s : move.getStepList())
        os << " " << s;
    return os;
}
