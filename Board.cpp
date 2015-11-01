#include <list>
#include <algorithm>

#include "Board.hpp"

#include "Direction.hpp"
#include "Square.hpp"
#include "Piece.hpp"
#include "Step.hpp"

using namespace std;

/* Row indices are moved by one */
Board::Board ( ): squares_(8), deadPieces_(2) {
    for (int i = 1; i <= 8; ++i) {
	for (int j = 1; j <= 8; ++j) {
	    squares_[i-1].push_back(Square(i,j));
	}
    }
}

void
Board::movePiece (Step step)
{
    int row = step.getRow();
    int column = step.getColumn();
    Direction direction = step.getDirection();

    int new_row = step.getDestRow();
    int new_column = step.getDestColumn();

    if (isPieceFrozen(row,column))
	throw  invalid_argument("Piece is frozen! Cannot move.");

    if (!squares_[new_row][new_column].getPiece().isEmpty())
	throw  invalid_argument("There is a piece in the way! Cannot move.");

    if (squares_[row][column].getPiece().toChar() != step.getPiece())
	throw invalid_argument("Piece not at this position. Cannot move.");

    squares_[new_row][new_column].setPiece(step.getPiece());
    squares_[row][column].setPiece(Piece());
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
    vector<Direction> allDirections;
    allDirections.push_back(Direction(NORTH));
    allDirections.push_back(Direction(EAST));
    allDirections.push_back(Direction(SOUTH));
    allDirections.push_back(Direction(WEST));

    bool ownColor = squares_[row][column].getPiece().isGold();

    /*
     * if no other pieces -> false
     * if only enemy pieces -> true
     * if any friendly piece -> directly false
     */
    bool ret = false;

    for(Direction &d : allDirections) {
	int new_row = row + d.getRow();
	int new_column = column + d.getColumn();

	if (new_row < 0 || new_row >= 8 ||
	    new_column < 0 || new_column >= 8 )
	    continue;

	if (squares_[new_row][new_column].getPiece().isGold() == ownColor)
	    return false;
	else {
	    if (squares_[row][column].getPiece().
		isStronger(squares_[new_row][new_column].getPiece()))
		ret = true;
	}
    }

    return ret;
}


void
Board::updatePossibleMoves (bool isPlayerGold )
{
    for (std::vector<Square> row : squares_) {
	for (Square square : row) {
	    if (square.isEmpty()) {
		square.setMovesPossible(set<Direction>());
		continue;
	    }
	    if (square.getPiece().isGold() == isPlayerGold) {
		if (isPieceFrozen(square.getRow(),square.getColumn())) {
		    square.setMovesPossible(set<Direction>());
		    continue;
		} else
		    square.setMovesPossible(
			getFreeDirections(square.getRow(), square.getColumn()));
	    } else { /* foreign colour */
		/* TODO */
		square.setMovesPossible(set<Direction>());
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


    /* all possible directions */
    vector<Direction> allDirections;
    allDirections.push_back(Direction(NORTH));
    allDirections.push_back(Direction(EAST));
    allDirections.push_back(Direction(SOUTH));
    allDirections.push_back(Direction(WEST));


    for(Direction &d : allDirections) {
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
