//
//  StrColorizer.hpp
//  parser
//
//
//

#ifndef StrColorizer_hpp
#define StrColorizer_hpp

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <ncurses.h>
#include "ascii-converter/AsciiConverter.hpp"

//Standard ncurses colors
enum class color_type: char {

    BLACK   = 'l',
    RED     = 'r' ,
    GREEN   = 'g' ,
    YELLOW  = 'y',
    BLUE    = 'b',
    MAGENTA = 'm',
    CYAN    = 'c',
    WHITE   = 'w',
    RAINBOW = 'v',

};
    
    
enum color_looper{
        
    RED,
    GREEN,
    YELLOW,
    BLUE,
    MAGENTA,
    CYAN,
    WHITE
        
};

struct color_token {

    char character;
    color_type charcolor;
    //Each character has a character color attribute

};

typedef std::vector<color_token> attribute_string;

class StrColorizer {

private:
    static bool is_tag(char c);
    static color_type get_color(char c);
    static color_type get_looper_color(int i);
    static bool is_space(char c);
    static int process_tagged_str(int i, color_type& c_type, const std::string& text, attribute_string& att_str);

    friend std::ostream& operator<<(std::ostream& os, const attribute_string& att_str);



    //-------------Converter-----------------//
    static int str_to_rainbow_token(const std::string& text, int i, attribute_string& att_str);
    static attribute_string str_to_colortoken (const std::string& text);

    //-------------Printer-------------------//
    static void colortoken_print (WINDOW *win, int ypos, int xpos,const attribute_string& att_str);


public:

    //Set xpos and ypos to zero if you dont want custom positions
    //Pasing null win would set win to stdscr (dont pass in null win)
    static void print_color(WINDOW *win, int ypos, int xpos, const std::string& text);


};


#endif /* parse_hpp */
