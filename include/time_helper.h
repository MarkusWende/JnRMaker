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
 * @file time_helper.h
 * @brief This file contains time formating helper functions.
 *
 * @author Markus Wende
 * https://github.com/MarkusWende
 */


#ifndef TIME_HELPER_H
#define TIME_HELPER_H

#include <ctime>       /* time_t, struct tm, time, localtime */
#include <string>
#include <sstream>
#include <iomanip>

namespace time_helper
{
  /**
	 * @brief Get the local time as a string.
	 * @return Return the formnated local time as a string.
	 */
  inline std::string GetTimeinfo()
  {
    std::time_t t = std::time(nullptr);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&t), "%c %Z");
  	return ss.str();
  }
}

#endif	// TIME_HELPER_H
