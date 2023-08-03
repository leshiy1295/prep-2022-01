#ifndef MY_HW_VK23_READ_WRITE_MES_FUNC_H
#define MY_HW_VK23_READ_WRITE_MES_FUNC_H

#include "client_record.h"

#define CLIENT_DATA 1
#define TRANSFER_DATA 2

#define CLIENT_DATA_SIZE 8
#define TRANSFER_DATA_SIZE 2

enum MessageType {
    BEGINNING_MESSAGE = 1,
    CLIENT_DATA_MESSAGE = 2,
    TRANSACTION_DATA_MESSAGE = 3
};

int read_from_file(FILE* file_pointer, data_t* client_data, int read_type);
void write_to_file(FILE* file_pointer, const data_t* client_data, int write_type);
void puts_info_message(enum MessageType printf_type);

#endif //MY_HW_VK23_READ_WRITE_MES_FUNC_H
