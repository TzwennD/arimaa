
#ifndef STEP_H
#define STEP_H

#include <stdexcept>
#include <iostream>

#include "enums.hpp"
#include "Direction.hpp"

/**
  * class Step
  *
  */
class Step
{
public:

    /* Should not be used */
    Step();

    Step(Direction direction, int column, int row, char piece);

    ~Step() = default;

    bool isInitialisationStep() const {
	return direction_.getDirection() == STAY;
    }

 private:
    Direction direction_;
    /* Attention:
     * This stores the actual board vector indecies,
     * not the visual names of the squares!
     */
    int column_;
    int row_;
    char piece_;

 public:
    Direction getDirection() const {
	return direction_;
    }
    int getRow() const {
	return row_;
    }
    int getColumn() const {
	return column_;
    }
    char getPiece() const {
	return piece_;
    }
    int getDestRow() const {
	return row_ + direction_.getRow();
    }
    int getDestColumn() const {
	return column_ + direction_.getColumn();
    }
};

std::ostream &operator<<(std::ostream &os, const Step &step);

#endif // STEP_H
