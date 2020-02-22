#ifndef MESSAGE_MANAGER_H
#define MESSAGE_MANAGER_H

#include <map>
#include <vector>
#include <sstream>

#include <SFML/OpenGL.hpp>

#include "time_helper.h"

#define MAX_STORED_MSG 20

enum class message_t {
  ERROR = 0,
  WARNING = 1,
  INFO = 2,
};

struct Message {
	std::string msg;
	std::string timeinfo;
  message_t type;
};

class MessageManager
{
public:
  static std::vector<Message> messages;

  // Add a message
  static Message AddMessage(std::stringstream& msg, message_t type);
  // Get all messages
	static std::vector<Message>* GetMessages() { return &messages; };

private:
  // Private constructor, that is we do not want any actual resource manager objects. Its members and functions should be publicly available (static).
  MessageManager() { }
  ~MessageManager() { }
};

#endif
