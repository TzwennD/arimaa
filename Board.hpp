#ifndef BOARD_H
#define BOARD_H

#include <string>
#include <vector>
#include <list>
#include <set>

#include "Square.hpp"
#include "Step.hpp"
#include "Move.hpp"

/**
  * class Board
  *
  */

class Board
{
public:
    Board ( );

    virtual ~Board ( ) = default;


 private:
    /*
     * Attention: From view of gold player,
     * row 8 is the top row, row 1 is the lowest.
     * Row 8 is north, row 1 is south.
     * This vector is upside-down!
     */
    std::vector<std::vector<Square> > squares_;

    std::vector<std::list<Piece*> > deadPieces_;

 public:


    bool movePiece (Step step);

    /* for initial setup */
    void setPiece (Step step);
    void applyInitMove(Move& move);

    bool isPieceFrozen (int row, int column) const;


    void updatePossibleMoves (bool isPlayerGold );


    std::set<Direction> getFreeDirections (int row, int column) const;


    std::vector<std::vector<Square> > getSquares ( ) const {
        return squares_;
    }
};

std::ostream &operator<<(std::ostream &os, const Board &board);

#endif // BOARD_H
