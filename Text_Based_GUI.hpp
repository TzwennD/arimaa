#ifndef TEXT_BASED_GUI_H
#define TEXT_BASED_GUI_H

#include <list>

#include "Player.hpp"
#include "Game.hpp"
#include "Step.hpp"
#include "Observer.hpp"

/**
  * class Text_Based_GUI
  *
  */

class Text_Based_GUI : public Player
{
public:

    Text_Based_GUI();

    virtual ~Text_Based_GUI ( ) = default;


    /**
     * ask until order "finish"
     * or four correct steps
     * gives color in case two player are one object
     */
    virtual Step getStep(bool gold);

    /*
     * gives color in case two player are one object
     */
    virtual Move getStartPosition(bool gold);


    void notify(Board copy_board, std::list<Move> changes);

 private:
    Game game;

 public:
    Game& getGame() {
	return game;
    }

};

#endif // TEXT_BASED_GUI_H
