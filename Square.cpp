#include "Square.hpp"

Square::Square (int row, int column):
    row_(row),column_(column),
    isTrap_((row == 2 || row == 5) && (column == 2 || column == 5))
{}

std::ostream &operator<<(std::ostream &os, const Square &square)
{
    os << square.getRow() + 1;
    os << "abcdefgh"[square.getColumn()];
    return os;
}
