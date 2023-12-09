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

files_list_entry_t *add_file_entry(files_list_t *list, char *file_path) {
    files_list_entry_t *new_entry = malloc(sizeof(files_list_entry_t));
    if (new_entry == NULL) {
        perror("malloc");
        return NULL;
    }

    strncpy(new_entry->path_and_name, file_path, sizeof(new_entry->path_and_name));
    new_entry->prev = NULL;
    new_entry->next = NULL;

    files_list_entry_t *temp = list->head;

    while (temp != NULL) {
        if (strcmp(temp->path_and_name, file_path) == 0) {
            free(new_entry);
            return NULL;
        } else if (strcmp(temp->path_and_name, file_path) > 0) {
            // Insert before current entry
            new_entry->prev = temp->prev;
            new_entry->next = temp;

            if (temp->prev != NULL) {
                temp->prev->next = new_entry;
            } else {
                list->head = new_entry;
            }

            temp->prev = new_entry;
            return new_entry;
        }

        temp = temp->next;
    }

    if (list->head == NULL) {
        list->head = new_entry;
        list->tail = new_entry;
    } else {
        new_entry->prev = list->tail;
        list->tail->next = new_entry;
        list->tail = new_entry;
    }

    return new_entry;
}
