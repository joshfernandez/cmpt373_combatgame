//
//  StrColorizer
//  parser
//
//

#include "StrColorizer.hpp"


typedef std::vector<color_token> attribute_string;

bool StrColorizer::is_tag(char c){
    return c == '&';
}

bool StrColorizer::is_space(char c){
    return c == ' '  ||
           c == '\n' ||
           c == '\r' ||
           c == '\t';
}


void StrColorizer::print_color(WINDOW *win, int ypos, int xpos, const std::string &text){

    attribute_string res = str_to_colortoken(text);
    colortoken_print(win,ypos,xpos,res);


}

void StrColorizer::colortoken_print(WINDOW *win, int ypos, int xpos, const attribute_string &att_str){

    int y,x;

    if (win == NULL){

        win = stdscr;

    }

    getyx(win, y, x);

    if(ypos != 0){

        y = ypos;

    }

    if(xpos != 0){

        x = xpos;

    }

    //Initialise all standard color pairs
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_YELLOW,COLOR_BLACK);
    init_pair(4, COLOR_BLUE,COLOR_BLACK);
    init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(6, COLOR_CYAN, COLOR_BLACK);
    init_pair(7, COLOR_WHITE, COLOR_BLACK);

    for(color_token ct: att_str){
        int color = 0;
        switch (ct.charcolor){
            case color_type::RED:
                color = COLOR_PAIR(1);
                break;

            case color_type::GREEN:
                color = COLOR_PAIR(2);
                break;

            case color_type::YELLOW:
                color = COLOR_PAIR(3);
                break;

            case color_type::BLUE:
                color = COLOR_PAIR(4);
                break;

            case color_type::MAGENTA:
                color = COLOR_PAIR(5);
                break;

            case color_type::CYAN:
                color = COLOR_PAIR(6);
                break;

            case color_type::WHITE:
            default:
                color = COLOR_PAIR(7);
                break;
        }

        wattron(win, color);
        mvwaddch(win,y,x,ct.character);
        wattroff(win, color);
        getyx(win, y, x);

    }


    refresh();

}

/*
std::ostream& operator<<(std::ostream& os, const color_token& ct){


    for (color_token ct: res) {

        if (is_space(ct.character)){
            os << "The char is a space "
            << " Color: " << char(ct.charcolor)
            << std::endl;
        } else {

            os << "The char: " << ct.character
            << " Color: " << char(ct.charcolor)
            << std::endl;
        }
    }



    return os;
}
*/

color_type StrColorizer::get_color(char c){

    switch (c){

        case 'l':
            return color_type::BLACK;
            break;

        case 'r':
            return color_type::RED;
            break;

        case 'g':
            return color_type::GREEN;
            break;

        case 'y':
            return color_type::YELLOW;
            break;

        case 'b':
            return color_type::BLUE;
            break;

        case 'm':
            return color_type::MAGENTA;
            break;

        case 'c':
            return color_type::CYAN;
            break;
            
        case 'v':
            return color_type::RAINBOW;
            break;

        default:
            return color_type::WHITE;
            break;
    }

}

color_type StrColorizer::get_looper_color(int i){
    
    
    switch(i){
            
        case 0:
            return color_type::RED;
            break;
            
        case 1:
            return color_type::GREEN;
            break;
            
        case 2:
            return color_type::YELLOW;
            break;
            
        case 3:
            return color_type::BLUE;
            break;
            
        case 4:
            return color_type::MAGENTA;
            break;
            
        case 5:
            return color_type::CYAN;
            break;
            
        case 6:
            return color_type::WHITE;
            break;
            
        default:
            return color_type::WHITE;
            break;
    }
    
}

attribute_string StrColorizer::str_to_colortoken(const std::string &text){

    attribute_string att_str_vector;

    for (int i=0; i < text.size(); i++){

        if(!is_tag(text[i])){
            att_str_vector.push_back(color_token{text[i], color_type::WHITE});

        } else{

            i++;
            color_type c_type = get_color(text[i]);
            i++;

            i = process_tagged_str(i,c_type,text,att_str_vector);

        }

    }

    return att_str_vector;
}


int StrColorizer::process_tagged_str(int i, color_type& c_type, const std::string &text, attribute_string &att_str) {

    while (!is_tag(text[i]) && i< text.size()){
        
        if(c_type == color_type::RAINBOW){
            
            return str_to_rainbow_token(text, i, att_str);
            
        } else {
            
            att_str.push_back(color_token { text[i], c_type});
            i++;
            
        }
    }
    
    return --i; //We return the last char before a tag
}

int StrColorizer::str_to_rainbow_token(const std::string &text, int i, attribute_string &att_str){
    
    int color_code = 0;

    //process until next rainbow tag
    for(int color_loop = color_looper::RED; ; color_loop++){

        if (i >= text.size()) {
            break;
        }

        if (is_tag(text[i])) {
            i++;
            if (get_color(text[i++]) == color_type::RAINBOW) {
                break;
            }
        }
        
        att_str.push_back(color_token {text[i], get_looper_color(color_code)});
        i++;
        color_code ++;
        
        if(color_code > 6) color_code = 0;
        
        if(color_loop == color_looper::WHITE) color_loop = color_looper::RED;
        
    }
    
    return --i;
    
}
