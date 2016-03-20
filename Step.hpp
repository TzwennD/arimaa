
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
    /*
     * TODO: allow also special step for end, takeback (last move) and resign
     * or maybe only special step for takeback and resign (offical)
     * and exception for end?
     * add undo to undo last step
     */
    Step(Direction direction, int column, int row, char piece);

    Step(step_type type);

    ~Step() = default;

    bool isInitialisationStep() const {
        return direction_.getDirection() == STAY;
    }

    Step& operator=(const Step&) = default;

private:
    const Direction direction_;

    /* Stores column and row as indices on the board.
     * Not as seen in Arimaa notation.
     */
    const int column_;
    const int row_;
    const char piece_;
    const step_type type_;

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
    step_type getType() const {
        return type_;
    }
};

std::ostream &operator<<(std::ostream &os, const Step &step);

#endif // STEP_H
