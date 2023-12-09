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

int compute_file_md5(files_list_entry_t *entry) {
    FILE *file = fopen(entry->path_and_name, "rb");
    if (!file) {
        perror("fopen");
        return -1;
    }

    EVP_MD_CTX *mdctx = EVP_MD_CTX_new();
    const EVP_MD *md = EVP_md5();

    if ((!mdctx || !md) || (1 != EVP_DigestInit_ex(mdctx, md, NULL))) {
        fclose(file);
        EVP_MD_CTX_free(mdctx);
        perror("EVP_DigestInit_ex");
        return -1;
    }

    unsigned char buffer[1024];
    size_t bytes;
    while ((bytes = fread(buffer, 1, sizeof(buffer), file)) != 0) {
        if (1 != EVP_DigestUpdate(mdctx, buffer, bytes)) {
            fclose(file);
            EVP_MD_CTX_free(mdctx);
            perror("EVP_DigestUpdate");
            return -1;
        }
    }

    unsigned int md_len;
    if (1 != EVP_DigestFinal_ex(mdctx, entry->md5sum, &md_len)) {
        fclose(file);
        EVP_MD_CTX_free(mdctx);
        perror("EVP_DigestFinal_ex");
        return -1;
    }

    EVP_MD_CTX_free(mdctx);
    fclose(file);

    return 0;
}
