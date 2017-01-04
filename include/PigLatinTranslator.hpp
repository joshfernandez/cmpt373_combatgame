/***************************
 THE PIG LATIN TRANSLATOR
***************************/

/*-------------------------------------------------------------

Inspiration: www.snowcrest.net/donnelly/piglatin.html

The Pig Latin system has the following functionalities:
 - Words that start with a vowel (A, E, I, O, U, Y) have
   "WAY" appended to the end of the word.
 - Words that start with a consonant have all consonant
   letters up to the first vowel moved to the end of the
   word and "AY" is appended.
 - Ensures proper capitalization
 - Correct upper case and lower case formatting
 - Correctly translates "q" followed by a vowel
 - Correctly translates contractions
 - Hyphenated words are treated as two words
 - Words may consist of alphabetic characters only (A-Z and
   a-z)
 - All common punctuation, numerals, symbols, and
   whitespace are not modified.

-------------------------------------------------------------*/



#ifndef ADVENTURE2016_PIGLATINTRANSLATOR_HPP
#define ADVENTURE2016_PIGLATINTRANSLATOR_HPP

#include <string>
#include <vector>
#include <iostream>



/*
 *	MEANINGFUL ASCII INDICES
 */



constexpr int UPPERCASE_A_INDEX = 65;
constexpr int UPPERCASE_Z_INDEX = 90;
constexpr int LOWERCASE_A_INDEX = 97;
constexpr int LOWERCASE_Z_INDEX = 122;
constexpr int LETTER_CASE_CHANGE_INDEX = 32;



/*
 *	SMALL METHODS AND OPERATIONS
 */



static inline bool 
isLetterVowel( const char& character )
{
	return ( character == 'A' || character == 'a' ||
			 character == 'E' || character == 'e' ||
			 character == 'I' || character == 'i' ||
			 character == 'O' || character == 'o' ||
			 character == 'U' || character == 'u' ||
			 character == 'Y' || character == 'y' );
}



static inline bool 
isPunctuation( const char& character )
{
	return ( character == '.' || character == ',' ||
			 character == ':' || character == ';' ||
			 character == '!' || character == '?' ||
			 character == '\'' );
			
}



static inline bool 
isLetterUppercase( const char& letter )
{
	return ( UPPERCASE_A_INDEX <= letter && letter <= UPPERCASE_Z_INDEX );
}



static inline bool 
isLetterLowercase( const char& letter )
{
	return ( LOWERCASE_A_INDEX <= letter && letter <= LOWERCASE_Z_INDEX );
}



static inline bool 
isLetterAlphabetical( const char& character )
{
	return ( isLetterUppercase( character ) ||
			 isLetterLowercase( character ) );
}



static inline bool 
isWordAlphabetical( const std::string& word )
{
	for( char character : word )
	{
		if( ! ( isLetterAlphabetical( character ) || 
				isPunctuation( character )))
		{ return false; }
	}
	return true;
}



static inline bool 
isWordSentencecase( const std::string& word )
{
	const int STARTING_CHARACTER_OF_WORD = 0;
	return isLetterUppercase( word [ STARTING_CHARACTER_OF_WORD ] );
}



static inline bool 
doesWordHavePunctuation( const std::string& word )
{
	const int ENDING_CHARACTER_OF_WORD = word.length() - 1;
	return isPunctuation( word [ ENDING_CHARACTER_OF_WORD ] );
}



static inline bool 
doesWordHaveHyphen( const std::string& word )
{
	for( char character : word )
	{
		if( character == '-' ) return true;
	}
	return false;
}



static inline void 
changeLetterToUppercase( char& letter )
{
	if( isLetterLowercase( letter ) )
	{
		letter = letter - LETTER_CASE_CHANGE_INDEX;
	}
}



static inline void 
changeLetterToLowercase( char& letter )
{
	if( isLetterUppercase( letter ))
	{
		letter = letter + LETTER_CASE_CHANGE_INDEX;
	}
}



static inline void 
changeWordToLowercase( std::string& word )
{
	for( char& letter : word )
	{
		changeLetterToLowercase( letter );
	}
}



static inline int 
findLocationOfFirstVowel( const std::string& word )
{
	// Special procedure where the word starts with 'q' followed by any vowel
	const int STARTING_CHARACTER_OF_WORD = 0;
	if( word[ STARTING_CHARACTER_OF_WORD ] == 'q' && 
		isLetterVowel( word[ 1 ] ) )
	{ return 2; }
	

	// Default procedure
	int firstOccurenceOfVowel = 0;
	for( int wordIterator = 0; wordIterator < word.length(); wordIterator++ )
	{
		if( isLetterVowel( word[ wordIterator ] ) )
		{
			firstOccurenceOfVowel = wordIterator;
			break;
		}
	}

	return firstOccurenceOfVowel;
}



