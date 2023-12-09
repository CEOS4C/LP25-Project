#include <sys/stat.h>
#include <dirent.h>
#include <openssl/evp.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>

#include "utility.h"
#include "file-properties.h"
#include "defines.h"

int get_file_stats(files_list_entry_t *entry) {
    if (entry == NULL) {
        return -1;
    }

    struct stat file_stat;
    if (stat(entry->path_and_name, &file_stat) != 0) {
        perror("stat");
        return -1;
    }

    entry->mode = file_stat.st_mode;
    entry->mtime = file_stat.st_mtim;

    if (S_ISDIR(file_stat.st_mode)) {
        entry->entry_type = DOSSIER;
    } else if (S_ISREG(file_stat.st_mode)) {
        entry->entry_type = FICHIER;

        if (compute_file_md5(entry) == -1) {
            return -1;
        }

    } else {
        fprintf(stderr, "File type not supported.\n");
        return -1;
    }

    return 0;
}
