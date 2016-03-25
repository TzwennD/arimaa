#include <algorithm>
#include <vector>
#include <list>
#include <sstream>

#include "Game.hpp"

#include "Observer.hpp"
#include "Player.hpp"
#include "enums.hpp"

using namespace std;

Game::Game () :
    goldToPlay_(FIRSTCOLOR), rounds_(FIRSTMOVE), finished_(false)
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

Step Game::getLastStep(int steps) const
{
    if (steps > 1) {
        Step tmpStep = moves_.back().getStepList().back();
        /* check if last step was a death */
        if (tmpStep.getDirection().getDirection() == TRAPPED) {
            auto i = moves_.back().getStepList().end();
            i--; // last element
            i--; // before last element
            return *i;
        } else
            return tmpStep;
    } // else
    return Step(END);
}

void Game::playOneRound()
{
    for (int i = 0; i < 2; i++) {
        if (finished_)
            break;
        bool finished = false;
        int steps = 0;
        bool gold = int2color(i);
        moves_.push_back(Move(rounds_, gold));
        while(!finished && !finished_ && steps < 4) {
            board_.updatePossibleMoves(gold);
            players_[i]->notify(board_,moves_);

            /* Step validates itself */
            Step s = players_[i]->getStep(gold);
            switch (s.getType()) {
            case BASIC:
            {
                Step lastStep = getLastStep(steps + 1);
                try {
                    Step deadAnimal = board_.movePiece(s, gold, steps + 1,
                                                       lastStep);
                    moves_.back().addStep(s);
                    if (deadAnimal.getType() != END)
                        moves_.back().addStep(deadAnimal);
                } catch (invalid_argument& inv) {
                    stringstream ss;
                    ss << "Error occured: " << inv.what();
                    players_[i]->notifyError(ss.str());
                    continue;
                }
                ++steps;
                break;
            }
            case END:
                finished = true;
                break;
            case RESIGN:
                finished = true;
                finished_ = true;
                winningPlayer_ = !gold;
                break;
            case TAKEBACK:
                //TODO: implement
                throw invalid_argument("not yet implemented");
                break;
            case UNDO:
                throw invalid_argument("not yet implemented");
                if (steps > 0) {
                    moves_.back().removeLastStep();
                    steps--;
                    //TODO:tell board about the undo
                }
                break;
            default:
                throw invalid_argument("should not happen");
            }
        }
        for (auto &o : observerCollectionAll_)
            o->notify(board_, moves_);
        bool color = gold;
        if (board_.endPosition(color)) {
            finished_ = true;
            winningPlayer_ = color;
        }
    }
}

void Game::setStartPosition(bool gold)
{
    if (finished_)
        return;

    players_[color2int(gold)]->notify(board_,moves_);
    Move m = players_[color2int(gold)]->getStartPosition(gold);

    /* check that the right pieces are set */
    vector<char> ref = {'r', 'r', 'r', 'r', 'r', 'r', 'r', 'r',
                        'c', 'c', 'd', 'd', 'h', 'h', 'm', 'e'};
    if (gold) {
        for (vector<char>::iterator it = ref.begin(); it != ref.end(); ++it) {
            *it = toupper(*it);
        }
    }

    list<Step> steps = m.getStepList();

    for (Step s : steps) {
        if (s.getType() == RESIGN) {
            finished_ = true;
            winningPlayer_ = !gold;
            return;
        }
    }

    vector<char> setFigures;
    setFigures.reserve(16);
    for (list<Step>::iterator it = steps.begin(); it != steps.end(); ++it) {
        setFigures.push_back(it->getPiece());
    }
    sort(ref.begin(), ref.end());
    sort(setFigures.begin(), setFigures.end());
    if (!equal(ref.begin(), ref.end(), setFigures.begin())) {
        throw invalid_argument("Not the proper pieces used.");
    }

    /* board does some more validations */
    board_.applyInitMove(m);
    moves_.push_back(m);
}

void Game::start()
{
    /* do twice: for gold=true and gold=false */
    for(int i = 1; i >= 0; --i) {
            try {
                setStartPosition(i);
            } catch (invalid_argument inv) {
                stringstream ss;
                ss << "Error occured: " << inv.what();
                players_[color2int(i)]->notifyError(ss.str());
                board_.emptyStartPosition(i);
                ++i;
            }
    }
    while (!isFinished()) {
        ++rounds_;
        playOneRound();
    }
    for (auto &o : observerCollectionAll_)
        o->notifyGameEnd(board_, moves_, winningPlayer_);
}

bool Game::isFinished()
{
    return finished_;
}

void Game::notifyObserver() const
{
    // if /* steps */ {
    //         for each observer in observer.list(current_culour)
    //                      observer.notify(b, curr_step);
    //         for each observer in observer.list(all)
    //                      observer.notify(b, curr_step);
    // } else if /* global */ {
    //         for each observer in each list
    //                      observer.notify(b, no_diff);
    // } else /* move colour just finished */ {
    //         for each observer in observer.None
    //                      observer.notify(b, whole_move);
    //         for each observer in observer.list(not_current_colour)
    //                      observer.notify(b, whole_move);
    // }
}

std::ostream &operator<<(std::ostream &os, const Game &game)
{
    os << game.getCurrentMove()
       << std::endl
       << game.board_;
    return os;
}
