#include <vector>

#include "Direction.hpp"

using namespace std;

Direction::Direction (): row_(0), column_(0), direction_(STAY) {}

Direction::Direction (direction_type direction): row_(0), column_(0), direction_(direction) {
    switch (direction) {
    case NORTH:
        row_ = 1;
        break;
    case EAST:
        column_ = 1;
        break;
    case SOUTH:
        row_ = -1;
        break;
    case WEST:
        column_ = -1;
        break;
    case STAY:
        /* stay means (0,0) = default */
        break;
    default:
        throw invalid_argument("Wrong direction!");
    }
}


ostream &operator<<(ostream &os, const Direction &direction)
{
    switch (direction.getDirection()) {
    case NORTH:
        os << "n";
        break;
    case EAST:
        os << "e";
        break;
    case SOUTH:
        os << "s";
        break;
    case WEST:
        os << "w";
        break;
    case STAY:
        /* dont't print anything */
        break;
    default:
        throw invalid_argument("Wrong direction!");
    }
    return os;
}

vector<Direction>
Direction::getAllDirections()
{
    return vector<Direction>{Direction(NORTH), Direction(EAST), Direction(SOUTH), Direction(WEST)};
}
