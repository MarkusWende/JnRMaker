#pragma once

#include "ILogger.h"
#include "FormatHelper.h"
#include "TimeHelper.h"
#include "memory"

#define MAX_STORED_LOGMSG 200               /**< Defines the maximal number of messages to be stored. */

class UILogger : public ILogger {
public:
    LogMessage Log(log_t type, const char* format, ...) override;
    LogMessage Log(const char* format, ...) override;
    std::shared_ptr<std::vector<LogMessage>> GetLogs() const { return std::make_shared<std::vector<LogMessage>>(logs_); };
private:
    std::vector<LogMessage> logs_;               /**< Contains all messages, that are shared at the moment. */
};