#include <algorithm>

#include "Game.hpp"

#include "Observer.hpp"
#include "Player.hpp"
#include "enums.hpp"

using namespace std;

Game::Game () :
    goldToPlay_(FIRSTCOLOR), rounds_(FIRSTMOVE), finished_(false)//, currentMove_(FIRSTMOVE,FIRSTCOLOR)
{
    players_[COLORGOLD] = nullptr;
    players_[COLORSILVER] = nullptr;
}


/**
 * config NONE: no player observed (remote spectator)
 * GOLD: player gold observed (bot or remote)
 * SILVER: player silver observed (bot or remote)
 * ALL: all players observed (local game)
 * view enums.h
 */
void Game::registerObserver (Observer *newObserver, observer_config config )
{
    switch (config) {
    case NONE:
	observerCollectionNone_.push_back(newObserver);
	return;
    case GOLD:
	observerCollection_[COLORGOLD].push_back(newObserver);
	return;
    case SILVER:
	observerCollection_[COLORSILVER].push_back(newObserver);
	return;
    case ALL:
	observerCollectionAll_.push_back(newObserver);
	return;
    default:
	throw std::invalid_argument("Wrong configuration given!");
	return;
    }
}

/**
 * adds also as observer
 */
void Game::addPlayer(Player *newPlayer, bool gold)
{
    int color = color2int(gold);
    if (players_[color] != nullptr)
	throw std::invalid_argument("Already player with this color registered!");
    players_[color] = newPlayer;
    observerCollection_[color].push_back(newPlayer);
}



void Game::playOneRound()
{
    for_each(observerCollectionAll_.begin(), observerCollectionAll_.end(),
	     [&] (Observer* ob) {ob->notify(board_, moves_); });

    for (int i = 0; i < 2; i++) {
	bool finished = false;
	int steps = 0;
	bool gold = int2color(i);
	moves_.push_back(Move(rounds_, gold));
	while(!finished && steps < 4) {
	    /* sanity checks missing */
	    steps++;
	    players_[i]->notify(board_,moves_);
	    Step s = players_[i]->getStep(gold);
	    board_.movePiece(s);
	    moves_.back().addStep(s);
	    //board_.update_possible_moves();
	}
    }
}

void Game::setStartPosition(bool gold)
{
    players_[color2int(gold)]->notify(board_,moves_);
    Move m = players_[color2int(gold)]->getStartPosition(gold);
    // check pieces
    board_.applyInitMove(m);
    moves_.push_back(m);
}

void Game::start()
{
    /* do twice: for gold=true and gold=false */
    setStartPosition(true);
    setStartPosition(false);
    while (!isFinished()) {
	++rounds_;
	playOneRound();
    }
}

bool Game::isFinished()
{
    return finished_;
}

void Game::notifyObserver() const
{
    // if /* steps */ {
    // 	for each observer in observer.list(current_culour)
    // 		     observer.notify(b, curr_step);
    // 	for each observer in observer.list(all)
    // 		     observer.notify(b, curr_step);
    // } else if /* global */ {
    // 	for each observer in each list
    // 		     observer.notify(b, no_diff);
    // } else /* move colour just finished */ {
    // 	for each observer in observer.None
    // 		     observer.notify(b, whole_move);
    // 	for each observer in observer.list(not_current_colour)
    // 		     observer.notify(b, whole_move);
    // }
}

std::ostream &operator<<(std::ostream &os, const Game &game)
{
    os << game.getCurrentMove()
       << std::endl
       << game.board_;
    return os;
}
