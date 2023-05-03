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


#include "MessageManager.h"

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
bool MessageManager::is_new_session_;

Message MessageManager::AddMessage(std::stringstream& msg, message_t type, bool popup)
{
	Message tmpMsg;

	tmpMsg.msg = msg.str();
	tmpMsg.timeinfo = TimeHelper::GetTimeinfo();
	tmpMsg.type = type;
	tmpMsg.popup = popup;

	messages.insert(messages.begin(), tmpMsg);

	if (messages.size() > MAX_STORED_MSG)
	messages.pop_back();

	msg.str("");

	return tmpMsg;
}

Message MessageManager::AddMessage(const char *msg, message_t type, bool popup)
{
	std::stringstream tmpMsg;
	tmpMsg << msg;
	return MessageManager::AddMessage(tmpMsg, type, popup);
}

int MessageManager::Log(std::stringstream& msg)
{
	MessageManager::Log(msg.str().c_str());
	return 0;
}

int MessageManager::Log(const char* format, ...)
{
	va_list args;
	va_start(args, format);
	std::stringstream ss;
	ss << format << "\n";
	std::vfprintf(stdout, ss.str().c_str(), args);
	va_end(args);

	std::ofstream outfile("jnrmaker.log", std::ios_base::app);
	if (!outfile.is_open())
	{
		return -1;
	}
	outfile.seekp(0, std::ios::beg);
	outfile << TimeHelper::GetTimeinfo().c_str() << "\t" << ss.str();
	outfile.close();

	return 0;
}