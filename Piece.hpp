
#ifndef PIECE_H
#define PIECE_H

#include <iostream>
#include <stdexcept>

#include "enums.hpp"

/**
  * class Piece
  *
  */

class Piece
{
public:
    Piece (): type_(piece_type('e')), gold_(true), empty_(true) {}
    Piece (piece_type type_, bool isGold_);
    Piece (char ch);

    virtual ~Piece ( ) = default;

    bool isStronger (Piece other) const;

    piece_type getType() const {
        return type_;
    }

    bool isGold() const {
        return gold_;
    }

    bool isEmpty() const {
	return empty_;
    }

    char toChar() const {
	char ch = (char)type_;
	if (gold_)
	    ch = toupper(ch);
	return ch;
    }

private:
    piece_type type_;
    bool gold_;
    bool empty_;
};

std::ostream &operator<<(std::ostream &os, const Piece &piece);

#endif // PIECE_H
