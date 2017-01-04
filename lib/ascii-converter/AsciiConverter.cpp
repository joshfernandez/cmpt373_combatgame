//AsciiConverter

#include "ascii-converter/AsciiConverter.hpp"


#define MAX_SHADES 10
#define MAX_WORD_COUNT 10
#define DEBUG 1
#define MAX_SEQ 65

typedef unsigned short uint16;
typedef unsigned int uint32;

using namespace std;

uint16 AsciiConverter::extractShort (ifstream &f)
{
    char buf[2];
    f.read (buf, 2);
    uint16 value = buf[0] | (buf[1] << 8);
    return value;
}

uint32 AsciiConverter::extractInt (ifstream &f)
{
    
    char buf[4];
    f.read (buf, 4);
    uint32 value = buf[0] | (buf[1] << 8) | (buf[2] << 16) | (buf[3] << 24);
    return value;
}

void AsciiConverter::to_lower(string& word){
    
    transform(word.begin(), word.end(), word.begin(), ::tolower);
}

string AsciiConverter::get_loc(const string &font_type){
    
    string type = font_type;
    to_lower(type);
    
    const string find_loc = "data//assets/fontsets/" + type + "/";
    return find_loc;
}

void AsciiConverter::process_letter_location(const char &c, string &word_file){
    
    word_file += c;
    word_file += ".txt";
    
}

int AsciiConverter::get_word_size(string &word){
    long long_word = word.size();
    const int word_size = int(long_word);
    return word_size;
}

string AsciiConverter::process_font_type(const string &font_type){
    
    string type = font_type;
    to_lower(type);
    
    if(type == "default"){
        return "default";
    } else if (type == "test"){
        return "test";
    } else if (type == "dark"){
        return "dark";
    }//Add more font types here
    
    return "default";
}

void AsciiConverter::readHeader(ifstream &f, BMPFileHeader &header){
    
    header.filetype = extractShort (f);
    header.filesize = extractInt (f);
    header.reserved = extractInt (f);
    header.offset = extractInt (f);
    header.bytesInHeader = extractInt (f);
    header.width = extractInt (f);
    header.height = extractInt (f);
    header.planes = extractShort (f);
    header.bitsPerPixel = extractShort (f);
    header.compression = extractInt (f);
    header.size = extractInt (f);
    header.horizRes = extractInt (f);
    header.vertRes = extractInt (f);
    header.indicesUsed = extractInt (f);
    header.indicesImportant = extractInt (f);
    
    /* DEBUG FLAGS------
     #ifdef DEBUG
     cout << "filetype " << header.filetype << endl;
     cout << "filesize " << header.filesize << endl;
     cout << "reserved " << header.reserved << endl;
     cout << "offset " << header.offset << endl;
     cout << "bytesInHeader " << header.bytesInHeader << endl;
     cout << "width " << int(header.width) << endl;
     cout << "height " << int(header.height) << endl;
     cout << "planes " << header.planes << endl;
     cout << "bitsPerPixel " << header.bitsPerPixel << endl;
     cout << "compression " << header.compression << endl;
     cout << "size " << header.size << endl;
     cout << "horizRes " << header.horizRes << endl;
     cout << "vertRes " << header.vertRes << endl;
     cout << "indicesUsed " << header.indicesUsed << endl;
     cout << "indicesImportant " << header.indicesImportant << endl;
     #endif
     */
    
}

string AsciiConverter::convertObject1(const string style, const string objname){
    return convertObject0(style,objname);
}

