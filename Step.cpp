#include <sstream>
#include <vector>
#include <algorithm>

#include "Step.hpp"
#include "Direction.hpp"

using namespace std;

Step::Step (Direction direction, int column, int row, char piece):
    direction_(direction), column_(column), row_(row), piece_(piece), type_(BASIC)
{
    /* Check starting square */
    if (row < 0 || row >= 8 || column < 0 || column >= 8)
        throw invalid_argument("Invalid step: Coming from out of the board.");

    /* Check piece */
    vector<char> figures = {'r', 'r', 'r', 'r', 'r', 'r', 'r', 'r',
                            'c', 'c', 'd', 'd', 'h', 'h', 'm', 'e'};
    if (find(figures.begin(), figures.end(), tolower(piece)) == figures.end())
        throw invalid_argument("Invalid step: Wrong piece.");

    /* Check destination */
    int new_row = row;
    int new_column = column;
    new_row += direction.getRow();
    new_column += direction.getColumn();
    if (new_row < 0 || new_row >= 8 || new_column < 0 || new_column >= 8)
        throw  invalid_argument("Invalid step: Going out of the board!");
}

Step::Step(step_type type):
    direction_(Direction()), column_(0), row_(0), piece_(0), type_(type) {}

ostream &operator<<(ostream &os, const Step &step)
{
    vector<char> alphabet = {'a','b','c','d','e','f','g','h'};
    os << step.getPiece()
       << alphabet[step.getColumn()]
       << step.getRow() + 1
       << step.getDirection();
    return os;
}

