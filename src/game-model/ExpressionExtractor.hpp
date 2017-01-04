#ifndef ADVENTURE2016_EXPRESSIONEXTRACTOR_H
#define ADVENTURE2016_EXPRESSIONEXTRACTOR_H

#include <string>
#include <boost/regex.hpp>

static inline bool tryExtractEffectsExpression(const std::string& input, std::string& out) {
    using namespace boost;

    const auto pattern = R"%(^'([^']+).*)%";
    regex effectsRegex{pattern};
    smatch matches;

    if (!regex_match(input, matches, effectsRegex)) {
        return false;
    }

    out = matches.str(1);
    return true;
};

#endif //ADVENTURE2016_EXPRESSIONEXTRACTOR_H
