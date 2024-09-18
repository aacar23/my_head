#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <getopt.h>
#include <sys/types.h>
#include <errno.h>

#include "my_head.h"
#include "disp_file.h"
#include "error_exit.h"
#include "time_to_str.h"

static ssize_t counts[2] = {0, 10};
static char *names[] = {"", "bytes", "", "lines", "quiet", "verbose", "zero-terminated"};

#define BYTE_COUNT_PTR (counts)
#define LINE_COUNT_PTR (counts + 1)

#define BYTE_COUNT (*BYTE_COUNT_PTR)
#define LINE_COUNT (*LINE_COUNT_PTR)

#define GET_OPTION_STR(option_names, c) (*((option_names) + ((c) - 93) / 5))

#define VERSION_OPT_VAL (1)
#define HELP_OPT_VAL (2)

#define UNASSIGNED (-1)

static void get_chars_and_display(size_t char_count)
{
    int c;
    for (;char_count && (c = getchar()) != EOF;--char_count)
        putchar(c);

    if (c == EOF)
        error_exit("%s:%s:%s.%s", __func__, "getchar", strerror(errno), time_to_str());
}

static void get_lines_and_display(size_t line_count)
{
    int c;
    for (;line_count && (c = getchar()) != EOF;line_count -= putchar(c) == '\n')
        ;

    if (c == EOF)
        error_exit("%s:%s:%s:%s", __func__, "getchar", strerror(errno), time_to_str());
}

static void get_and_display(size_t count, void (*getter)(size_t count), int print_head)
{
    if (print_head)
        printf("==> standard input <==\n");

    (*getter)(count);
}

static char **find_str_in_str_array(const char * const *strs, size_t length, ...)
{
    va_list args;
    va_start(args, length);

    int found = 0;
    size_t i = 0lu;

    for (const char *key;(key = va_arg(args, const char *));){
        for (i = 0lu;i < length;++i)
            if ((found = !strcmp(*(strs + i), key)))
                goto OUT;
    }
    OUT:

    va_end(args);

    return found ? (char **)strs + i : NULL;
}


int main(int argc, char **argv)
{
    if (find_str_in_str_array((const char * const *)argv + 1, argc - 1, "--help", NULL)){
        disp_file("help.txt");
        return 0;
    }

    if (find_str_in_str_array((const char * const *)argv + 1, argc - 1, "--version", NULL)){
        printf("1.00v\n");
        return 0;
    }

    opterr = 0;

    int print_head = UNASSIGNED;
    int is_zero_terminated = 0;

    void (*runner)(FILE *file_handle, ssize_t count, int terminating_character) = &my_head_line_runner;

    struct option options[] = {
        {.name ="bytes", .has_arg = required_argument, .flag = NULL, .val = 'c'},
        {.name = "lines", .has_arg = required_argument, .flag = NULL, .val = 'n'},
        {.name = "quiet", .has_arg = no_argument, .flag = NULL, .val = 'q'},
        {.name = "verbose", .has_arg = no_argument, .flag = NULL, .val = 'v'},
        {.name = "zero-terminated", .has_arg = no_argument, .flag = NULL, .val = 'z'},
        {.name = "help", .has_arg = no_argument, .flag = NULL, .val = HELP_OPT_VAL},
        {.name = "version", .has_arg = no_argument, .flag = NULL, .val = VERSION_OPT_VAL},
        {0, 0, 0, 0}
    };

    optopt = 0;

    for (int c;(c = getopt_long(argc, argv, "c:n:vqz", options, NULL)) != -1;optopt = 0){
        if (c == '?'){
            if (!optopt)
                error_exit("Invalid long option.\n");
            if (!strchr("c:n:vqz", optopt))
                error_exit("Invalid option : %c\n", optopt);
            error_exit("Please enter an argument for --%s / %c\n", GET_OPTION_STR(names, optopt), optopt);
        }

        char *end = "\0";

        if (c == 'n'){
            runner = &my_head_line_runner;
            LINE_COUNT = strtol(optarg, &end, 0);
            BYTE_COUNT = 0;
        }

        else if (c == 'c'){
            runner = &my_head_byte_runner;
            BYTE_COUNT = strtol(optarg, &end, 0);
            LINE_COUNT = 0;
        }

        if (*end)
            error_exit("Please enter an integer for count value for option -%c\n", c);

        is_zero_terminated = c == 'z';
        print_head = c == 'q' ? 0 : c == 'v' ? 1 : print_head;
    }



    if (optind == argc){
        get_and_display(LINE_COUNT ? LINE_COUNT : BYTE_COUNT, LINE_COUNT ? &get_lines_and_display : &get_chars_and_display, print_head);

        return 0;
    }

    print_head = print_head == UNASSIGNED ? optind < argc : print_head;

    for (int i = optind;i < argc;){
        if (!strcmp(*(argv + i), "-")){
            get_and_display(LINE_COUNT ? LINE_COUNT : BYTE_COUNT, LINE_COUNT ? &get_lines_and_display : &get_chars_and_display, print_head);
            goto END;
        }

        my_head(*(argv + i), BYTE_COUNT ? BYTE_COUNT : LINE_COUNT, print_head, is_zero_terminated, runner);

        END:
        if (argc != ++i && print_head)
            printf("\n");
    }

    return 0;
}
