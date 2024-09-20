#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <getopt.h>
#include <errno.h>

#include "my_head.h"
#include "disp_file.h"
#include "error_exit.h"

#define HELP_OPT (-111)
#define VERSION_OPT (111)

#define UNASSIGNED (-1)

int main(int argc, char **argv)
{

    errno = 0;

    int print_header = UNASSIGNED;
    int lines_or_counts = 'n';

    ssize_t count = 10;

    struct option options[] = {
        {.name = "bytes", .has_arg = required_argument, .flag = &lines_or_counts, .val = 'c'},
        {.name = "lines", .has_arg = required_argument, .flag =&lines_or_counts, .val = 'n'},
        {.name = "verbose", .has_arg = no_argument, .flag = &print_header, .val = 1},
        {.name = "quiet", .has_arg = no_argument, .flag = &print_header, .val = 0},
        {.name = "silent", .has_arg = no_argument, .flag = &print_header, .val = 0},
        {.name = "zero_terminated", .has_arg = no_argument, .flag = &terminating_character, .val = '\0'},
        {.name = "help", .has_arg = no_argument, .flag = NULL, .val = HELP_OPT},
        {.name = "version", .has_arg = no_argument, .flag = NULL, .val = VERSION_OPT},
        {0, 0, 0, 0}
    };

    for (int c;(c = getopt_long(argc, argv, "c:n:zvq", options, NULL)) != -1;){
        if (c == VERSION_OPT){
            printf("1.00v\n");
            return 0;
        }

        if (c == HELP_OPT){
            disp_file("help.txt");
            return 0;
        }

        if (c == 'z'){
            terminating_character = '\0';
            continue;
        }

        char *end = &(char){'\0'};

        if (c == 'c' || c == 'n')
            count = strtol(optarg, &end, 0);

        if (errno == ERANGE)
            count = LONG_MAX;

        if (*end)
            error_exit("Please enter an integer argument for option -%c", c);
    }

    print_header = print_header == UNASSIGNED ? optind != argc - 1 : print_header;

    for (int i = optind;i < argc;++i)
        my_head(*(argv + i), count, print_header, lines_or_counts == 'n' ? &my_head_line_runner : my_head_byte_runner);
}
