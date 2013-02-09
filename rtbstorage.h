// Copyright (c) 2012 by Manuel Simoni (msimoni@gmail.com)
// Released under the MIT license.

#ifndef RTBSTORAGE_H
#define RTBSTORAGE_H

/*
  This header contains the API for memtables, sorted arrays that map
  keys to file offsets.
*/

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* A memtable is a sorted table in RAM that maps each master key to
   the offset in the bucket file where the master key's chunk is
   stored.  The memtable contains a small header and then the keys and
   offsets stored contiguously. */
typedef struct {
  /* The length of a key in bytes.  Should be a multiple of 2. */
  size_t mt_key_len;
  /* The length of an offset in bytes.  Should be a multiple of 2. */
  size_t mt_off_len;
  /* The number of key-offset pairs in the memtable. */
  size_t mt_entries_ct;
  /* The entries (key and offset pairs). */
  char mt_entries[0];
} rtb_memtable;

/* Allocates a new empty memtable with the given key and offset sizes
   in bytes, and the given number of entries. */
rtb_memtable *
rtb_new_memtable(size_t key_len, size_t off_len, size_t entries_ct);

/* Updates the nth key-offset pair in the memtable.  The caller must
   ensure to enter entries correctly sorted. */
void
rtb_memtable_put(rtb_memtable *mt, size_t n, char *key, char *off);

/* Finds the address of the offset associated with the given key in
   the memtable, or NULL if the key is not in the memtable. */
char *
rtb_memtable_get(rtb_memtable *mt, char *key);

/* Prints an error message to stderr and exits the program. */
void
rtb_error(char *msg);

#endif
