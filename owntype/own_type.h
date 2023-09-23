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

    void set_id(unsigned int const &value);

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

    meeting_type get_meeting_type() const;
    void set_meeting_type(meeting_type const &value);

    format get_format() const;
    void set_format(format const &value);

    std::shared_ptr<flyweight_string> get_description() const;
    void set_description(std::string const &value);

    std::shared_ptr<flyweight_string> get_link() const;
    void set_link(std::string const &value);

    std::shared_ptr<flyweight_string> get_creator_surname() const;
    void set_creator_surname(std::string const &value);

    std::shared_ptr<flyweight_string> get_creator_name() const;
    void set_creator_name(std::string const &value);

    std::shared_ptr<flyweight_string> get_creator_patronymic() const;
    void set_creator_patronymic(std::string const &value);

    std::shared_ptr<flyweight_string> get_date() const;
    void set_date(std::string const &value);

    std::shared_ptr<flyweight_string> get_start_time() const;
    void set_start_time(std::string const &value);

    std::shared_ptr<flyweight_string> get_min_duration() const;
    void set_min_duration(std::string const &value);

    std::shared_ptr<flyweight_string> get_invited_people() const;
    void set_invited_people(std::string const &value);

};

#endif //OWN_TYPE_H