/**
 * @file utils.h
 * @brief Storj utilities.
 *
 * Helper utilities
 */
#ifndef STORJ_UTILS_H
#define STORJ_UTILS_H

#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>
#include <stdbool.h>

#ifdef _WIN32
#include <windows.h>
#include <time.h>
#include <io.h>

ssize_t pread(int fd, void *buf, size_t count, uint64_t offset);
#else
#include <sys/time.h>
#endif

int hex2str(unsigned length, uint8_t *data, char *buffer);

void print_int_array(uint8_t *array, unsigned length);

int str2hex(unsigned length, char *data, uint8_t *buffer);

char *str_concat_many(int count, ...);

void random_buffer(uint8_t *buf, size_t len);

uint64_t shard_size(int hops);

uint64_t get_time_milliseconds();

void memset_zero(void *v, size_t n);

#endif /* STORJ_UTILS_H */
