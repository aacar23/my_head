#include <string.h>
#include <errno.h>

#include "my_head.h"
#include "error_exit.h"
#include "time_to_str.h"


int terminating_character = '\n';

static void print_head_template(const char *file_name){
    printf("==> %s <==\n", file_name);
}

static long get_end_of_file(FILE *file_handle)
{
    long cur_point = ftell(file_handle);

    if (fseek(file_handle, 0, SEEK_END))
        error_exit("%s:%s:%s:%s", __func__, "fseek", strerror(errno), time_to_str());

    long end_point = ftell(file_handle);

    if (fseek(file_handle, cur_point, SEEK_SET))
        error_exit("%s:%s:%s:%s", __func__, "fseek", strerror(errno), time_to_str());

    return end_point;
}

void my_head_byte_runner(FILE *file_handle, ssize_t count)
{
    for (int c;((c = fgetc(file_handle)) != EOF) && count > 0;--count)
        printf("%c", c);
}


void my_head_line_runner(FILE *file_handle, ssize_t count)
{
    for (int c;((c = fgetc(file_handle)) != EOF) && count > 0;count -= c == terminating_character)
        printf("%c", c);

    if (ferror(file_handle))
        error_exit("%s:%s:%s:%s", __func__, "fgetc", strerror(errno), time_to_str());
}

void my_head(const char *file_name, ssize_t count, int print_head, void (*runner)(FILE *file_handle, ssize_t count))
{
    const char *faulty_call = NULL;

    FILE *file_handle = fopen(file_name, "rb");
    if (!file_handle){
        faulty_call = "fopen";
        goto FAIL_1;
    }

    if (print_head)
        print_head_template(file_name);

    if (count >= 0){
        (*runner)(file_handle, count);
        return;
    }

    int end_of_file = get_end_of_file(file_handle);

    int is_count_applicable = -count <= end_of_file;

    if (is_count_applicable && fseek(file_handle, count, SEEK_END)){
        faulty_call = "fseek";
        goto FAIL_2;
    }

    long end_point = is_count_applicable ? ftell(file_handle) : 0;

    if (fseek(file_handle, 0, SEEK_SET)){
        faulty_call = "fseek";
        goto FAIL_2;
    }

    (*runner)(file_handle, end_point - 1);

    FAIL_2:
    fclose(file_handle);
    FAIL_1:
    if (faulty_call){
        if (errno == ENOENT){
            printf("my_head: cannot open \'%s\' for reading: %s\n", file_name, strerror(errno));
            return;
        }
        error_exit("%s:%s:%s:%s", __func__, faulty_call, strerror(errno), time_to_str());
    }
}
