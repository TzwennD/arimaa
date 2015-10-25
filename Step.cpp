#include <sstream>
#include <vector>

#include "Step.hpp"
#include "Direction.hpp"

using namespace std;

Step::Step() {}

Step::Step (Direction direction, int column, int row, char piece): 
    direction_(direction), column_(column), row_(row), piece_(piece)
{
}

ostream &operator<<(ostream &os, const Step &step)
{
    vector<char> alphabet = {'a','b','c','d','e','f','g','h'};
    os << step.getPiece()
       << alphabet[step.getColumn()]
       << step.getRow()
       << step.getDirection();
    return os;
}