string AsciiConverter::convertObject0(const string style, const string &objname){
    
    int width, height;
    unsigned char *image;
    char def_shades[MAX_SHADES] = {'#','$','O','=','+','|','-','^','.',' '};
    char num_shades[MAX_SHADES] = {'0','1','2','3','4','5','6','7','8','.'};
    
    // = {'#','$','O','=','+','|','-','^','.',' '};
    // = {'0','1','2','3','4','5','6','7','8','9'};
    int average_color = 0;
    
    
    string ret_string;
    ifstream bmpfile;
    BMPFileHeader header;
    
    const string filepath = "data/assets/bmpimgs/" + move(objname) + ".bmp";
    
    // Open the image file
    bmpfile.open (filepath, ios::in | ios::binary);
    if ( ! bmpfile ) {
        //cout << "cannot open bmp file" << endl;
        return "File not found";
    }
    
    // Read header
    readHeader (bmpfile, header);
    
    // Read image
    width = int(header.width);
    if ( width < 0 )
        width *= -1;
    height = int(header.height);
    if ( height < 0 )
        height *= -1;
    
    int rowsize = width * 3;
    
    image = new unsigned char [ rowsize * height ];
    
    bmpfile.seekg ( header.offset, ios::beg );
    bmpfile.read ( (char *)image, 3*width*height );
    bmpfile.close();
    
    //char tempoutput[3][3];
    //string temo;
    for(int y = height-1; y >= 0; y--) {
        
        for(int x = 0; x < width; x++) {
            
            // Get the average color
            average_color = ( image[x*3     + y*rowsize] +
                             image[x*3 + 1 + y*rowsize] +
                             image[x*3 + 2 + y*rowsize] ) / 3;
            
            // Convert to a shade
            average_color /= (256/MAX_SHADES);
            if(average_color >= MAX_SHADES)
                average_color = MAX_SHADES-1;
            
            // Write Output to string
            if(style == "default"){
                ret_string += def_shades[average_color];
            } else if (style == "numbers"){
                ret_string += num_shades[average_color];
            } else {
                ret_string += def_shades[average_color];
            }
            
        }
        
        ret_string += "\n";
        
    }
    
    return ret_string;

}


string AsciiConverter::word1(const string font_type, const string& word){
    
    string ret_string;
    string sf_1 = get_loc(font_type);
    string w_1 = word;
    to_lower(w_1);
    
    process_letter_location(w_1[0], sf_1);
    
    /* DEBUG Flag
     #ifdef DEBUG
     cout << "The full location is " << sf_1 << endl;
     #endif
     */
    
    ifstream pf_1(sf_1);
    string pc_1;
    
    if(pf_1.is_open()){
        while (getline(pf_1, pc_1)){
            ret_string += pc_1;
            
            ret_string += "\n";
            
            /* DEBUG FLAGS
             #ifdef DEBUG
             cout << pc_1
             << "\n";
             #endif
             */
            
        }
        pf_1.close();
    }
    
    return ret_string;
}


string AsciiConverter::word2(const string font_type, const string& word){
    
    string ret_string;
    
    string w_1 = word;
    to_lower(w_1);
    
    vector<string> sf;
    for (int i=0; i < get_word_size(w_1); i++){
        sf.push_back(get_loc(font_type));
        process_letter_location(w_1[i], sf[i]);
    }
    
    
    /*DEBUG flag
     #ifdef DEBUG
     for(string s:sf){
     cout << "The element: " << s << endl;
     }
     #endif
     */
    
    
    
    ifstream pf[2];
    for(int i=0; i < get_word_size(w_1); i++){
        pf[i].open((sf[i]));
    }
    
    string pc[2];
    
    if(pf[0].is_open()){
        while (getline(pf[0],pc[0])){
            getline(pf[1],pc[1]);
            
            for(int i=0; i < get_word_size(w_1); i++){
                ret_string += pc[i];
            }
            
            ret_string += "\n";
        }
        
        for(int i=0; i < get_word_size(w_1); i++){
            pf[i].close();
        }
    }
    
    return ret_string;
}


string AsciiConverter::word3(const string font_type, const string& word){
    
    string ret_string;
    
    string w_1 = word;
    to_lower(w_1);
    
    vector<string> sf;
    for (int i=0; i < get_word_size(w_1); i++){
        sf.push_back(get_loc(font_type));
        process_letter_location(w_1[i], sf[i]);
    }
    
    
    ifstream pf[3];
    for(int i=0; i < get_word_size(w_1); i++){
        pf[i].open((sf[i]));
    }
    
    string pc[3];
    
    if(pf[0].is_open()){
        while (getline(pf[0],pc[0])){
            
            for (int i=1; i< get_word_size(w_1); i++) {
                getline(pf[i], pc[i]);
            }
            
            for(int i=0; i < get_word_size(w_1); i++){
                ret_string += pc[i];
            }
            
            ret_string += "\n";
        }
        
        for(int i=0; i < get_word_size(w_1); i++){
            pf[i].close();
        }
    }
    
    return ret_string;
}

