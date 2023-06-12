#include "UILogger.h"

UILogger::UILogger()
{

}

LogMessage
UILogger::Log(const char* format, ...)
{
    va_list args;
    va_start(args, format);

    std::string buffer = FormatHelper::vformat(format, args);
    int n = buffer.size();
    va_end(args);

	std::stringstream out;
    out.write(buffer.data(), n);

    LogMessage tmpMsg;

	tmpMsg.msg = out.str();
	tmpMsg.timeinfo = TimeHelper::GetTimeinfo();
	tmpMsg.type = log_t::INFO;
	tmpMsg.popup = false;

	logs_.insert(logs_.begin(), tmpMsg);

	if (logs_.size() > MAX_STORED_LOGMSG)
		logs_.pop_back();

	out.str("");

    return tmpMsg;
}

LogMessage
UILogger::Log(log_t type, const char* format, ...)
{
    va_list args;
    va_start(args, format);

    std::string buffer = FormatHelper::vformat(format, args);
    int n = buffer.size();
    va_end(args);

	std::stringstream out;
    out.write(buffer.data(), n);

    LogMessage tmpMsg;

	tmpMsg.msg = out.str();
	tmpMsg.timeinfo = TimeHelper::GetTimeinfo();
	tmpMsg.type = type;
	tmpMsg.popup = false;

	logs_.insert(logs_.begin(), tmpMsg);

	if (logs_.size() > MAX_STORED_LOGMSG)
		logs_.pop_back();

	out.str("");

    return tmpMsg;
}

LogMessage
UILogger::LogStatus(const char* format, ...)
{
	va_list args;
    va_start(args, format);

    std::string buffer = FormatHelper::vformat(format, args);
    int n = buffer.size();
    va_end(args);

	std::stringstream out;
    out.write(buffer.data(), n);

    LogMessage tmpMsg;

	tmpMsg.msg = out.str();
	tmpMsg.timeinfo = TimeHelper::GetTimeinfo();
	tmpMsg.type = log_t::INFO;
	tmpMsg.popup = false;

	status_logs_.insert(status_logs_.begin(), tmpMsg);

	if (status_logs_.size() > MAX_STORED_LOGMSG)
		status_logs_.pop_back();

	out.str("");
	new_status_message_ = true;

    return tmpMsg;
}

LogMessage
UILogger::LogStatus(log_t type, const char* format, ...)
{
	va_list args;
    va_start(args, format);

    std::string buffer = FormatHelper::vformat(format, args);
    int n = buffer.size();
    va_end(args);

	std::stringstream out;
    out.write(buffer.data(), n);

    LogMessage tmpMsg;

	tmpMsg.msg = out.str();
	tmpMsg.timeinfo = TimeHelper::GetTimeinfo();
	tmpMsg.type = type;
	tmpMsg.popup = false;

	status_logs_.insert(status_logs_.begin(), tmpMsg);

	if (status_logs_.size() > MAX_STORED_LOGMSG)
		status_logs_.pop_back();

	out.str("");
	new_status_message_ = true;

    return tmpMsg;
}

LogMessage
UILogger::Popup(const char* format, ...)
{
	va_list args;
    va_start(args, format);

    std::string buffer = FormatHelper::vformat(format, args);
    int n = buffer.size();
    va_end(args);

	std::stringstream out;
    out.write(buffer.data(), n);

    LogMessage tmpMsg;

	tmpMsg.msg = out.str();
	tmpMsg.timeinfo = TimeHelper::GetTimeinfo();
	tmpMsg.type = log_t::INFO;
	tmpMsg.popup = true;

	status_logs_.insert(status_logs_.begin(), tmpMsg);

	if (status_logs_.size() > MAX_STORED_LOGMSG)
		status_logs_.pop_back();

	out.str("");
	new_status_message_ = true;

    return tmpMsg;
}

LogMessage
UILogger::Popup(log_t type, const char* format, ...)
{
	va_list args;
    va_start(args, format);

    std::string buffer = FormatHelper::vformat(format, args);
    int n = buffer.size();
    va_end(args);

	std::stringstream out;
    out.write(buffer.data(), n);

    LogMessage tmpMsg;

	tmpMsg.msg = out.str();
	tmpMsg.timeinfo = TimeHelper::GetTimeinfo();
	tmpMsg.type = type;
	tmpMsg.popup = true;

	status_logs_.insert(status_logs_.begin(), tmpMsg);

	if (status_logs_.size() > MAX_STORED_LOGMSG)
		status_logs_.pop_back();

	out.str("");
	new_status_message_ = true;

    return tmpMsg;
}

std::shared_ptr<std::vector<LogMessage>>
UILogger::GetStatusLogs()
{
	new_status_message_ = false;
	return std::make_shared<std::vector<LogMessage>>(status_logs_);
};