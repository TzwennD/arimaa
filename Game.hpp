#ifndef GAME_H
#define GAME_H

#include <string>
#include <list>
#include <stdexcept>

#include "Observer.hpp"
#include "Player.hpp"
#include "Board.hpp"
#include "Move.hpp"


class Game
{
 private:
    static const int FIRSTMOVE = 1;
    static const bool FIRSTCOLOR = true;

    static const int COLORGOLD = 0;
    static const int COLORSILVER = 1;
    
 public:

    Game ();

    virtual ~Game ( ) = default;

 
    /**
     * config 0: no player observed (remote spectator)
     * 1: player gold observed (bot or remote)
     * 2: player silver observed (bot or remote)
     * 3: all players observed (local game)
     */
    void registerObserver (Observer *newObserver, observer_config config );


    /**
     * adds also as observer
     */
    void addPlayer (Player *newPlayer, bool gold);

    void start();

    bool isFinished();

private:


    bool goldToPlay_;
    Board board_;

    /* COLORGOLD for gold, COLORSILVER for silver */
    std::list<Observer*> observerCollection_[2];

    std::list<Observer*> observerCollectionNone_;
    std::list<Observer*> observerCollectionAll_;

    /* 
     * must be 2 players
     * gold player is nr COLORGOLD
     * silver player is nr COLORSILVER
     * use color2int to convert
     */
    Player* players_[2];

    // moves stores stack/ list of all moves - Command Pattern
    std::list<Move> moves_;

    int rounds_;
    bool finished_;

    // must always be initialized
    Move currentMove_;

public:

    int getRounds ( ) const {
        return rounds_;
    }

    bool isGoldToPlay() const {
	return goldToPlay_;
    }

    std::list<Move> getMoves ( ) const {
        return moves_;
    }

    Move getCurrentMove() const {
	return currentMove_;
    }

    friend std::ostream &operator<<(std::ostream &os, const Game &game);
private:

    int color2int(bool gold) {
	if (gold)
	    return COLORGOLD;
	else
	    return COLORSILVER;
    }

    void playOneRound();


    void setStartPosition(bool gold);


    void notifyObserver ( ) const;
};

std::ostream &operator<<(std::ostream &os, const Game &game);

#endif // GAME_H
