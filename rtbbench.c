// Copyright (c) 2012 by Manuel Simoni (msimoni@gmail.com)
// Released under the MIT license.

/*
  This file contains utility functions for benchmarking.

  See the header file "rtbbench.h" for API documentation.
*/

#include "rtbbench.h"

int64_t
bench_timeval_diff_ms(struct timeval *start, struct timeval *stop)
{
  int64_t t1, t2;
  t1 = start->tv_sec * 1000000 + start->tv_usec;
  t2 = stop->tv_sec * 1000000 + stop->tv_usec;
  return (t2 - t1) / 1000;
}
