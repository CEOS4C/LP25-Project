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
