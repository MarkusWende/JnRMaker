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