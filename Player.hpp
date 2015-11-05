#ifndef PLAYER_H
#define PLAYER_H

#include "list"

#include "Observer.hpp"
#include "Step.hpp"


/**
 * class Player
 * gets board information through notify-function inherited by Observer
 */
class Player : public Observer
{
public:
    Player ();

    virtual ~Player ( ) = default;


    /**
     * ask until order "finish"
     * or four correct steps
     * gives color in case two player are one object
     */
    virtual Step getStep(bool gold) = 0;

    /*
     * gives color in case two player are one object
     */
    virtual Move getStartPosition(bool gold) = 0;

    virtual void notify(Board copy_board, std::list<Move> changes) = 0;

};

#endif // PLAYER_H
