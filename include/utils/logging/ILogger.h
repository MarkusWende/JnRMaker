/**
 * JnRMaker - A tile editor and jump and run game maker
 * See COPYRIGHT file at the top of the source tree.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the JnRMaker License Statement and
 * the GNU General Public License along with this program.
 *
 */

/**
 * @file Logger.h
 * @brief Interface for logging
 *
 * @author Markus Wende
 * https://github.com/MarkusWende
 */

#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <memory>

/**
 * @brief Enumeration for log types.
 */
enum class log_t
{
    ERROR_T = 0,                          /**< Error message. */
    WARNING = 1,                          /**< Warning message. */
    INFO = 2,                             /**< Info message. */
    DEBUG = 3,                            /**< Debug message. */
    DEBUG_WS = 4,                         /**< Websocket debug message. */
};

/**
 * @brief Struct for representing a log message.
 */
struct LogMessage
{
    std::string msg;                    /**< The actual message. */
    std::string timeinfo;               /**< Timestamp. */
    log_t type;                         /**< Message type. */
    bool popup;                         /**< Show a popup window */
};

class ILogger
{
public:
    virtual ~ILogger() {}
    virtual LogMessage Log(log_t type, const char* format, ...) = 0;
    virtual LogMessage Log(const char* format, ...) = 0;
    virtual std::shared_ptr<std::vector<LogMessage>> GetLogs() = 0;
};