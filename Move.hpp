
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

    Move& operator=(Move&) = delete;

private:
    std::list<Step> stepList_;

    const int moveNr_;
    const bool gold_;

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

    void removeLastStep() {
        stepList_.pop_back();
    }
};

std::ostream &operator<<(std::ostream &os, const Move &move);

#endif // MOVE_H
