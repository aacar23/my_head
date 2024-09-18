#if !defined MY_HEAD_H
#define MY_HEAD_H

#include <stdio.h>
#include <sys/types.h>

void my_head_byte_runner(FILE *file_handle, ssize_t count, int terminating_character);
void my_head_line_runner(FILE *file_handle, ssize_t count, int terminating_character);

void my_head(const char *file_name, ssize_t count, int print_head, int is_zero_terminated, void (*runner)(FILE *file_handle, ssize_t count, int terminating_character));

#endif
