/*
 * Copyright (C) 2012 The Bumblebee Project
 * Author: Joaquín Ignacio Aramendía <samsagax@gmail.com>
 *
 * This file is part of Optidesk.
 *
 * Optidesk is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Optidesk is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Optidesk. If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <gio/gio.h>

/**
 * Print version string
 */
static void print_version(void) {
    printf("%s version %s\n", "optidesk", "0.1");
}

/**
 * Formatted help string
 */
static void print_opt_line(char* opt, char* description) {
    printf(" %-25s%s\n", opt, description);
}

/**
 * Print a little note on usage
 */
static void print_help(int exit_val) {
    print_version();
    printf("Usage: %s [options] <orig> <dest>\n", "optidesk");
    print_opt_line("-h / --help","Print this help message");
    print_opt_line("-V / --version","Print version and exit");
    exit(exit_val);
}


int main(int argc, char* argv[]) {

    int exit_flag = EXIT_SUCCESS;

    // Parse the options
    char* short_opts= "Vh";
    struct option long_opts[] = {
        {"version"  , no_argument, 0, 'V'},
        {"help"     , no_argument, 0, 'h'},
        {0          , 0, 0, 0}
    };
    char c;
    while(1) {
        c = getopt_long(argc, argv, short_opts, long_opts, 0);
        if (c == -1) {
            break;
        }
        switch (c) {
            case 'V':
                print_version();
                break;
            case 'h':
                print_help(EXIT_SUCCESS);
                break;
            case '?':
                print_help(EXIT_FAILURE);
                break;
            default:
                printf("?? getopt returned %o ??\n", c);
        }
    }
    if ((argc - optind) < 2) {
        printf("Not enough arguments\n");
        printf("expected: 2\ngot: %d\n", argc - optind);
        exit(EXIT_FAILURE);
    } else if ((argc - optind) > 2) {
        printf("Too many arguments\n");
        printf("expected: 2\ngot: %d\n", argc - optind);
        exit(EXIT_FAILURE);
    } else {
        g_type_init();
        GFile *source;
        GFile* destination;
        source = g_file_new_for_commandline_arg(argv[optind]);
        destination = g_file_new_for_commandline_arg(argv[optind+1]);
        GError* file_err = NULL;
        GFileCopyFlags copy_flags = G_FILE_COPY_NONE;
        if (g_file_copy(
                source,
                destination,
                copy_flags,
                NULL,
                NULL,
                NULL,
                &file_err)) {
            printf("File copy success\n");
        } else {
            printf("(Implement) An error occurred.");
            exit_flag = EXIT_FAILURE;
        }
        // Free allocated memory
        g_object_unref(source);
        g_object_unref(destination);
    }
    return exit_flag;
}
