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
 * @file message_manager.h
 * @brief This file contains the singleton message manager class.
 *
 * @author Markus Wende
 * https://github.com/MarkusWende
 */


#pragma once

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/fetch.h>
#include <emscripten/websocket.h>
#include <emscripten/bind.h>
#include <emscripten/emscripten.h>
#include <emscripten/val.h>
#endif

#include <map>
#include <vector>
#include <sstream>

#include "imgui.h"

#include "time_helper.h"

#define MAX_STORED_MSG 200               /**< Defines the maximal number of messages to be stored. */

/**
 * @brief Enumeration for message types.
 */
enum class message_t {
  ERROR_T = 0,                          /**< Error message. */
  WARNING = 1,                          /**< Warning message. */
  INFO = 2,                             /**< Info message. */
  DEBUG = 3,                            /**< Debug message. */
  DEBUG_WS = 4,                         /**< Websocket debug message. */
};

/**
 * @brief Message struct for representing a message.
 */
struct Message {
	std::string msg;                     /**< The actual message. */
	std::string timeinfo;                /**< Timestamp. */
  message_t type;                      /**< Message type. */
  bool popup;                           /**< Show a popup window */
};

/**
 * @brief The sindgleton message manager class manages messages accross the application.
 */
class MessageManager
{
public:
  static std::vector<Message> messages;               /**< Contains all messages, that are shared at the moment. */

  /**
	 * @brief Add a message.
   * @param msg Message string.
   * @param type Type of the message {ERROR, WARNING, INFO}.
	 * @return Return the message struct.
	 */
  static Message AddMessage(std::stringstream& msg, message_t type, bool popup = 0);

  /**
	 * @brief Get all messages in the message vector.
	 * @return Return a pointer to the messages vector.
	 */
	static std::vector<Message>* GetMessages() { return &messages; };

private:
  // Private constructor, that is we do not want any actual resource manager objects. Its members and functions should be publicly available (static).
  MessageManager() { };                             //!< constructor
  ~MessageManager() { };                            //!< destructor
};