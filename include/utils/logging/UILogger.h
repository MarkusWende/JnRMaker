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
    LogMessage Log(const char* format, ...) override;
    LogMessage Log(log_t type, const char* format, ...) override;
    LogMessage LogStatus(const char* format, ...);
    LogMessage LogStatus(log_t type, const char* format, ...);
    LogMessage Popup(const char* format, ...);
    LogMessage Popup(log_t type, const char* format, ...);

    std::shared_ptr<std::vector<LogMessage>> GetLogs() override { return std::make_shared<std::vector<LogMessage>>(logs_); };
    std::shared_ptr<std::vector<LogMessage>> GetStatusLogs();
    bool IsNewStatus() { return new_status_message_; };

private:
    std::vector<LogMessage> logs_;               /**< Contains all messages, that are shared at the moment. */
    std::vector<LogMessage> status_logs_;               /**< Contains all messages, that are shared at the moment. */
    bool new_status_message_;
};

template <typename T>
std::shared_ptr<UILogger>& operator<<(std::shared_ptr<UILogger>& logger, const T& value)
{
    *logger << value;
    return logger;
}