#include <stdio.h>

#include "write_transaction.h"
#include "read_write_mes_func.h"
#include "client_record.h"

void write_transaction_to_file(char *filename) {
    FILE *transaction_pointer = fopen(filename, "w+");
    if (transaction_pointer == NULL) {
        puts("Cannot access transaction.dat");
    } else {
        data_t transfer;
        puts_info_message(TRANSACTION_DATA_MESSAGE);
        while (read_from_file(stdin, &transfer,TRANSFER_DATA) == TRANSFER_DATA_SIZE)) {
            write_to_file(transaction_pointer,&transfer,TRANSFER_DATA);
            puts_info_message(TRANSACTION_DATA_MESSAGE);
        }
        fclose(transaction_pointer);
    }
}
