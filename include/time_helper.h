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
    //time_t rawtime;
    //struct tm * timeinfo;

    //time (&rawtime);
    //timeinfo = localtime(&rawtime);

  	//char* cArray = asctime(timeinfo);
    //cArray[sizeof(cArray) - 1] = '\0';

  	return ss.str();
  }
}

#endif	// DRAW_H
