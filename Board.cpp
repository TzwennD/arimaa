#include <list>
#include <algorithm>

#include "Board.hpp"

#include "Direction.hpp"
#include "Square.hpp"
#include "Piece.hpp"
#include "Step.hpp"

using namespace std;  

/* squares are upside-down */
Board::Board ( ): squares(8), deadPieces(2) {
    for (int i = 8; i >= 1; i--) {
	for (int j = 1; j <= 8; j++) {
	    squares[8-i].push_back(Square(i,j));
	}
    }
}

/* Attention: no sanity checks for step */
bool
Board::movePiece (Step step)
{
  int row = step.getRow();
  int column = step.getColumn();
  Direction direction = step.getDirection();

  int new_row = row;
  int new_column = column;

  if (isPieceFrozen(row,column))
    return false;

  new_row += direction.getRow();
  new_column += direction.getColumn();
  if (new_row < 0 || new_row >= 8)
    return false;
  if (new_column < 0 || new_column >= 8)
    return false;


  // is there already a piece in the way?
  if (!squares[new_row][new_column].getPiece().isEmpty())
    return false;

  squares[new_row][new_column].
    setPiece(squares[row][column].getPiece());
  squares[row][column].setPiece(Piece());


  return true;
}

/* for initial setup */
void
Board::setPiece (Step step)
{
  int column = step.getColumn();
  int row = step.getRow();
  cout << "Setting piece to (" << column << "," << row << ")." << endl;
  Piece piece(step.getPiece());
  
    /* validate right position */
  if (row >= 0 && row < 2) {// silver side
    if (piece.isGold()) //wrong color
      throw invalid_argument("wrong side");
  }
  else if (row >= 6 && row < 8) {// gold side
    if (!piece.isGold()) // wrong side
      throw invalid_argument("wrong side");
  }
  else // no initial start position
    throw invalid_argument("No start position");

  /* square must be free */
  if (!squares[row][column].isEmpty())
    throw invalid_argument("not empty");

  squares[row][column].setPiece(piece);
}

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
    
    bool ownColor = squares[row][column].getPiece().isGold();
    
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

	if (squares[new_row][new_column].getPiece().isGold() == ownColor)
	    return false;
	else {
	    if (squares[row][column].getPiece().isStronger(squares[new_row][new_column].getPiece()))
		ret = true;
	}
    }

    return ret;
}


void
Board::updatePossibleMoves (bool isPlayerGold )
{
    for (std::vector<Square> row : squares) {
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
	if (squares[new_row][new_column].isEmpty())
	    ret.insert(d);
    }
        
    return ret;
}

ostream &operator<<(std::ostream &os, const Board &board)
{
  os  << "   a b c d e f g h" << endl;
  os << " +-----------------+" << endl;
  for (int i = 0; i < 8; i++) {
	os << i+1 << "|";
	for (int j = 0; j < 8; j++) {
	    Square sq = board.getSquares()[i][j];
	    if (sq.isEmpty())
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
