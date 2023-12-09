#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "files-list.h"

void clear_files_list(files_list_t *list) {
    while (list->head) {
        files_list_entry_t *tmp = list->head;
        list->head = tmp->next;
        free(tmp);
    }
}
