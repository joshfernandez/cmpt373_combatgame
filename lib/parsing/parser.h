//parser.h

//Zachary Cesaretti
//301235784
//CMPT 373

#include <string>
#include <vector>

using std::string;
using std::vector;

#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED

class Parser {
private:
	vector<string> actions {"GET", "DROP", "USE", "OPEN", "CLOSE", "EXAMINE", "INVENTORY", "LOOK"};
	vector<string> targets {"CAVE", "KITCHEN", "NORTH", "SOUTH", "UP", "DOWN", "SWORD", "GOLD", "TREE", "BUSH"};
public:

	//Converts input to uppercase to avoid direct matching
	string toUpperFunction(string input);

	//Splits string into a vector of words
	vector<string> stringSplitter(string input);

	//Returns a vector with "ACTION:"[][][]"TARGET:"[][][]"
	vector<string> organizer(vector<string> words);

};

#endif