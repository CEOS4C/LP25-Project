#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "defines.h"

typedef struct {
    char source[PATH_SIZE];
    char destination[PATH_SIZE];
    bool is_parallel;
    bool uses_md5;
    bool is_verbose;
    bool is_dry_run;
    uint8_t processes_count;
} configuration_t;

void init_configuration(configuration_t *the_config);

int set_configuration(configuration_t *the_config, int argc, char *argv[]);
