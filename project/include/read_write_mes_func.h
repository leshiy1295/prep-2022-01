#ifndef MY_HW_VK23_READ_WRITE_MES_FUNC_H
#define MY_HW_VK23_READ_WRITE_MES_FUNC_H

#define BEGINNING_MESSAGE 1
#define CLIENT_DATA_MESSAGE 2
#define TRANSACTION_DATA_MESSAGE 3

typedef struct client_record data_t;

void  write_to_file(FILE *ofPTR, data_t Client);
void puts_info_message(int info_type);

#endif //MY_HW_VK23_READ_WRITE_MES_FUNC_H
