// time.h
#ifndef TIME_HELPER_H
#define TIME_HELPER_H

#include <ctime>       /* time_t, struct tm, time, localtime */
#include <string>
#include <sstream>
#include <iomanip>

namespace time_helper
{
  inline std::string GetTimeinfo()
  {
    std::time_t t = std::time(nullptr);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&t), "%c %Z");
  	return ss.str();
  }
}

#endif	// DRAW_H
