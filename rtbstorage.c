// Copyright (c) 2012 by Manuel Simoni (msimoni@gmail.com)
// Released under the MIT license.

/*
  This file contains the implementation of memtables, sorted arrays
  that map keys to file offsets.

  See the header file "rtbstorage.h" for API documentation.
*/

#include "rtbstorage.h"

rtb_memtable *
rtb_new_memtable(size_t key_len, size_t off_len, size_t entries_ct)
{
  size_t entry_len = key_len + off_len;
  size_t total_len = sizeof(rtb_memtable) + (entries_ct * entry_len);
  
  rtb_memtable *mt = (rtb_memtable *) calloc(1, total_len);
  if (mt == NULL) rtb_error("Cannot allocate memtable.");
  
  mt->mt_key_len = key_len;
  mt->mt_off_len = off_len;
  mt->mt_entries_ct = entries_ct;

  return mt;
}

/* Returns the length of a key-offset pair in the memtable in bytes. */
static inline size_t
rtb_memtable_entry_len(rtb_memtable *mt)
{
  return mt->mt_key_len + mt->mt_off_len;
}

/* Returns the address of the nth key of the memtable. */
static inline char *
rtb_memtable_key_at(rtb_memtable *mt, size_t n)
{
  if (n > mt->mt_entries_ct) rtb_error("Key index out of range.");
  size_t entry_len = rtb_memtable_entry_len(mt);
  return mt->mt_entries + (n * entry_len);
}

/* Returns the address of the nth offset of the memtable. */
static inline char *
rtb_memtable_off_at(rtb_memtable *mt, size_t n)
{
  return rtb_memtable_key_at(mt, n) + mt->mt_key_len;
}

void
rtb_memtable_put(rtb_memtable *mt, size_t n, char *key, char *off)
{
  memcpy(rtb_memtable_key_at(mt, n), key, mt->mt_key_len);
  memcpy(rtb_memtable_off_at(mt, n), off, mt->mt_off_len);
}

/* Adapted from http://en.wikipedia.org/wiki/Binary_search_algorithm */
static char *
rtb_binary_search(rtb_memtable *mt, char *key, size_t imin, size_t imax)
{
  // test if array is empty
  if (imax < imin) {
    return NULL;
  } else {
    // calculate midpoint to cut array in half
    size_t imid = imin + ((imax - imin) / 2);
    // three-way comparison
    int cmp = memcmp(key, rtb_memtable_key_at(mt, imid), mt->mt_key_len);
    if (cmp < 0) {
      // key is in lower subset
      return rtb_binary_search(mt, key, imin, imid - 1);
    } else if (cmp > 0) {
      // key is in upper subset
      return rtb_binary_search(mt, key, imid + 1, imax);
    } else {
      // key has been found, return offset associated with key
      return rtb_memtable_off_at(mt, imid);
    }
  }
}

char *
rtb_memtable_get(rtb_memtable *mt, char *key)
{
  return rtb_binary_search(mt, key, 0, mt->mt_entries_ct);
}

void
rtb_error(char *msg)
{
  fprintf(stderr, msg);
  fprintf(stderr, "\n");
  fflush(stderr);
  exit(EXIT_FAILURE);
}
