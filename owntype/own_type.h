#ifndef OWN_TYPE_H
#define OWN_TYPE_H

#include <iostream>
#include <string>

enum meeting_type
{
    DAILY,
    PERIOD,
    INTERVIEW,
    CORPORATIVE
};

enum format
{
    ONLINE,
    OFFLINE
};

struct key
{
    unsigned int _id_meeting;
};

struct value
{
    meeting_type _type;
    format _format;
    std::string _description;
    std::string _link;
    std::string _creator_surname;
    std::string _creator_name;
    std::string _creator_patronymic;
    std::string _date;
    std::string _start_time;
    std::string _min_duration;
    std::string _invited_people;
};

#endif //OWN_TYPE_H