string AsciiConverter::word4(const string font_type, const string& word){
    
    string ret_string;
    
    string w_1 = word;
    to_lower(w_1);
    
    vector<string> sf;
    for (int i=0; i < get_word_size(w_1); i++){
        sf.push_back(get_loc(font_type));
        process_letter_location(w_1[i], sf[i]);
    }
    
    
    ifstream pf[4];
    for(int i=0; i < get_word_size(w_1); i++){
        pf[i].open((sf[i]));
    }
    
    string pc[4];
    
    if(pf[0].is_open()){
        while (getline(pf[0],pc[0])){
            
            for (int i=1; i< get_word_size(w_1); i++) {
                getline(pf[i], pc[i]);
            }
            
            for(int i=0; i < get_word_size(w_1); i++){
                ret_string += pc[i];
            }
            
            ret_string += "\n";
        }
        
        for(int i=0; i < get_word_size(w_1); i++){
            pf[i].close();
        }
    }
    
    return ret_string;
}

string AsciiConverter::word5(const string font_type, const string& word){
    
    string ret_string;
    
    string w_1 = word;
    to_lower(w_1);
    
    vector<string> sf;
    for (int i=0; i < get_word_size(w_1); i++){
        sf.push_back(get_loc(font_type));
        process_letter_location(w_1[i], sf[i]);
    }
    
    
    ifstream pf[5];
    for(int i=0; i < get_word_size(w_1); i++){
        pf[i].open((sf[i]));
    }
    
    string pc[5];
    
    if(pf[0].is_open()){
        while (getline(pf[0],pc[0])){
            
            for (int i=1; i< get_word_size(w_1); i++) {
                getline(pf[i], pc[i]);
            }
            
            for(int i=0; i < get_word_size(w_1); i++){
                ret_string += pc[i];
            }
            
            ret_string += "\n";
        }
        
        for(int i=0; i < get_word_size(w_1); i++){
            pf[i].close();
        }
    }
    
    return ret_string;
}

string AsciiConverter::word6(const string font_type, const string& word){
    
    string ret_string;
    
    string w_1 = word;
    to_lower(w_1);
    
    vector<string> sf;
    for (int i=0; i < get_word_size(w_1); i++){
        sf.push_back(get_loc(font_type));
        process_letter_location(w_1[i], sf[i]);
    }
    
    
    ifstream pf[6];
    for(int i=0; i < get_word_size(w_1); i++){
        pf[i].open((sf[i]));
    }
    
    string pc[6];
    
    if(pf[0].is_open()){
        while (getline(pf[0],pc[0])){
            
            for (int i=1; i< get_word_size(w_1); i++) {
                getline(pf[i], pc[i]);
            }
            
            for(int i=0; i < get_word_size(w_1); i++){
                ret_string += pc[i];
            }
            
            
            ret_string += "\n";
        }
        
        for(int i=0; i < get_word_size(w_1); i++){
            pf[i].close();
        }
    }
    
    return ret_string;
}


string AsciiConverter::word7(const string font_type, const string& word){
    
    string ret_string;
    
    string w_1 = word;
    to_lower(w_1);
    
    vector<string> sf;
    for (int i=0; i < get_word_size(w_1); i++){
        sf.push_back(get_loc(font_type));
        process_letter_location(w_1[i], sf[i]);
    }
    
    
    ifstream pf[7];
    for(int i=0; i < get_word_size(w_1); i++){
        pf[i].open((sf[i]));
    }
    
    string pc[7];
    
    if(pf[0].is_open()){
        while (getline(pf[0],pc[0])){
            
            for (int i=1; i< get_word_size(w_1); i++) {
                getline(pf[i], pc[i]);
            }
            
            for(int i=0; i < get_word_size(w_1); i++){
                ret_string += pc[i];
            }
            
            
            ret_string += "\n";
        }
        
        for(int i=0; i < get_word_size(w_1); i++){
            pf[i].close();
        }
    }
    
    return ret_string;
}

string AsciiConverter::word8(const string font_type, const string& word){
    
    string ret_string;
    
    string w_1 = word;
    to_lower(w_1);
    
    vector<string> sf;
    for (int i=0; i < get_word_size(w_1); i++){
        sf.push_back(get_loc(font_type));
        process_letter_location(w_1[i], sf[i]);
    }
    
    
    ifstream pf[8];
    for(int i=0; i < get_word_size(w_1); i++){
        pf[i].open((sf[i]));
    }
    
    string pc[8];
    
    if(pf[0].is_open()){
        while (getline(pf[0],pc[0])){
            
            for (int i=1; i< get_word_size(w_1); i++) {
                getline(pf[i], pc[i]);
            }
            
            for(int i=0; i < get_word_size(w_1); i++){
                ret_string += pc[i];
            }
            
            
            ret_string += "\n";
        }
        
        for(int i=0; i < get_word_size(w_1); i++){
            pf[i].close();
        }
    }
    
    return ret_string;
}

