#include "SimpleLogger.h"

void
SimpleLogger::Log(const std::string& message)
{
    logs_.push_back(message);
}

void
SimpleLogger::Print() const
{
    for (const auto& log : logs_)
    {
        std::cout << log << std::endl;
    }
}