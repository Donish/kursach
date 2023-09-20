#include "meeting.h"

unsigned int meeting::get_id() const
{
    return _id;
}

void meeting::set_id(const unsigned int &value)
{
    _id = value;
}

meeting::meeting_type meeting::get_type() const
{
    return _type;
}

void meeting::set_type(const meeting::meeting_type &value)
{
    _type = value;
}

meeting::format meeting::get_format() const
{
    return _format;
}

void meeting::set_format(const meeting::format &value)
{
    _format = value;
}

std::shared_ptr<flyweight_string> meeting::get_description() const
{
    return _description;
}

void meeting::set_description(const std::string &value)
{
    flyweight_factory &factory = flyweight_factory::get_instance();
    _description = factory.get_flyweight_string(value);
}

std::shared_ptr<flyweight_string> meeting::get_link() const
{
    return _link;
}

void meeting::set_link(const std::string &value)
{
    flyweight_factory &factory = flyweight_factory::get_instance();
    _link = factory.get_flyweight_string(value);
}

std::shared_ptr<flyweight_string> meeting::get_creator_surname() const
{
    return _creator_surname;
}

void meeting::set_creator_surname(const std::string &value)
{
    flyweight_factory &factory = flyweight_factory::get_instance();
    _creator_surname = factory.get_flyweight_string(value);
}

std::shared_ptr<flyweight_string> meeting::get_creator_name() const
{
    return _creator_name;
}

void meeting::set_creator_name(const std::string &value)
{
    flyweight_factory &factory = flyweight_factory::get_instance();
    _creator_name = factory.get_flyweight_string(value);
}

std::shared_ptr<flyweight_string> meeting::get_creator_patronymic() const
{
    return _creator_patronymic;
}

void meeting::set_creator_patronymic(const std::string &value)
{
    flyweight_factory &factory = flyweight_factory::get_instance();
    _creator_patronymic = factory.get_flyweight_string(value);
}

std::shared_ptr<flyweight_string> meeting::get_date() const
{
    return _date;
}

void meeting::set_date(const std::string &value)
{
    flyweight_factory &factory = flyweight_factory::get_instance();
    _date = factory.get_flyweight_string(value);
}

std::shared_ptr<flyweight_string> meeting::get_start_time() const
{
    return _start_time;
}

void meeting::set_start_time(const std::string &value)
{
    flyweight_factory &factory = flyweight_factory::get_instance();
    _start_time = factory.get_flyweight_string(value);
}

std::shared_ptr<flyweight_string> meeting::get_min_duration() const
{
    return _min_duration;
}

void meeting::set_min_duration(const std::string &value)
{
    flyweight_factory &factory = flyweight_factory::get_instance();
    _min_duration = factory.get_flyweight_string(value);
}

std::shared_ptr<flyweight_string> meeting::get_invited_people() const
{
    return _invited_people;
}

void meeting::set_invited_people(const std::string &value)
{
    flyweight_factory &factory = flyweight_factory::get_instance();
    _invited_people = factory.get_flyweight_string(value);
}