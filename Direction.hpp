#ifndef DIRECTION_H
#define DIRECTION_H

#include <stdexcept>
#include <iostream>
#include <vector>

#include "enums.hpp"


class Direction
{
public:
    Direction();
    Direction (direction_type direction);

    virtual ~Direction ( ) {}
    Direction& operator=(const Direction&) = default;


private:
    int row_;
    int column_;
    const direction_type direction_;

public:
    int getRow() const {
        return row_;
    }

    int getColumn() const {
        return column_;
    }

    direction_type getDirection() const {
        return direction_;
    }

    /* required for sets */
    bool operator<(const Direction &other) const {
        return getDirection() < other.getDirection();
    }

    static std::vector<Direction> getAllDirections();
};

std::ostream &operator<<(std::ostream &os, const Direction &direction);

#endif // DIRECTION_H
