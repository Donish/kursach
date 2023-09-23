#include "own_type.h"

unsigned int key::get_id() const
{
    return _id_meeting;
}

void key::set_id(unsigned int &value)
{
    _id_meeting = value;
}

meeting_type value::get_meeting_type() const
{
    return _type;
}

void value::set_meeting_type(meeting_type const &value)
{
    _type = value;
}

format value::get_format() const
{
    return _format;
}

void value::set_format(const format &value)
{
    _format = value;
}

std::shared_ptr<flyweight_string> value::get_description() const
{
    return _description;
}

void value::set_description(const std::string &value)
{
    flyweight_factory &factory = flyweight_factory::get_instance();
    _description = factory.get_flyweight_string(value);
}

std::shared_ptr<flyweight_string> value::get_link() const
{
    return _link;
}

void value::set_link(const std::string &value)
{
    flyweight_factory &factory = flyweight_factory::get_instance();
    _link = factory.get_flyweight_string(value);
}

std::shared_ptr<flyweight_string> value::get_creator_surname() const
{
    return _creator_surname;
}

void value::set_creator_surname(const std::string &value)
{
    flyweight_factory &factory = flyweight_factory::get_instance();
    _creator_surname = factory.get_flyweight_string(value);
}

std::shared_ptr<flyweight_string> value::get_creator_name() const
{
    return _creator_name;
}

void value::set_creator_name(const std::string &value)
{
    flyweight_factory &factory = flyweight_factory::get_instance();
    _creator_name = factory.get_flyweight_string(value);
}

std::shared_ptr<flyweight_string> value::get_creator_patronymic() const
{
    return _creator_patronymic;
}

void value::set_creator_patronymic(const std::string &value)
{
    flyweight_factory &factory = flyweight_factory::get_instance();
    _creator_patronymic = factory.get_flyweight_string(value);
}

std::shared_ptr<flyweight_string> value::get_date() const
{
    return _date;
}

void value::set_date(const std::string &value)
{
    flyweight_factory &factory = flyweight_factory::get_instance();
    _date = factory.get_flyweight_string(value);
}

std::shared_ptr<flyweight_string> value::get_start_time() const
{
    return _start_time;
}

void value::set_start_time(const std::string &value)
{
    flyweight_factory &factory = flyweight_factory::get_instance();
    _start_time = factory.get_flyweight_string(value);
}

std::shared_ptr<flyweight_string> value::get_min_duration() const
{
    return _min_duration;
}

void value::set_min_duration(const std::string &value)
{
    flyweight_factory &factory = flyweight_factory::get_instance();
    _min_duration = factory.get_flyweight_string(value);
}

std::shared_ptr<flyweight_string> value::get_invited_people() const
{
    return _invited_people;
}

void value::set_invited_people(const std::string &value)
{
    flyweight_factory &factory = flyweight_factory::get_instance();
    _invited_people = factory.get_flyweight_string(value);
}