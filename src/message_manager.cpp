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

// Instantiate static messages vector
std::vector<Message> MessageManager::messages;

Message MessageManager::AddMessage(std::stringstream& msg, message_t type)
{
  Message tmpMsg;

  tmpMsg.msg = msg.str();
  tmpMsg.timeinfo = time_helper::GetTimeinfo();
  tmpMsg.type = type;

  messages.push_back(tmpMsg);

  if (messages.size() > MAX_STORED_MSG)
    messages.erase(messages.begin());

  msg.str("");

  return tmpMsg;
}
