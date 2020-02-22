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
