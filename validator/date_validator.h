#ifndef DATE_VALIDATOR_H
#define DATE_VALIDATOR_H

#include <iostream>
#include <vector>
#include "digit_validator.h"

class date_validator
{

private:

    digit_validator _digit_validator;

public:

    bool operator()(std::string const &);

private:

    std::vector<std::string> split(std::string const & text, char delim) const;

};

#endif //DATE_VALIDATOR_H