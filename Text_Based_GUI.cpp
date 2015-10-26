#include <stdexcept>
#include <vector>
#include <utility>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <string>

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
  if (s.length() != 4)
    throw invalid_argument("Not the right size!");
  /* is s == end : throw exception that can be handled by the game to end move*/
  
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
    cout << "Player ";
    if (gold)
	cout << "gold";
    else
	cout << "silver";
    cout << ": What is your next step?" << endl;
    string st;
    cin >> st;
    Step s = parseStep(st);
    return s;
}

/*
 * gives color in case two player are one object 
 */
Move
Text_Based_GUI::
getStartPosition(bool gold)
{
  vector<char> figures = {'r', 'r', 'r', 'r', 'r', 'r', 'r', 'r',
			  'c', 'c', 'd', 'd', 'h', 'h', 'm', 'e'};

  cout << "Player ";
    if (gold)
	cout << "gold";
    else
	cout << "silver";
    cout << ": Where to put your pieces?" << endl;
    if(gold)
      cout << "Your start rows are rows 7 and 8. " << endl;
    else
      cout << "Your start rows are rows 1 and 2. " << endl;


    Move m(1, gold);
    for (unsigned i = 0; i < figures.size(); i++)
      {
	char this_fig = figures[i];
	if (gold)
	  this_fig = toupper(this_fig);
	bool success = false;
	pair<int,int> p;
	while (!success) {
	  success = true;
	  cout << "Where to put piece " << this_fig << ": ";
	  try {
	    string s;
	    cin >> s;
	    p = parseSquare(s);
	  } catch (invalid_argument& inv) {
	    cout << "Retry because of " << inv.what() << endl;
	    success = false;
	    continue;
	  }
	  Step s(STAY, p.first, p.second, this_fig);
	  m.addStep(s);
	}
      }
    return m;
}


void
Text_Based_GUI::
notify (Board copy_board, list<Move> changes )
{
    cout << "Current game status:" << endl;
    if (changes.empty()) {
      cout << "No moves yet." << endl;
    }
    else {  
      Move& last = changes.back();
      cout << last << endl;
    }
    cout << copy_board << endl;
}
