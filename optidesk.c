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
#include <sys/stat.h>
#include <getopt.h>
#include <gio/gio.h>
#include <glib.h>

int parse_desktop_file (GKeyFile* key_file, char* source_name);
int load_from_filepath (GKeyFile* key_file, const char* source_file_path);
int load_from_filename (GKeyFile* key_file, const char* source_file_name);

int parse_exec (GKeyFile* key_file);
void set_exec_with_optirun (GKeyFile* key_file, gchar* exec_field);

int write_desktop_file (GKeyFile* key_file, const gchar* destination_name);

struct key_file_conf {
	gchar *file_name; //without any extension or with extension ?
	gchar *file_path;
	int has_optirun;
	int has_shortcuts;
	gchar *shortcuts_list;
};

int parse_desktop_file (GKeyFile* key_file, char* source_name) {
	load_from_filename (key_file, source_name);
	if (key_file ==NULL) {
		printf("Error on loading a key file : %s\n", source_name);
		return(0);
	} else {
		printf("File parsing ...\n");
		if (!parse_exec (key_file)) {
			return(0);
		} else {
			printf("Try to write the file\n");
		}
	}
	return (1);
}

/**
Parse exec and apply the needed modification to the exec field
Set or remove the configuration of exec field according to options specified as argument
*/
int parse_exec (GKeyFile* key_file) {
	gchar* exec_field=NULL;
	gboolean has_optirun=FALSE;
	exec_field = g_key_file_get_string(key_file, "Desktop Entry", "Exec" , NULL);
	if (exec_field == NULL) {
		printf("Error getting the Exec field\n");
		return(0);		
	} else {
		has_optirun= g_str_has_prefix(exec_field,"optirun ");
		if (has_optirun==TRUE) {
			printf("File already configured with optirun: %s\n",exec_field);
			return(0);
		} else {
			printf("File is not configured : %s\n", exec_field);
			set_exec_with_optirun (key_file, exec_field);
		}
	}
	return(1);
}

int has_optirun (GKeyFile* key_file) {

}

/**
Prepend an optirun string to the exec field content
*/

void set_exec_with_optirun (GKeyFile* key_file, gchar* exec_field) {
	gchar* new_exec=NULL;
	new_exec = g_strjoin(" ","optirun", exec_field, NULL);
	g_key_file_set_string(key_file,"Desktop Entry","Exec", new_exec);
	//g_key_file_set_comment(key_file,"Desktop Entry", "Exec","Exec value modified by optidesk", NULL);
	printf("New Exec field value : %s\n", new_exec);
}

/**
Write the configuration into a file specified as an argument
This function overwrite the file if it already exist or create one if it doesn't exist.
The file is filled with the content of the modified source desktop file stored in memory.
*/

int write_desktop_file (GKeyFile* key_file, const gchar* destination_name) {
	gchar* output=NULL;	
	output = g_key_file_to_data(key_file, NULL, NULL);

	FILE* destination_file = NULL;
	destination_file = fopen( destination_name, "w+");

	if (destination_file != NULL) {
		fprintf(destination_file, "%s", output);
		fclose(destination_file);
		//chmod(destination_name,755);
		return(1);
    } else {
       	printf("Error on opening the file to write: %s\n", destination_name);
		return(0);
	}
}

/**
Return a keyfile object using an absolute or a relative path
This function ensure the user to use the wanted source file
*/

int load_from_filepath (GKeyFile* key_file, const char* source_file_path) {
	if (!g_key_file_load_from_file(key_file, source_file_path , G_KEY_FILE_NONE, NULL)) {
		printf("Error on loading a key file object from filepath : %s\n", source_file_path);
		return 1;
	}
	return 0;
}


/**
Return a keyfile object using a filename
The function look for application/filename.desktop file in the user directory and then in the system directory (xdg compliant)	
*/

int load_from_filename (GKeyFile* key_file, const char* source_file_name) {
	gchar* source_file_relative_path=NULL;
	gchar* source_full_path=NULL;
	source_file_relative_path = g_strconcat("applications/", source_file_name,".desktop", NULL);
	if (!g_key_file_load_from_data_dirs(key_file, source_file_relative_path, &source_full_path, G_KEY_FILE_NONE, NULL)) {
		printf("Error on loading a key file object from filename : %s\n", source_file_name);
		return (0);
	} else {
		printf ("Source file is located at : %s\n", source_full_path);
		return (1);
	}
}


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
	gchar* source_name;
	gchar* destination_name;
	source_name=argv[optind];
	destination_name=argv[optind+1];
	printf("Source name : %s\nDestination name : %s\n", source_name, destination_name);
	GKeyFile* key_file;
	key_file = g_key_file_new();
	if (parse_desktop_file (key_file, source_name )) {
		if (!write_desktop_file (key_file, destination_name )) {
			printf("File could not be written : %s\n", destination_name);
		} else {
			printf("File succesfully configured\n");
		}
	} else {
	printf("Unable to parse the file : %s", source_name);
	}
	g_key_file_free(key_file);
	/*
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
	*/
    }

    return exit_flag;
}
