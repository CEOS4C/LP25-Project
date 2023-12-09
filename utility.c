#include <string.h>
#include "defines.h"

char *concat_path(char *result, char *prefix, char *suffix) {
    if (result == NULL || prefix == NULL || suffix == NULL || *prefix == '\0' || *suffix == '\0') {
        return NULL;
    }

    size_t prefix_len = strlen(prefix);
    if (prefix_len >= PATH_SIZE) {
        return NULL;
    }

    strcpy(result, prefix);

    if (result[prefix_len - 1] != '/') {
        if (prefix_len + 1 >= PATH_SIZE) {
            return NULL;
        }
        result[prefix_len] = '/';
        result[prefix_len + 1] = '\0';
    }

    size_t result_len = strlen(result) + strlen(suffix);
    if (result_len >= PATH_SIZE) {
        return NULL;
    }

    strcat(result, suffix);

    return result;
}
