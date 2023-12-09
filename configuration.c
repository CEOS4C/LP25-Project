#include <stddef.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdio.h>
#include <string.h>

#include "configuration.h"
#include "defines.h"

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

void init_configuration(configuration_t *the_config) {
    if (the_config == NULL) {
        fprintf(stderr, "Error: Invalid configuration pointer.\n");
        return;
    } else {
        // Initialize the configuration structure with default values
        strcpy(the_config->source, "");
        strcpy(the_config->destination, "");
        the_config->processes_count = 1;
        the_config->is_parallel = true;
        the_config->uses_md5 = true;
        the_config->is_verbose = false;
        the_config->is_dry_run = false;
    }
}

int set_configuration(configuration_t *the_config, int argc, char *argv[]) {
    if (the_config == NULL) {
        fprintf(stderr, "Erreur : Pointeur vers la configuration invalide.\n");
        return -1;
    }

    // Initialiser la structure de configuration avec des valeurs par défaut
    init_configuration(the_config);

    int opt;
    while ((opt = getopt(argc, argv, "n:v")) != -1) {
        switch (opt) {
            case 'n':
                the_config->processes_count = atoi(optarg);
                break;
            case 'v':
                the_config->is_verbose = true;
                break;
            case '-':
                // Traitement des options longues
                if (strcmp(optarg, "date-size-only") == 0) {
                    the_config->uses_md5 = false;
                } else if (strcmp(optarg, "no-parallel") == 0) {
                    the_config->is_parallel = false;
                } else if (strcmp(optarg, "dry-run") == 0) {
                    the_config->is_dry_run = true;
                } else {
                    fprintf(stderr, "Erreur : Option inconnue : %s\n", optarg);
                    return -1;
                }
                break;
            default:
                fprintf(stderr, "Usage : %s [-n processes_count] [-v] [--date-size-only] [--no-parallel] [--dry-run] source destination\n", argv[0]);
                return -1;
        }
    }

    // Vérifier que les chemins source et destination ont été fournis
    if (optind + 1 < argc) {
        strncpy(the_config->source, argv[optind], PATH_SIZE - 1);
        strncpy(the_config->destination, argv[optind + 1], PATH_SIZE - 1);
    } else {
        fprintf(stderr, "Erreur : Les chemins source et destination doivent être spécifiés.\n");
        return -1;
    }

    return 0;
}