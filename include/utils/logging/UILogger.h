#pragma once
#include "ILogger.h"
#include "FormatHelper.h"
#include "TimeHelper.h"
#include "memory"

#define MAX_STORED_LOGMSG 200               /**< Defines the maximal number of messages to be stored. */

class UILogger : public ILogger
{
public:
    template <typename T>
    UILogger& operator<<(const T& value)
    {
        std::stringstream out;
        out << value;
        Log("%s", out.str().c_str());
        return *this;
    }

    UILogger();
    LogMessage Log(log_t type, const char* format, ...) override;
    LogMessage Log(const char* format, ...) override;
    std::shared_ptr<std::vector<LogMessage>> GetLogs() const { return std::make_shared<std::vector<LogMessage>>(logs_); };

private:
    std::vector<LogMessage> logs_;               /**< Contains all messages, that are shared at the moment. */
};

template <typename T>
std::shared_ptr<UILogger>& operator<<(std::shared_ptr<UILogger>& logger, const T& value)
{
    *logger << value;
    return logger;
}