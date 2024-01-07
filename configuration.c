#include <stddef.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdio.h>
#include <string.h>

#include "configuration.h"
#include "defines.h"


/*!
 * @brief function display_help displays a brief manual for the program usage
 * @param my_name is the name of the binary file
 * This function is provided with its code, you don't have to implement nor modify it.
 */
void display_help(char *my_name) {
    printf("%s [options] source_dir destination_dir\n", my_name);
    printf("Options:\n");
    printf("  -n <processes count>\tNumber of processes for file calculations\n");
    printf("  -h\t\t\tDisplay help (this text)\n");
    printf("  --date_size_only\tDisable MD5 calculation for files\n");
    printf("  --no-parallel\t\tDisable parallel computing (cancels values of option -n)\n");
    printf("  --dry-run\t\tList the changes that would need to be synchronized but doesn't perform them\n");
    printf("  -v\t\t\tEnable verbose mode\n");
}


/*!
 * @brief init_configuration initializes the configuration with default values
 * @param the_config is a pointer to the configuration to be initialized
 */
void init_configuration(configuration_t *the_config) {
    if (the_config == NULL) {
        fprintf(stderr, "Error: Invalid configuration pointer.\n");
        return;
    } else {
        strcpy(the_config->source, "");
        strcpy(the_config->destination, "");
        the_config->processes_count = 1;
        the_config->is_parallel = true;
        the_config->uses_md5 = true;
        the_config->is_verbose = false;
        the_config->is_dry_run = false;
    }
}


/*!
 * @brief set_configuration updates a configuration based on options and parameters passed to the program CLI
 * @param the_config is a pointer to the configuration to update
 * @param argc is the number of arguments to be processed
 * @param argv is an array of strings with the program parameters
 * @return -1 if configuration cannot succeed, 0 when ok
 */
int set_configuration(configuration_t *the_config, int argc, char *argv[]) {
    if (the_config == NULL) {
        fprintf(stderr, "Error : Pointer to invalid configuration.\n");
        return -1;
    }

    init_configuration(the_config);
    struct option long_options[] = {
            {"date-size-only", no_argument,       0, 'd'},
            {"no-parallel",    no_argument,       0, 'p'},
            {"dry-run",        no_argument,       0, 'r'},
            {"verbose",        no_argument,       0, 'v'},
            {0, 0, 0, 0}
    };
    int opt;
    while ((opt = getopt_long(argc, argv, "n:vh", long_options, NULL)) != -1) {
        switch (opt) {
            case 'n':
                the_config->processes_count = atoi(optarg);
                break;
            case 'v':
                the_config->is_verbose = true;
                break;
            case 'd':
                the_config->uses_md5 = false;
                break;
            case 'p':
                the_config->is_parallel = false;
                break;
            case 'r':
                the_config->is_dry_run = true;
                break;
            case 'h':
                display_help(argv[0]);
                return 0;
            default:
                fprintf(stderr,
                        "Usage: %s [-n processes_count] [-v] [--date-size-only] [--no-parallel] [--dry-run] source destination\n",
                        argv[0]);
                return -1;
        }
    }

    if (optind + 1 < argc) {
        strncpy(the_config->source, argv[optind], PATH_SIZE - 1);
        strncpy(the_config->destination, argv[optind + 1], PATH_SIZE - 1);
    } else {
        fprintf(stderr, "Error: Source and destination paths must be specified.\n");
        return -1;
    }

    return 0;
}
