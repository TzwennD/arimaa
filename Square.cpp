#include "Square.hpp"

Square::Square (int row, int column):
    row_(row),column_(column),
    isTrap_((row == 3 || row == 6) && (column == 3 || column == 6))
{}

std::ostream &operator<<(std::ostream &os, const Square &square)
{
    os << square.getRow();
    os << "abcdefgh"[square.getColumn()-1];
    return os;
}
