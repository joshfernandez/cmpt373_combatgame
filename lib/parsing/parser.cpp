//parser.h

//Zachary Cesaretti
//301235784
//CMPT 373

#include "parser.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <string>
#include <vector>
#include <boost/algorithm/string.hpp>

using std::string;
using std::vector;
using std::stringstream;


//Convert inputted command to uppercase
string Parser::toUpperFunction(string input){
	std::transform(input.begin(), input.end(),input.begin(), ::toupper);
	return input;
}

//split up string into individual words, return a vector of words
vector<string> Parser::stringSplitter(string input){
	char delimiter = ' ';
	
	vector<string> words;
	
	stringstream ss(input);
	string tok;

	while(getline(ss, tok, delimiter)){
		words.push_back(tok);
	}

	return words;
}

//Parser
//Takes in the vector string, returns vector in "ACTION"[][][]"TARGET:"[][][]" format
vector<string> Parser::organizer(vector<string> words){
	
	//Initilize initial vector, first item "action"
	vector<string> organizedWords {"ACTION"};
	for(string word : words){
		for(string action : actions){
			if(word == action){
				organizedWords.push_back(word);
			}
		}
	}
	organizedWords.push_back("TARGET");
	for(string word : words){
		for(string target : targets){
			if(word == target){
				organizedWords.push_back(word);
			}
		}
	}
	return organizedWords;
}



