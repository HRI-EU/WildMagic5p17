// Geometric Tools, LLC
// Copyright (c) 1998-2014
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt
// http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
//
// File Version: 5.0.0 (2010/01/01)

#include "Wm5CorePCH.h"
#include "Wm5Time.h"

namespace Wm5
{

#ifdef __APPLE__
#include <sys/time.h>
static timeval gsInitial;
static bool gsInitializedTime = false;
#else
#include <sys/timeb.h>
static long gsInitialSec = 0;
static long gsInitialUSec = 0;
static bool gsInitializedTime = false;

// This method has been added to eliminate warnings coming from the deprecated ftime function.
static void ftime_replacement(struct timeb* tp)
{
  struct timespec ts;

  // Use CLOCK_REALTIME to get the current time
  if (clock_gettime(CLOCK_REALTIME, &ts) == -1)
  {
    perror("clock_gettime");
    return;
  }

  // Convert timespec to timeb structure
  tp->time = ts.tv_sec;
  tp->millitm = ts.tv_nsec / 1000000; // convert nanoseconds to milliseconds
  tp->timezone = 0; // time zone information is not provided by clock_gettime
  tp->dstflag = 0; // daylight saving time flag is not provided by clock_gettime
}

#endif

//----------------------------------------------------------------------------
int64_t GetTimeInMicroseconds()
{
#ifdef __APPLE__
  if (!gsInitializedTime)
  {
    gsInitializedTime = true;
    gettimeofday(&gsInitial, 0);
  }

  struct timeval currentTime;
  gettimeofday(&currentTime, 0);

  struct timeval deltaTime;
  timersub(&currentTime, &gsInitial, &deltaTime);

  return 1000000*deltaTime.tv_sec + deltaTime.tv_usec;
#else
  struct timeb currentTime;

  if (!gsInitializedTime)
  {
    gsInitializedTime = true;
    ftime_replacement(&currentTime);
    gsInitialSec = (long)currentTime.time;
    gsInitialUSec = 1000*currentTime.millitm;
  }

  ftime_replacement(&currentTime);
  long currentSec = (long)currentTime.time;
  long currentUSec = 1000*currentTime.millitm;
  long deltaSec = currentSec - gsInitialSec;
  long deltaUSec = currentUSec - gsInitialUSec;
  if (deltaUSec < 0)
  {
    deltaUSec += 1000000;
    --deltaSec;
  }

  return 1000000*deltaSec + deltaUSec;
#endif
}
//----------------------------------------------------------------------------
double GetTimeInSeconds()
{
  int64_t microseconds = GetTimeInMicroseconds();
  return 1e-06*microseconds;
}
//----------------------------------------------------------------------------

}