string AsciiConverter::word9(const string font_type, const string& word){
    
    string ret_string;
    
    string w_1 = word;
    to_lower(w_1);
    
    vector<string> sf;
    for (int i=0; i < get_word_size(w_1); i++){
        sf.push_back(get_loc(font_type));
        process_letter_location(w_1[i], sf[i]);
    }
    
    
    ifstream pf[9];
    for(int i=0; i < get_word_size(w_1); i++){
        pf[i].open((sf[i]));
    }
    
    string pc[9];
    
    if(pf[0].is_open()){
        while (getline(pf[0],pc[0])){
            
            for (int i=1; i< get_word_size(w_1); i++) {
                getline(pf[i], pc[i]);
            }
            
            for(int i=0; i < get_word_size(w_1); i++){
                ret_string += pc[i];
            }
            
            
            ret_string += "\n";
        }
        
        for(int i=0; i < get_word_size(w_1); i++){
            pf[i].close();
        }
    }
    
    return ret_string;
}


string AsciiConverter::word10(const string font_type, const string& word){
    
    string ret_string;
    
    string w_1 = word;
    to_lower(w_1);
    
    vector<string> sf;
    for (int i=0; i < get_word_size(w_1); i++){
        sf.push_back(get_loc(font_type));
        process_letter_location(w_1[i], sf[i]);
    }
    
    
    ifstream pf[10];
    for(int i=0; i < get_word_size(w_1); i++){
        pf[i].open((sf[i]));
    }
    
    string pc[10];
    
    if(pf[0].is_open()){
        while (getline(pf[0],pc[0])){
            
            for (int i=1; i< get_word_size(w_1); i++) {
                getline(pf[i], pc[i]);
            }
            
            for(int i=0; i < get_word_size(w_1); i++){
                ret_string += pc[i];
            }
            
            
            ret_string += "\n";
        }
        
        for(int i=0; i < get_word_size(w_1); i++){
            pf[i].close();
        }
    }
    
    return ret_string;
}




string AsciiConverter::convertWord0(const string &font_type, const string &word){
    
    string type = process_font_type(move(font_type));
    string processed_word = move(word);
    
    bool flag = false;
    
    for(char c:word){
        if (c == ' '){
            flag = true;
        }
    }
    
    
    if(flag){
        return "words cant be sentences";
    }
    
    const int count = get_word_size(processed_word);
    
    if(count > MAX_WORD_COUNT){
        return "Word size is too long";
    }
    
    
    switch (count) {
        case 0:
            processed_word = "Blank word";
            break;
            
        case 1:
            processed_word = word1(type, processed_word);
            break;
            
        case 2:
            processed_word = word2(type, processed_word);
            break;
            
        case 3:
            processed_word = word3(type, processed_word);
            break;
            
        case 4:
            processed_word = word4(type, processed_word);
            break;
            
        case 5:
            processed_word = word5(type, processed_word);
            break;
            
        case 6:
            processed_word = word6(type, processed_word);
            break;
            
        case 7:
            processed_word = word7(type, processed_word);
            break;
            
        case 8:
            processed_word = word8(type, processed_word);
            break;
            
        case 9:
            processed_word = word9(type, processed_word);
            break;
            
        case 10:
            processed_word = word10(type, processed_word);
            break;
            
        default:
            processed_word = "Error";
            break;
    }
    
    return processed_word;
}

/*
 string AsciiConverter::convertWord1(const string font_type, const string word){
 
 string type = process_font_type(font_type);
 return convertWord0(font_type, word);
 }
 */

