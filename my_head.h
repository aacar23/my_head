#if !defined MY_HEAD_H
#define MY_HEAD_H

#include <stdio.h>
#include <sys/types.h>

extern int terminating_character;

void my_head_byte_runner(FILE *file_handle, ssize_t count);
void my_head_line_runner(FILE *file_handle, ssize_t count);

void my_head(const char *file_name, ssize_t count, int print_head, void (*runner)(FILE *file_handle, ssize_t count));

#endif
