#include <stdio.h>

#include "read_write_mes_func.h"
#include "client_record.h"
#include " create_new_client.h"

void  write_client_to_file(char *filename) {
    FILE* record_pointer = fopen(filename , "w+");
    if (record_pointer == NULL) {
        puts("Cannot access record.dat");
    } else {
        puts_info_message(CLIENT_DATA_MESSAGE);
        data_t new_client;
        while (read_from_file(stdin, &new_client,CLIENT_DATA) == CLIENT_DATA_SIZE){
            write_to_file(record_pointer, &new_client,CLIENT_DATA);
            puts_info_message(CLIENT_DATA_MESSAGE);
        }
        fclose(record_pointer);
    }
}
