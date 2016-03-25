#ifndef OBSERVER_H
#define OBSERVER_H

#include <list>

#include "Board.hpp"
#include "Move.hpp"

/**
 * class Observer
 *
 */

class Observer
{
public:

    /**
     * Empty Constructor
     */
    Observer ( );

    /**
     * Empty Destructor
     */
    virtual ~Observer ( );

    /**
     * @param  copy_board
     * @param  changes
     */
    virtual void notify (Board copy_board, std::list<Move> changes ) = 0;
    virtual void notifyGameEnd(Board copy_board, std::list<Move> changes,
                               bool winningPlayer) = 0;
    virtual void notifyError(std::string error) = 0;
};

#endif // OBSERVER_H
