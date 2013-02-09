// Copyright (c) 2012 by Manuel Simoni (msimoni@gmail.com)
// Released under the MIT license.

#include "rtbstorage.h"
#include "rtbbench.h"

/*
  This program benchmarks lookup performance of memtables of various
  sizes containing 8B keys and 8B offsets and prints results
  TSV-formatted to standard output.

  The output begins with a comment line, and then consists of
  tab-separated lines containing two values: the number of entries in
  the memtable, and the lookups per second with that memtable size.
*/

void
rtb_bench_lookup_perf(size_t entries_ct) {
  rtb_memtable *mt = rtb_new_memtable(8, 8, entries_ct);
  size_t i;

  // Populate table
  for (i = 0; i < entries_ct; i++) {
    char key[9];
    sprintf(key, "%08x", i);
    char val[9];
    sprintf(val, "%08x", i);
    rtb_memtable_put(mt, i, key, val);
  }

  // Lookup
  struct timeval start_lookup, stop_lookup;
  gettimeofday(&start_lookup, NULL);

  for (i = 0; i < entries_ct; i++) {
    char key[9];
    sprintf(key, "%08x", i);
    char val[9];
    sprintf(val, "%08x", i);
    char *off = rtb_memtable_get(mt, key);
    if (off == NULL) { rtb_error("Not found."); }
    if (memcmp(val, off, 8) != 0) { rtb_error("Wrong value."); }
  }

  gettimeofday(&stop_lookup, NULL);
  int64_t lookup_time_ms = bench_timeval_diff_ms(&start_lookup, &stop_lookup);

  int64_t entries_per_second = (entries_ct / lookup_time_ms) * 1000;

  printf("%d\t%d\n", entries_ct, entries_per_second);

}

int
main()
{
  printf("# count\tlookups_per_second\n");
  size_t i;
  for(i = 1; i <= 20; i++) {
    rtb_bench_lookup_perf(50000 * i);
  }
}
