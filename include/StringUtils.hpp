#ifndef ADVENTURE2016_STRINGUTILS_HPP
#define ADVENTURE2016_STRINGUTILS_HPP

#include <string>
#include <sstream>
#include <vector>
#include "boost/algorithm/string.hpp"
#include <boost/tokenizer.hpp>

static inline std::pair<std::string, std::string> separateFirstWord(const std::string text) noexcept
{
    const std::string delim = " \t";
    auto delimPos = text.find_first_of(delim);

    std::string firstWord;
    std::string rest;

    if (delimPos == std::string::npos) {
        firstWord = text;
    } else {
        firstWord = text.substr(0, delimPos);
        rest = text.substr(delimPos + 1);
    }

    return std::pair<std::string, std::string>(firstWord, rest);
};

static inline std::vector<std::string> splitString(std::string text)
{
    std::vector<std::string> tokens;
    if(text.empty()) {
        return tokens;
    }

    boost::trim(text);

    using EscapeList = boost::escaped_list_separator<char>;
    boost::tokenizer<EscapeList> tokenizer{text, EscapeList("", " ", "@")}; //@ simple workaround for worldbuilding
    for (const auto& token : tokenizer) {
        tokens.emplace_back(token);
    }

    return tokens;
}

#endif //ADVENTURE2016_STRINGUTILS_HPP
