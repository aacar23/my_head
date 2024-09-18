#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "disp_file.h"
#include "error_exit.h"
#include "time_to_str.h"

void disp_file(const char *file_name)
{
    const char *faulty_call = NULL;

    FILE *file_handle = fopen(file_name, "rb");
    if (!file_handle){
        faulty_call = "fopen";
        goto FAIL_1;
    }

    for (int c;(c = fgetc(file_handle)) != EOF;printf("%c", c))
        ;

    fflush(stdout);

    if (ferror(file_handle)){
        faulty_call = "fgetc";
        goto FAIL_2;
    }

    FAIL_2:
    fclose(file_handle);
    FAIL_1:
    if (faulty_call)
        error_exit("%s:%s:%s:%s\n", __func__, faulty_call, strerror(errno), time_to_str());
}
