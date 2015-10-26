
#ifndef MOVE_H
#define MOVE_H

#include <string>
#include <list>

#include "Step.hpp"

/**
  * class Move
  *
  */

class Move
{
public:

    Move(int moveNr, bool gold);

    virtual ~Move() = default;


 private:
    std::list<Step> stepList_;

    int moveNr_;
    bool gold_;

 public:
    int getMoveNr() const {
	return moveNr_;
    }

    bool isGold() const {
	return gold_;
    }

    std::list<Step> getStepList() const {
        return stepList_;
    }

    void addStep(Step s) {
	stepList_.push_back(s);
    }
};

std::ostream &operator<<(std::ostream &os, const Move &move);
//Move parseMove(std::istream &is);

#endif // MOVE_H
