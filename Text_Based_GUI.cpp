#include <stdexcept>
#include <vector>
#include <utility>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <string>
#include <unistd.h>

#include "Text_Based_GUI.hpp"

#include "Step.hpp"
#include "Move.hpp"
#include "Board.hpp"
#include "enums.hpp"

using namespace std;

Text_Based_GUI::
Text_Based_GUI()
{
    game.addPlayer(this, true);
    game.addPlayer(this, false);
    game.registerObserver(this, ALL);
}

static pair<int, int> parseSquare(string& s)
{

    /* Sanity checks */
    if (s == "")
        throw invalid_argument("String was empty");
    if (s.length() != 2)
        throw invalid_argument("Not the right size!");
    vector<char> alphabet = {'a','b','c','d','e','f','g','h'};

    /* Column */
    int col;
    vector<char>::iterator it = find(alphabet.begin(), alphabet.end(), s[0]);
    if (it == alphabet.end())
        throw invalid_argument("Wrong column.");
    else
        col = distance(alphabet.begin(), it);

    /* Row */
    int row;
    stringstream ss;
    ss << s[1];
    ss >> row;
    if (row < 1 || row > 8) {
        cout << "Row was " << row << endl;
        throw invalid_argument("Wrong row.");
    }
    row--;

    return make_pair(col, row);
}

static Step parseStep(string& s)
{
    if (s == "")
        throw invalid_argument("String was empty -> last Step already given?");
    if (s == "end")
        return Step(END);
    if (s == "takeback")
        return Step(TAKEBACK);
    if (s == "undo")
        return Step(UNDO);
    if (s == "resign")
        return Step(RESIGN);
    if (s.length() != 4)
        throw invalid_argument("Not the right size!");

    /* sanity checks missing */
    char figure = s[0];
    char direction = s[3];
    string pos =  s.substr(1,2);
    pair<int,int> p = parseSquare(pos);

    return Step(Direction(direction_type(direction)),p.first,p.second,figure);
}

/**
 * ask until order "end"
 * or four correct steps
 * gives color in case two player are one object
 */
Step
Text_Based_GUI::
getStep(bool gold)
{
    bool success = false;
    while(!success) {
        success = true;

        cout << "Player ";
        if (gold)
            cout << "gold";
        else
            cout << "silver";

        cout << ": What is your next step?" << endl;
        string st;
        cin >> st;

        try {
            Step s = parseStep(st);
            cout << "Your step is: " << s << endl;
            return s;
        } catch (invalid_argument& inv) {
            cout << "Retry because of " << inv.what() << endl;
            success = false;
            sleep(1);
            continue;
        }
    }
    throw invalid_argument("not reached!");
}

/*
 * gives color in case two player are one object
 */
Move
Text_Based_GUI::
getStartPosition(bool gold)
{
    list<char> figures = {'r', 'r', 'r', 'r', 'r', 'r', 'r', 'r',
                            'c', 'c', 'd', 'd', 'h', 'h', 'm', 'e'};
    if(gold) {
        for(auto i = figures.begin(); i != figures.end(); i++) {
            *i = toupper(*i);
        }
    }

    cout << "Player ";
    if (gold)
        cout << "gold";
    else
        cout << "silver";
    cout << ": Where to put your pieces?" << endl;
    if(gold)
        cout << "Your start rows are rows 1 and 2. " << endl;
    else
        cout << "Your start rows are rows 7 and 8. " << endl;


    Move m(1, gold);

    while(!figures.empty()) {
        cout << "Following pieces need a start position:" << endl;
        for (auto f : figures)
            cout << f << " ";
        cout << endl;

        cout << "Enter start position:" << endl;
        string input;
        cin >> input;
        if(input == "resign") {
            Step s(RESIGN);
            m.addStep(s);
            return m;
        }
        if(input.length() != 3) {
            cout << "Wrong input length, try again! Format is: Ra1" << endl;
            sleep(1);
            continue;
        }
        char figure = input[0];
        string pos =  input.substr(1,2);
        pair<int,int> p;
        try {
            p = parseSquare(pos);
        } catch (invalid_argument& inv) {
            cout << "Retry because of " << inv.what() << endl;
            sleep(1);
            continue;
        }

        if(gold) {
            if(p.second > 1) {
                cout << "Wrong position, just rows 1 and 2! Try again!" << endl;
                continue;
            }
        }
        else {
            if(p.second < 6) {
                cout << "Wrong position, just rows 7 and 8! Try again!" << endl;
                continue;
            }
        }
        bool found = false;
        //manually remove figures
        for (auto j = figures.begin(); j != figures.end(); j++) {
            if(*j == figure) {
                figures.erase(j);
                found = true;
                break;
            }
        }
        if (!found) {
            cout << "Figure already put or wrong input! Try again!" << endl;
            continue;
        }
        Step s(STAY, p.first, p.second, figure);
        m.addStep(s);
        cout << "Your step is: " << s << endl;
    }
    return m;
}


void
Text_Based_GUI::
notify (Board copy_board, list<Move> changes )
{
    cout << "====================================" << endl
         << "Current game status:" << endl;
    if (changes.empty()) {
        cout << "No moves yet." << endl;
    } else {
        Move& last = changes.back();
        cout << last << endl;
    }
    cout << copy_board << endl;
}

void
Text_Based_GUI::
notifyGameEnd(Board copy_board, std::list<Move> changes, bool winningPlayer)
{
    notify(copy_board, changes);
    cout << "Game over!" << endl;
    cout << "Player ";
    if (winningPlayer)
        cout << "gold";
    else
        cout << "silver";
    cout << " won!" << endl;
}
