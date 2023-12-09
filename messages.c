#include <sys/msg.h>
#include <string.h>
#include <stdio.h>

#include "messages.h"

int send_file_entry(int msg_queue, int recipient, files_list_entry_t *file_entry, int cmd_code) {
    int result;
    any_message_t message;

    message.list_entry.op_code = cmd_code;
    message.list_entry.payload = *file_entry;

    result = msgsnd(msg_queue, &message, sizeof(files_list_entry_transmit_t), 0);
    if (result == -1) {
        perror("msgsnd");
    }

    return result;
}

int send_analyze_dir_command(int msg_queue, int recipient, char *target_dir) {
    int result;
    any_message_t message;

    message.analyze_dir_command.mtype = recipient;
    message.analyze_dir_command.op_code = COMMAND_CODE_ANALYZE_DIR;
    strncpy(message.analyze_dir_command.target, target_dir, PATH_SIZE - 1);
    message.analyze_dir_command.target[PATH_SIZE - 1] = '\0';

    result = msgsnd(msg_queue, &message, sizeof(analyze_dir_command_t), 0);
    if (result == -1) {
        perror("msgsnd");
    }

    return result;
}

int send_analyze_file_command(int msg_queue, int recipient, files_list_entry_t *file_entry) {
    return send_file_entry(msg_queue, recipient, file_entry, COMMAND_CODE_ANALYZE_FILE);
}

int send_analyze_file_response(int msg_queue, int recipient, files_list_entry_t *file_entry) {
    return send_file_entry(msg_queue, recipient, file_entry, COMMAND_CODE_FILE_ANALYZED);
}

int send_files_list_element(int msg_queue, int recipient, files_list_entry_t *file_entry) {
    return send_file_entry(msg_queue, recipient, file_entry, COMMAND_CODE_FILE_ENTRY);
}