string AsciiConverter::animation_helper(const string &objname, char anim_frame[MAX_SHADES], const string& file){
    
    int width, height;
    unsigned char *image;
    // = {'#','$','O','=','+','|','-','^','.',' '};
    // = {'0','1','2','3','4','5','6','7','8','9'};
    int average_color = 0;
    
    
    string ret_string;
    ifstream bmpfile;
    BMPFileHeader header;
    
    string filepath = file;
    
    // Open the image file
    bmpfile.open (filepath, ios::in | ios::binary);
    if ( ! bmpfile ) {
        //cout << "cannot open bmp file" << endl;
        return "File not found";
    }
    
    // Read header
    readHeader (bmpfile, header);
    
    // Read image
    width = int(header.width);
    if ( width < 0 )
        width *= -1;
    height = int(header.height);
    if ( height < 0 )
        height *= -1;
    
    int rowsize = width * 3;
    
    image = new unsigned char [ rowsize * height ];
    
    bmpfile.seekg ( header.offset, ios::beg );
    bmpfile.read ( (char *)image, 3*width*height );
    bmpfile.close();
    
    //char tempoutput[3][3];
    //string temo;
    for(int y = height-1; y >= 0; y--) {
        
        for(int x = 0; x < width; x++) {
            
            // Get the average color
            average_color = ( image[x*3     + y*rowsize] +
                             image[x*3 + 1 + y*rowsize] +
                             image[x*3 + 2 + y*rowsize] ) / 3;
            
            // Convert to a shade
            average_color /= (256/MAX_SHADES);
            if(average_color >= MAX_SHADES)
                average_color = MAX_SHADES-1;
            
            // Write Output to string
            
            ret_string += anim_frame[average_color];
            
        }
        
        ret_string += "\n";
        
    }
    
    return ret_string;
    
    
}

void AsciiConverter::set_anim_splitter(int i, int &splitter) {

    if(i%11 ==0)
        splitter = 1;

}

vector<string> AsciiConverter::animateObject(const string &objname){
    
    vector<string> string_animation_frames;
    vector<animation_type> char_types;
    const int frames = 11;
    const string filepath = "data/assets/bmpimgs/" + move(objname) + ".bmp";
    
    //vector<animation_frame> anim_frames[framespersec];
    
    // vector<animation_array> anim_arr;
    // vector<char[MAX_SHADES]> animation_frames;
    
    //Init 4 char array types and a final type
    char_types.push_back({'-','-','-','-','-','-','-','-','-','-'});
    char_types.push_back({'.',' ',' ',' ',' ',' ',' ',' ','.',' '});
    char_types.push_back({'^',' ',' ','#',' ',' ',' ','#',' ',' '});
    char_types.push_back({'.','^','.','^','.','^','.','^','.',' '});
    char_types.push_back({' ',' ','^',' ','^',' ',' ','#',' ',' '});
    char_types.push_back({' ','q',' ',' ','*',' ',' ',' ',' ',' '});
    char_types.push_back({'c',' ',' ','!',' ',' ',' ',' ',')',' '});
    char_types.push_back({' ',' ','^',' ',' ',']',' ',' ',' ','-'});
    char_types.push_back({' ',' ',' ',' ',' ',' ',' ','.',' ',' '});
    char_types.push_back({'c','q','^','!','*',']','[','.',')',' '});
    
    //Final
    char_types.push_back({'#','$','O','=','+','|','-','^','.',' '});
    
    
    int splitter = 1;
    for (int i=0; i < frames-1; i++){
        
        string_animation_frames.push_back(animation_helper(objname, char_types[splitter].anim_type, filepath));
        
        splitter++;
        set_anim_splitter(i, splitter);
    }
    
    string_animation_frames.push_back(animation_helper(objname, char_types[10].anim_type, filepath));
    
    
    return string_animation_frames;
    
}

void AsciiConverter::set_sequence_filepath(vector<string> &filepath_seq, const string& objname){
    
    for(int i=0; i< MAX_SEQ; i++){
        string sq = to_string(i);
        string push = "data/assets/bmpimgs/sequences/" + move(objname) +
        "/" + sq + ".bmp";
        filepath_seq.push_back(push);
    }
    
}

vector<string> AsciiConverter::animateSequence(const string &objname){
    
    vector<string> string_animation_frames;
    
    vector<string> filepath_sequence;
    set_sequence_filepath(filepath_sequence, move(objname));
    
    char def_shades[MAX_SHADES] = {'#','$','O','=','+','|','-','^','.',' '};
    
    for(int i=0; i< MAX_SEQ; i++){
        string_animation_frames.push_back(animation_helper(objname, def_shades, filepath_sequence[i]));
    }
    
    
    return string_animation_frames;
    
}

