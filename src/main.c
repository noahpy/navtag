
#include "utils.h"
#include <getopt.h>
#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

char* HELP_MSG =
    "USAGE: navtag mark_file_path [OPTIONS] [OPTARGS]\n"
    "mark_file_path (required): absolute path to the mark file\n"
    "OPTIONS change the way the optional arguments [OPTARGS]\n"
    "get intepreted. No options is equivalent to one -t\n"
    "option. It is not possible to specify more than one option.\n\n"
    "OPTIONS:\n"
    "  -t: Translate swift path to valid path. All positional\n"
    "      arguments are interpreted as swift paths. For every\n"
    "      argument, all valid mark label will be replaced with \n"
    "      its path. Each result is printed in given order,\n"
    "      seperated by spaces.\n"
    "  -a: Add new marks. The first two positional arguments\n"
    "      are intepreted as a mark label followed by a path.\n"
    "  -d: Delete marks. The first positional argument is the \n"
    "      label of the mark that needs to be removed.\n"
    "  -l: List and print all marks.\n"
    "  -L: List and print all labels.\n"
    "  -h: Print this message.";

char* optstring = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ-";

int main(int argc, char** argv)
{
    char optchar;
    char modi = 0;
    int past_optind = 0;
    while ((optchar = getopt(argc, argv, optstring)) != -1) {
        switch (optchar) {
        case 't':
        case 'a':
        case 'd':
        case 'l':
        case 'L':
            if (optind == past_optind) {
                break;
            }
            if (modi == 't') {
                fprintf(stdout, "-%c ", optchar);
                break;
            } else if (modi) {
                fprintf(stderr, "Found more than one valid option.\n");
                return EXIT_FAILURE;
            }
            modi = optchar;
            past_optind = optind;
            break;
        case 'h':
            if (optind == past_optind) {
                break;
            }
            if (modi == 't') {
                fprintf(stdout, "-h ");
                past_optind = optind;
                break;
            }
            printf("%s\n", HELP_MSG);
            return EXIT_SUCCESS;
        default:
            if (modi == 't') {
                if (optind == past_optind) {
                    break;
                }
                fprintf(stdout, "%s ", argv[optind - 1]);
                past_optind = optind;
                break;
            }
            fprintf(stderr, "Unknown option\n");
            return EXIT_FAILURE;
        }
    }
    if (argc - optind < 1) {
        fprintf(stderr, "Missing required positional argument: mark_file_path\n");
        return 1;
    }
    char* mark_file_path = argv[optind++];
    switch (modi) {
    case 't':
    case 0:
        translate_paths(argc - optind, argv + optind, mark_file_path);
        break;
    case 'a':
        if (argc - optind < 2) {
            fprintf(stderr, "Not enough optional arguments found: need 2.\n");
            return EXIT_FAILURE;
        }

        char absolute_path[PATH_MAX];
        if (!realpath(argv[optind + 1], absolute_path)) {
            perror("Error resolving absolute path");
            return EXIT_FAILURE;
        }

        if (add_mark(argv[optind], absolute_path, mark_file_path)) {
            fprintf(stderr, "Could not add mark.\n");
            return EXIT_FAILURE;
        }
        printf("Saved label: '%s'\n", argv[optind]);
        break;
    case 'd':
        if (argc - optind < 1) {
            fprintf(stderr, "Not enough optional arguments found: need 1.\n");
            return EXIT_FAILURE;
        }
        if (delete_mark(argv[optind], mark_file_path, ".tmp_mark.txt")) {
            fprintf(stderr, "Could not delete mark.\n");
            return EXIT_FAILURE;
        }
        printf("Removed label: '%s'\n", argv[optind]);
        break;
    case 'l':
        printf("Mark file: %s\n", mark_file_path);
        print_marks(mark_file_path);
        break;
    case 'L':
        print_labels(mark_file_path);
        break;
    }
}
