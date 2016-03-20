#include <list>
#include <algorithm>

#include "Board.hpp"

#include "Direction.hpp"
#include "Square.hpp"
#include "Piece.hpp"
#include "Step.hpp"

using namespace std;

/* Row indices are moved by one */
Board::Board ( ): squares_(8), deadPieces_(2), pushSquare_(nullptr) {
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            squares_[i].push_back(Square(i,j));
        }
    }
}

/* Attention: don't use lastStep in first step */
Step
Board::movePiece (Step step, bool gold, int stepNr, Step lastStep)
{
    int row = step.getRow();
    int column = step.getColumn();
    Direction direction = step.getDirection();

    int new_row = step.getDestRow();
    int new_column = step.getDestColumn();

    const Piece& currentPiece = squares_[row][column].getPiece();

    if (currentPiece.isGold() == gold
        && isPieceFrozen(row,column))
        throw  invalid_argument("Piece is frozen! Cannot move.");

    if (!squares_[new_row][new_column].getPiece().isEmpty())
        throw  invalid_argument("There is a piece in the way! Cannot move.");

    if (currentPiece.toChar() != step.getPiece())
        throw invalid_argument("Piece not at this position. Cannot move.");

    if (pushSquare_) {
        if (pushSquare_->getRow() != new_row
            || pushSquare_->getColumn() != new_column)
            throw invalid_argument("You have to complete the push.");
        if (currentPiece.isGold() != gold)
            throw invalid_argument("You try to push with an opponants piece.");
    }

    /*
     * when pushing, check if own stronger piece is near
     * or when we are pulling..
     */
    bool pushing = false;
    if (currentPiece.isGold() != gold) {
        /* if pulling possible */
        if (stepNr > 0
            && lastStep.getRow() == new_row && lastStep.getColumn() == new_column
            && (isupper(lastStep.getPiece()) != 0) == gold /* same color */
            && Piece(lastStep.getPiece()).isStronger(currentPiece) )
            ; // do nothing, allow pulling
        /* else: no pulling possible */
        else {
            pushing = true;
            if (stepNr == 4)
                throw invalid_argument("Cannot push as last step.");
            if (!isStrongerPieceNear(row, column, gold))
                throw invalid_argument("Pushing without stronger piece, which can do that.");
        }
    }

    /* do actual movement */
    squares_[new_row][new_column].setPiece(step.getPiece());
    squares_[row][column].setPiece(Piece());

    Step deadAnimal = removeTrappedPieces();

    /* pushing completed? */
    if (pushSquare_)
        pushSquare_ = nullptr;

    /* currently pushing? */
    if (pushing)
        pushSquare_ = &squares_[row][column];

    return deadAnimal;
}

bool
Board::isStrongerPieceNear(int row, int column, bool gold) const
{
    for(Direction &d : Direction::getAllDirections()) {
        int neighbor_row = row + d.getRow();
        int neighbor_column = column + d.getColumn();
        if (neighbor_row < 0 || neighbor_row >= 8 ||
            neighbor_column < 0 || neighbor_column >= 8 )
            continue;
        Piece neighbourPiece = squares_[neighbor_row][neighbor_column].getPiece();
        if (!neighbourPiece.isEmpty()
            && neighbourPiece.isGold() == gold
            && neighbourPiece.isStronger(squares_[row][column].getPiece())
            && !isPieceFrozen(neighbor_row, neighbor_column)) {
            return true;
        }
    }
    return false;
}

/* TODO: add new step for every captured piece */
Step
Board::removeTrappedPieces()
{
    vector<int> traps = {2, 5};
    for (int row: traps) {
        for (int column: traps) {
            Piece thisPiece = squares_[row][column].getPiece();
            if (thisPiece.isEmpty())
                continue;
            /* there is a piece -> is it being hold by a friend? */
            bool hold = false;
            for(Piece &p : getAllNeighboringPieces(row, column)) {
                if (!p.isEmpty() && p.isGold() == thisPiece.isGold()) {
                    hold = true;
                    break;
                }
            }
            if (!hold) {
                deadPieces_.push_back(thisPiece);
                squares_[row][column].setPiece(Piece());
                Step deadAnimal(TRAPPED,column, row, thisPiece.toChar());
                return deadAnimal;
            }
        }
    }
    return Step(END);
}

/* for initial setup */
void
Board::setPiece (Step step)
{
    int column = step.getColumn();
    int row = step.getRow();
    Piece piece(step.getPiece());

    /* validate right position */
    if (row >= 0 && row < 2) {// gold side
        if (!piece.isGold()) //wrong color
            throw invalid_argument("Pieces set to wrong side.");
    }
    else if (row >= 6 && row < 8) {// silver side
        if (piece.isGold()) // wrong side
            throw invalid_argument("Pieces set to wrong side.");
    }
    else // no initial start position
        throw invalid_argument("Pieces not set to start position.");

    /* square must be free */
    if (!squares_[row][column].isEmpty())
        throw invalid_argument("Already a piece at that position.");

    squares_[row][column].setPiece(piece);
}

