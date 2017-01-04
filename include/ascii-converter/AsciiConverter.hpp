/*
 - Image Art generator library from www.mcs.csueastbay.edu/~tebo/classes/1160/ascii-art/
 - Modified a bit tailored to Bebinca's Adventure game
 - Class generates two things: an ascii art of Bmp img and ascii art of text
 - Find/Place source images in assets/bmpimgs
 - Find/Place font txt files in assets/fontsets/<fontname>/<letter>.txt
 - Object list,Image specifications and Font Usage on WIKI [link:https://csil-git1.cs.surrey.sfu.ca/373-16-3-bebinca/adventure2016/wikis/AsciiConverter-Class ]
 */

#ifndef AsciiConverter_hpp
#define AsciiConverter_hpp

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include <string>
#include <algorithm>
#include <vector>


#define MAX_SHADES 10
#define MAX_WORD_COUNT 10
#define DEBUG 1
#define MAX_SEQ 65

using namespace std;

struct animation_type{
    char anim_type[MAX_SHADES];
};

class AsciiConverter {
    
    private:
    
    //Types to hold BMP File header data
    typedef unsigned short uint16;
    typedef unsigned int uint32;
    
    //BMP File header format Ref: paulbourke.net/dataformats/bmp/
    typedef struct bmp_file_header{
        uint16 filetype; // BM
        uint32 filesize; // in 32-bit integers
        uint32 reserved; // must be 0
        uint32 offset;	  // byte offset to start of data
        uint32 bytesInHeader;	// 40
        uint32 width;			// in pixels
        uint32 height;			// in pixels
        uint16 planes;			// 1
        uint16 bitsPerPixel;	// 1,4,8, or 24
        uint32 compression;	// 0 = none, 1 = 8bit RLE, 2 = 4 bit RLE
        uint32 size;			// of image, in bytes
        uint32 horizRes;		// in pixels/m
        uint32 vertRes;		//      "
        uint32 indicesUsed;	// # color indices used by the bitmap
        uint32 indicesImportant; // # important indices (0=all)
    }BMPFileHeader;
    
    //String to print both image and font and return it
    string ascii_print;
    
    //Functions -------
    
    //Extract File Data
    static uint16 extractShort(ifstream &f);
    static uint32 extractInt(ifstream &f);
    
    static void readHeader(ifstream& f, BMPFileHeader& header);
    
    
    static string get_loc(const string &font_type);
    static void to_lower(string& word);
    static void process_letter_location(const char &c, string& word_file);
    static int get_word_size(string& word);
    static string process_font_type(const string& font_type);

    static void set_anim_splitter(int i, int& splitter);
    
    //Methods supporting word streams upto  10 letters. Multiple methods so that memory (allocation) is not
    //wasted if unnecessary
    //-----Function moved to public for debug purpose--------//
    static string word1(const string font_type, const string& word);
    static string word2(const string font_type, const string& word);
    static string word3(const string font_type, const string& word);
    static string word4(const string font_type, const string& word);
    static string word5(const string font_type, const string& word);
    static string word6(const string font_type, const string& word);
    static string word7(const string font_type, const string& word);
    static string word8(const string font_type, const string& word);
    static string word9(const string font_type, const string& word);
    static string word10(const string font_type, const string& word);
    
    //To be added
    // static string sentence_helper(const string font_type, const string& sentence);
    
    //Animation
    static string animation_helper(const string& objname, char anim_frame[MAX_SHADES], const string& file);
    static void set_sequence_filepath(vector<string>& filepath_seq, const string&objname);
    
    
    
    public:
    
    //Print Object: Returns a Multitext string of ASCII art of an object in the assets/bmpimgs folder
    //Styles: default, numbers
    //printObject0: Call this method if object argument being passed is a variable (type:string)
    static string convertObject0(const string style,const string &objname);
    //printObject1: Call this method if the object string is directly typed in function call
    static string convertObject1(const string style,const string objname);
    
    
    //Print Word: Returns a Multitext string of ASCII art of the word
    //Font Types: (...to be upadted)
    static string convertWord0(const string &font_type, const string &word);
    static string convertWord1(const string font_type, const string word);
    
    //-------------For Debug purpose----------------
    static string wordn(const string font_type, const string& word);
    //-----------------------------------------------
    
    
    //To be added
    // static string printSentence(const string font_type, const string &sentence);
    // static string printSentence(const string font_type, const string sentence);
    
    //Animation
    static vector<string> animateObject(const string &objname);
    static vector<string> animateSequence(const string &objname);
    
};

#endif /* AsciiConverter_hpp */
