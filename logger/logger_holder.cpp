#include "logger_holder.h"

const logger_holder *logger_holder::log_with_guard(const std::string &msg, logger::severity sev_level) const
{
    auto *logger = get_logger();

    if(logger != nullptr)
    {
        logger->log(msg, sev_level);
    }

    return this;
}

const logger_holder *logger_holder::trace_with_guard(const std::string &msg) const
{
    return log_with_guard(msg, logger::severity::trace);
}

const logger_holder *logger_holder::debug_with_guard(const std::string &msg) const
{
    return log_with_guard(msg, logger::severity::debug);
}

const logger_holder *logger_holder::information_with_guard(const std::string &msg) const
{
    return log_with_guard(msg, logger::severity::information);
}

const logger_holder *logger_holder::warning_with_guard(const std::string &msg) const
{
    return log_with_guard(msg, logger::severity::warning);
}

const logger_holder *logger_holder::error_with_guard(const std::string &msg) const
{
    return log_with_guard(msg, logger::severity::error);
}

const logger_holder *logger_holder::critical_with_guard(const std::string &msg) const
{
    return log_with_guard(msg, logger::severity::critical);
}