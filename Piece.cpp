#include <vector>
#include <algorithm>
#include <iterator>

#include "Piece.hpp"

Piece::Piece(piece_type type, bool isGold): type_(type), gold_(isGold), empty_(false)
{
}

Piece::Piece(char ch): type_(piece_type(tolower(ch))), gold_(isupper(ch)), empty_(false)
{
}

std::ostream &operator<<(std::ostream &os, const Piece &piece)
{
    char c;

    if (piece.isEmpty())
        c = ' ';
    else {
        c = piece.getType();
        if (piece.isGold()) {
            c = std::toupper(c);
            os << "\033[1m";
        }
    }

    os << c << "\033[0m";

    return os;
}

bool
Piece::isStronger(Piece other) const
{
    std::vector<char> ch = {'r', 'c', 'd', 'h', 'm', 'e'};

    int type1 = std::distance(ch.begin(), std::find(ch.begin(), ch.end(), getType()));
    int type2 = std::distance(ch.begin(), std::find(ch.begin(), ch.end(), other.getType()));

    return type1 > type2;
}
