/****************************************************************************
 * Copyright (C) 2019 Kirsch-Audio GmbH <software@kirsch-audio.com>
 * All rights reserved.
 *
 * This file is part of ANCMonitor
 *
 *	SoundIMP is a software for creating sound immission predictions.
 *
 *	Unauthorized copying of this file, via any medium is strictly
 *	prohibited
 *
 *	Proprietary and confidential.
 ****************************************************************************/

/**
 * @file time_helper.hpp
 * @author Kirsch Audio GmbH
 * @brief Time helper namespace.
 */


#pragma once

#include <string>
#include <ctime>
#include <iostream>
#include <iomanip>
#include <chrono>
#include <sstream>

#ifdef WIN32
#define localtime_r(_Time, _Tm) localtime_s(_Tm, _Time)
#endif

typedef std::chrono::high_resolution_clock Clock;
typedef std::chrono::milliseconds milliseconds;
typedef std::chrono::microseconds microseconds;
typedef std::chrono::seconds seconds;
typedef std::chrono::nanoseconds nanoseconds;

static Clock::time_point t0 = Clock::now();

namespace TimeHelper
{
  /**
   * @brief Get the local time as a string.
   * @return Return the formnated local time as a string.
   */
  inline std::string GetTimeinfo()
  {
    tm localTime;
    std::chrono::system_clock::time_point t = std::chrono::system_clock::now();
    time_t now = std::chrono::system_clock::to_time_t(t);
    localtime_r(&now, &localTime);

    const std::chrono::duration<double> tse = t.time_since_epoch();
    std::chrono::seconds::rep milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(tse).count() % 1000;

    std::stringstream ss;

    ss << (1900 + localTime.tm_year) << '-'
      << std::setfill('0') << std::setw(2) << (localTime.tm_mon + 1) << '-'
      << std::setfill('0') << std::setw(2) << localTime.tm_mday << ' '
      << std::setfill('0') << std::setw(2) << localTime.tm_hour << ':'
      << std::setfill('0') << std::setw(2) << localTime.tm_min << ':'
      << std::setfill('0') << std::setw(2) << localTime.tm_sec << '.'
      << std::setfill('0') << std::setw(3) << milliseconds;

    return ss.str();
  }

  inline long long tic()
  {
    t0 = Clock::now();
    nanoseconds ns = std::chrono::duration_cast<nanoseconds>(t0.time_since_epoch());
    return ns.count();
    //
  }

  inline int toc(int format = 0)
  {
    int delta = 0;
    Clock::time_point t1 = Clock::now();
    if (format == 0)
    {
      microseconds us = std::chrono::duration_cast<microseconds>(t1 - t0);
      std::stringstream msg;
      msg << "\tTime elapsed " << us.count() << " us\n";
      //std::cout << msg.str().c_str() << "\n";
      delta = (int)us.count();
    }
    else if (format == 1)
    {
      milliseconds ms = std::chrono::duration_cast<milliseconds>(t1 - t0);
      std::stringstream msg;
      msg << "\tTime elapsed " << ms.count() << " ms\n";
      //std::cout << msg.str().c_str() << "\n";
      delta = (int)ms.count();
    }
    else if (format == 2)
    {
      seconds s = std::chrono::duration_cast<seconds>(t1 - t0);
      std::stringstream msg;
      msg << "\tTime elapsed " << s.count() << " s\n";
      //std::cout << msg.str().c_str() << "\n";
      delta = (int)s.count();
    }
    else if (format == 3)
    {
      nanoseconds ns = std::chrono::duration_cast<nanoseconds>(t1 - t0);
      std::stringstream msg;
      msg << "\tTime elapsed " << ns.count() << " ns\n";
      //std::cout << msg.str().c_str() << "\n";
      delta = (int)ns.count();
    }

    return delta;
  }
}