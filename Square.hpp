#ifndef SQUARE_H
#define SQUARE_H

#include <iostream>
#include <set>

#include "Direction.hpp"
#include "Piece.hpp"

class Piece;

/**
 * class Square
 *
 */

class Square
{
public:

    Square (int row, int column);

    virtual ~Square ( ) = default;

    bool isEmpty ( ) const {
	return piece_.isEmpty();
    }

    Square& operator=(Square&) = delete;

private:
    /*
     * Attention:
     * Stored row and column are game row and column.
     * Not indices into the board vector!
     */
    const int row_;
    const int column_;

    const bool isTrap_;

    Piece piece_;
    std::set<Direction> movesPossible_;

public:
    bool getIsTrap() const {
        return isTrap_;
    }

    void setPiece(Piece new_var) {
	piece_ = new_var;
    }

    Piece getPiece() const {
        return piece_;
    }

    int getRow() const {
        return row_;
    }

    int getColumn() const {
        return column_;
    }

    void setMovesPossible (std::set<Direction> moves) {
	movesPossible_ = moves;
    }

    std::set<Direction> getMovesPossible () const {
        return movesPossible_;
    }
};

std::ostream &operator<<(std::ostream &os, const Square &square);


#endif // SQUARE_H
