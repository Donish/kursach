#ifndef LOGGER_HOLDER_H
#define LOGGER_HOLDER_H

#include "logger.h"

class logger_holder
{
public:

    const logger_holder *log_with_guard(std::string const&, logger::severity) const;

    const logger_holder *trace_with_guard(std::string const&) const;

    const logger_holder *debug_with_guard(std::string const&) const;

    const logger_holder *information_with_guard(std::string const&) const;

    const logger_holder *warning_with_guard(std::string const&) const;

    const logger_holder *error_with_guard(std::string const&) const;

    const logger_holder *critical_with_guard(std::string const&) const;

protected:

    virtual logger *get_logger() const = 0;

public:

    virtual ~logger_holder() = default;

};

#endif //LOGGER_HOLDER_H