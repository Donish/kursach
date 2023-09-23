#ifndef OWN_TYPE_H
#define OWN_TYPE_H

#include <iostream>
#include <string>
#include "../flyweight/flyweight_factory.h"

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
private:

    unsigned int _id_meeting;

public:
    
    unsigned int get_id() const;

    void set_id(unsigned int &value);

};

struct value
{
private:

    meeting_type _type;
    format _format;
    std::shared_ptr<flyweight_string> _description;
    std::shared_ptr<flyweight_string> _link;
    std::shared_ptr<flyweight_string> _creator_surname;
    std::shared_ptr<flyweight_string> _creator_name;
    std::shared_ptr<flyweight_string> _creator_patronymic;
    std::shared_ptr<flyweight_string> _date;
    std::shared_ptr<flyweight_string> _start_time;
    std::shared_ptr<flyweight_string> _min_duration;
    std::shared_ptr<flyweight_string> _invited_people;

public:

    unsigned int get_id();

};



#endif //OWN_TYPE_H