static inline void 
performPigLatinWithConsonantAsFirstLetter( std::string& word )
{
	// Word will change to lowercase and will return back to sentence case
	// at the end of the routine
	bool startsWithCapitalLetter = isWordSentencecase( word );
	if( startsWithCapitalLetter ){ changeWordToLowercase( word ); }


	// Main pig latin procedure
	const int STARTING_CHARACTER_OF_WORD = 0;
	const int SWAPPED_SUBSTRING = findLocationOfFirstVowel( word );

	std::string firstConsonants = word.substr( STARTING_CHARACTER_OF_WORD, 
											   SWAPPED_SUBSTRING );
	word.erase(STARTING_CHARACTER_OF_WORD, SWAPPED_SUBSTRING);
	word += firstConsonants + "ay";


	// Return back to sentence case
	if( startsWithCapitalLetter )
	{
		changeLetterToUppercase( word[ STARTING_CHARACTER_OF_WORD ] ); 
	}
}



static inline void 
performPigLatinWithVowelAsFirstLetter( std::string& word )
{
	word += "way";
}



/*
 *	METHODS THAT PERFORM PIG LATIN STEPS
 */



static inline void 
transformOneWordToPigLatin( std::string& word )
{
	const int STARTING_CHARACTER_OF_WORD = 0;

	if( isWordAlphabetical( word ) && 
	isLetterVowel( word[ STARTING_CHARACTER_OF_WORD ] ) )
	{
		performPigLatinWithVowelAsFirstLetter( word ) ;
	}
	else if( isWordAlphabetical( word ) && 
	! isLetterVowel( word[ STARTING_CHARACTER_OF_WORD ] ) )
	{
		performPigLatinWithConsonantAsFirstLetter( word );
	}
	else
	{
		// Do nothing. All words with non-alphabetic characters
		// will stay the same.
	}
}



static inline void 
transformTwoWordsToPigLatin( std::string& word, const char& separator )
{
	const int STARTING_CHARACTER_OF_WORD = 0;
	const int LOCATION_OF_SEPARATOR = word.find( separator );
	const int END_OF_WORD = word.length();


	// Split the word.
	std::string firstWord 	= word.substr( 	STARTING_CHARACTER_OF_WORD, 
											LOCATION_OF_SEPARATOR 
										 );
	std::string secondWord 	= word.substr(	LOCATION_OF_SEPARATOR + 1, 
											END_OF_WORD 
										 );

	transformOneWordToPigLatin(	firstWord );
	transformOneWordToPigLatin(	secondWord );


	// Concatenate both words back.
	word = "";
	word = firstWord + separator + secondWord;
}



static inline std::vector< std::string > 
splitSentence( const std::string& text )
{
	int nextWord = 0;
	std::vector<std::string> wordArray = {};

    for( int charIterator = 0; charIterator <= text.length(); charIterator++ )
    {

    	// If an entire word is read, pop it out of the sentence and store it
    	// in the wordArray vector.
    	if(text[charIterator] == ' ' || charIterator == text.length())
    	{
    		std::string word = text.substr( nextWord, charIterator - nextWord );
    		wordArray.push_back( word );
    		nextWord = charIterator + 1;
    	}

    }

    return wordArray;
}



static inline std::vector< std::string > 
translateWordsToPigLatin( const std::vector< std::string >& originalWords )
{
	std::vector<std::string> newWords = {};

	for( std::string word : originalWords )
    {
    	// Punctuation will be removed from word and added back
		// at the end of the routine
    	char puncMark;
		bool hasPunctuation = doesWordHavePunctuation( word );
		if( hasPunctuation )
		{
			const int ENDING_CHARACTER_OF_WORD = word.length() - 1;
			puncMark = word[ ENDING_CHARACTER_OF_WORD ];
			word.erase( ENDING_CHARACTER_OF_WORD );
		}


		// Main procedure
		if ( word == "" )
		{
			// Do nothing.
		}
		else if( doesWordHaveHyphen(word) )
		{
			transformTwoWordsToPigLatin( word, '-' );
		}
		else
		{
			transformOneWordToPigLatin( word );
		}


		// Punctuation is added back
    	if(hasPunctuation) { word += puncMark; }
    	
    	newWords.push_back(word);
    }

    return newWords;
}



static inline std::string 
formPigLatinSentence( const std::vector< std::string >& wordArray )
{
	std::string sentence = "";
    for( std::string word : wordArray )
    {
    	sentence += word + " ";
    }

    const int UNNECESSARY_SPACE_OF_SENTENCE = sentence.length() - 1;
    sentence.erase( UNNECESSARY_SPACE_OF_SENTENCE );

    return sentence;
}



/********************
 	MAIN METHOD
 *******************/



static inline std::string 
translateToPigLatin( const std::string& text )
{
	if ( text.empty() )
	{
		return "";
	}

    std::vector<std::string> sentenceSplit = splitSentence(text);
    std::vector<std::string> pigLatinWords = translateWordsToPigLatin(sentenceSplit);
    std::string finalSentence = formPigLatinSentence(pigLatinWords);

    return finalSentence;
}



#endif //ADVENTURE2016_PIGLATINTRANSLATOR_HPP