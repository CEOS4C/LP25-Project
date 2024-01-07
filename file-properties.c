#include <sys/stat.h>
#include <openssl/evp.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>

#include "file-properties.h"
#include "defines.h"


/*!
 * @brief get_file_stats gets all of the required information for a file (inc. directories)
 * @param the files list entry
 * You must get:
 * - for files:
 *   - mode (permissions)
 *   - mtime (in nanoseconds)
 *   - size
 *   - entry type (FICHIER)
 *   - MD5 sum
 * - for directories:
 *   - mode
 *   - entry type (DOSSIER)
 * @return -1 in case of error, 0 else
 */
int get_file_stats(files_list_entry_t *entry) {
    if (entry == NULL) {
        return -1;
    }

    struct stat file_stat;
    if (stat(entry->path_and_name, &file_stat) != 0) {
        perror("stat");
        return -1;
    }

    entry->mtime.tv_sec = file_stat.st_mtime;
    entry->mtime.tv_nsec = 0;

    entry->size = file_stat.st_size;
    entry->mode = file_stat.st_mode;

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


/*!
 * @brief compute_file_md5 computes a file's MD5 sum
 * @param the pointer to the files list entry
 * @return -1 in case of error, 0 else
 * Use libcrypto functions from openssl/evp.h
 */
int compute_file_md5(files_list_entry_t *entry) {
    if (entry == NULL || entry->path_and_name == NULL) {
        fprintf(stderr, "Invalid entry or path.\n");
        return -1;
    }
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


/*!
 * @brief directory_exists tests the existence of a directory
 * @path_to_dir a string with the path to the directory
 * @return true if directory exists, false else
 */
bool directory_exists(char *path_to_dir) {
    if (path_to_dir == NULL || *path_to_dir == '\0') {
        fprintf(stderr, "Invalid path to directory.\n");
        return false;
    }
    struct stat dir_stat;
    return stat(path_to_dir, &dir_stat) == 0 && S_ISDIR(dir_stat.st_mode);
}


/*!
 * @brief is_directory_writable tests if a directory is writable
 * @param path_to_dir the path to the directory to test
 * @return true if dir is writable, false else
 * Hint: try to open a file in write mode in the target directory.
 */
bool is_directory_writable(char *path_to_dir) {
    if (path_to_dir == NULL || *path_to_dir == '\0') {
        fprintf(stderr, "Invalid path to directory.\n");
        return false;
    }

    char temp_file_path[PATH_SIZE];
    snprintf(temp_file_path, sizeof(temp_file_path), "%s/temp_file_for_write_test", path_to_dir);

    int fd = open(temp_file_path, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fd != -1) {
        close(fd);

        unlink(temp_file_path);

        return true;
    }

    return false;
}
