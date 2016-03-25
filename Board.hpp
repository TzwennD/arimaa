#ifndef BOARD_H
#define BOARD_H

#include <string>
#include <vector>
#include <list>
#include <set>

#include "Square.hpp"
#include "Step.hpp"
#include "Move.hpp"
#include "Piece.hpp"

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
     * Attention: Indices do not match square coordinates!
     * Squares counted from 1 but indices from 0.
     * Also North and South are switched because they are
     * viewed from the gold player.
     */
    std::vector<std::vector<Square> > squares_;

    std::list<Piece> deadPieces_;

    /*
     * Store square if next step must complete push at this position
     * Otherwise null
     */
    Square *pushSquare_;

public:


    Step movePiece (Step step, bool gold, int stepNr, Step lastStep);

    void applyInitMove(Move& move);
    void emptyStartPosition(bool gold);

    void updatePossibleMoves (bool isPlayerGold );

    std::vector<std::vector<Square> > getSquares ( ) const {
        return squares_;
    }

    bool endPosition(bool& gold);
    bool rabbitEndPos(bool gold);

private:
    /* for initial setup */
    void setPiece (Step step);

    bool isPieceFrozen (int row, int column) const;

    std::set<Direction> getFreeDirections (int row, int column) const;

    /* movePiece helper*/
    Step removeTrappedPieces();
    bool isStrongerPieceNear(int row, int column, bool gold) const;

    std::vector<Piece> getAllNeighboringPieces(int row, int column) const;
};

std::ostream &operator<<(std::ostream &os, const Board &board);

#endif // BOARD_H