/* let setPiece validate positions */
void
Board::applyInitMove(Move& move)
{
    std::list<Step> stepList = move.getStepList();
    for_each(stepList.begin(), stepList.end(), [&] (Step s) { setPiece(s); });
}

bool
Board::isPieceFrozen (int row, int column) const
{
    /*
     * check for stronger ennemy pieces -> need friend
     * check for friendly pieces -> isNotFrozen
     */
    bool ownColor = squares_[row][column].getPiece().isGold();

    /*
     * if no other pieces -> false
     * if only enemy pieces -> true
     * if any friendly piece -> directly false
     */
    bool ret = false;

    for(Piece &p: getAllNeighboringPieces(row, column)) {
        if (p.isEmpty())
            continue;
        if (p.isGold() == ownColor)
            return false;
        if (p.isStronger(squares_[row][column].getPiece()))
            ret = true;
    }

    return ret;
}


/* TODO: pushing and pulling not implemented */
void
Board::updatePossibleMoves (bool isPlayerGold )
{
    if (pushSquare_) {
        /* only own pieces can move only to this square */
        // TODO
        return;
    }
    for (int row = 0; row < 8; ++row) {
        for (int column = 0; column < 8; ++column) {
            Square square = squares_[row][column];
            if (square.isEmpty()) {
                square.setMovesPossible(set<Direction>());
            } else if (square.getPiece().isGold() == isPlayerGold) {
                if (isPieceFrozen(row, column)) {
                    square.setMovesPossible(set<Direction>());
                } else {
                    square.setMovesPossible(
                        getFreeDirections(row, column));
                }
            } else { /* foreign colour */
                set<Direction> dir();
                /*int free_direct = get_free_directions(square);
                  if (free_direct) */
                /* test all not free neighbour square */
                /* if (piece_on_this_square.isGold == isPlayerGold &&
                   piece_on_this_square.isStronger(square.piece) &&
                   isPieceFrozen(piece_on_this_square))
                   square.possible_moves = get_free_directions(square);*/
            }
        }
    }
}


set<Direction>
Board::getFreeDirections (int row, int column) const
{
    /* return set */
    set<Direction> ret;

    for (Direction &d: Direction::getAllDirections()) {
        int new_row = row + d.getRow();
        int new_column = column + d.getColumn();

        if (new_row < 0 || new_row >= 8 ||
            new_column < 0 || new_column >= 8 )
            continue;

        /* add to return set iff square free */
        if (squares_[new_row][new_column].isEmpty())
            ret.insert(d);
    }

    return ret;
}

vector<Piece>
Board::getAllNeighboringPieces(int row, int column) const
{
    vector<Piece> ret;

    for (Direction &d: Direction::getAllDirections()) {
        int new_row = row + d.getRow();
        int new_column = column + d.getColumn();

        if (new_row < 0 || new_row >= 8 ||
            new_column < 0 || new_column >= 8 )
            continue;
        ret.push_back(squares_[new_row][new_column].getPiece());
    }

    return move(ret);
}

/* Input: Color to check for */
bool
Board::rabbitEndPos(bool gold)
{
    int aimRow;
    if (gold) {
        aimRow = 7;
    } else {
        aimRow = 0;
    }

    for (Square s : squares_[aimRow]) {
        if (!s.getPiece().isEmpty()
            && s.getPiece().isGold() == gold
            && s.getPiece().getType() == RABBIT) {
            return true;
        }
    }

    return false;
}

/*
 * The order of checking for win/lose conditions is as follows assuming
 *       player A just made the move and player B now needs to move:

 * Check if a rabbit of player A reached goal. If so player A wins.
 * Check if a rabbit of player B reached goal. If so player B wins.
 * Check if player B lost all rabbits. If so player A wins.
 * Check if player A lost all rabbits. If so player B wins.
 * Check if player B has no possible move (all pieces are frozen
 *       or have no place to move). If so player A wins.
 * Check if the only moves player B has are 3rd time repetitions.
 *       If so player A wins.
 * Taken from http://arimaa.com/arimaa/learn/rulesIntro.html
 */

/*
 * Input: player that just finished his move
 * Output: return value: is game finished?
 *         gold: which player wins?
 */
bool
Board::endPosition(bool& gold)
{
    if (rabbitEndPos(gold)) {
        return true;
    } else if (rabbitEndPos(!gold)) {
        gold = !gold;
        return true;
    } // TODO
    return false;
}

ostream &operator<<(std::ostream &os, const Board &board)
{
    os << " +-----------------+" << endl;
    for (int i = 8; i >= 1; --i) {
        os << i << "|";
        for (int j = 0; j < 8; j++) {
            Square sq = board.getSquares()[i-1][j];
            if(sq.getIsTrap()) {
                if (sq.isEmpty())
                    os << " \033[41m \033[0m";
                else
                    os << " \033[41m" << sq.getPiece() << "\033[0m";
            }
            else if (sq.isEmpty())
                os << "  ";
            else
                os << " " << sq.getPiece();
        }
        os << " |" << endl;
    }

    os << " +-----------------+" << endl
       << "   a b c d e f g h" << endl;
    return os;
}
