#include <string.h>
#include "defines.h"


/*!
 * @brief concat_path concatenates suffix to prefix into result
 * It checks if prefix ends by / and adds this token if necessary
 * It also checks that result will fit into PATH_SIZE length
 * @param result the result of the concatenation
 * @param prefix the first part of the resulting path
 * @param suffix the second part of the resulting path
 * @return a pointer to the resulting path, NULL when concatenation failed
 */
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
