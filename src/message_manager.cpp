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
 * @file message_manager.cpp
 * @brief This file contains the singleton message manager class implementation.
 *
 * @author Markus Wende
 * https://github.com/MarkusWende
 */


#include "../include/message_manager.h"

#ifdef __EMSCRIPTEN__
void msgErr(std::string const& msg)
{
    std::stringstream ss;
    ss << msg;
    MessageManager::AddMessage(ss, message_t::ERROR_T);
}

void msgDebug(std::string const& msg)
{
    std::stringstream ss;
    ss << msg;
    MessageManager::AddMessage(ss, message_t::DEBUG);
}

EMSCRIPTEN_BINDINGS(my_module) {
    emscripten::function("msgErr", &msgErr);
    emscripten::function("msgDebug", &msgDebug);
}
#endif

// Instantiate static messages vector
std::vector<Message> MessageManager::messages;

Message MessageManager::AddMessage(std::stringstream& msg, message_t type)
{
  Message tmpMsg;

  tmpMsg.msg = msg.str();
  tmpMsg.timeinfo = TimeHelper::GetTimeinfo();
  tmpMsg.type = type;

  messages.insert(messages.begin(), tmpMsg);

  if (messages.size() > MAX_STORED_MSG)
    messages.pop_back();

  msg.str("");

  return tmpMsg;
}
