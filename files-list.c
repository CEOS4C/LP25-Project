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


int add_entry_to_tail(files_list_t *list, files_list_entry_t *entry) {
    if (list == NULL || entry == NULL) {
        return -1;
    }

    if (list->tail == NULL) {
        list->head = entry;
        list->tail = entry;
        entry->prev = NULL;
        entry->next = NULL;
    } else {
        entry->prev = list->tail;
        entry->next = NULL;
        list->tail->next = entry;
        list->tail = entry;
    }

    return 0;
}

files_list_entry_t *find_entry_by_name(files_list_t *list, char *file_path, size_t start_of_src, size_t start_of_dest) {
    if (list == NULL || file_path == NULL) {
        return NULL;
    }

    files_list_entry_t *current = list->head;

    while (current != NULL) {
        int cmp_result = strcmp(current->path_and_name, file_path);

        if (cmp_result == 0) {
            return current;
        } else if (cmp_result > 0) {
            break;
        }

        current = current->next;
    }

    return NULL;
}

void display_files_list(files_list_t *list) {
    if (!list)
        return;

    for (files_list_entry_t *cursor = list->head; cursor != NULL; cursor = cursor->next) {
        printf("%s\n", cursor->path_and_name);
    }
}

void display_files_list_reversed(files_list_t *list) {
    if (!list)
        return;

    for (files_list_entry_t *cursor = list->tail; cursor != NULL; cursor = cursor->prev) {
        printf("%s\n", cursor->path_and_name);
    }
}
