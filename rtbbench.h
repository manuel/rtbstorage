// Copyright (c) 2012 by Manuel Simoni (msimoni@gmail.com)
// Released under the MIT license.

#ifndef RTBBENCH_H
#define RTBBENCH_H

/*
  This header defines utility functions for benchmarking.
*/

#include <stdint.h>
#include <sys/time.h>

/* Returns the difference between the two timevals in milliseconds. */
int64_t
bench_timeval_diff_ms(struct timeval *start, struct timeval *stop);

#endif
