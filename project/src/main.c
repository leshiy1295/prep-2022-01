#include <stdio.h>
#include <stdlib.h>

#include "utils.h"
#include "write_transaction.h"
#include "client_record.h"
#include "read_write_mes_func.h"

#define ADD_NEW_CLIENT 1
#define ADD_NEW_TRANSACTION 2
#define UPDATE_DATABASE 3

#define CLIENTS_FILENAME "record.dat"
#define TRANSACTION_FILENAME "transaction.dat"
#define UPDATED_CLIENTS_FILENAME "blackrecord.dat"

int main(void) {
    int choice = 0;
    FILE *record_pointer, *transaction_pointer, *up_record_pointer;
    data_t client_data, transfer;

    puts_info_message(BEGINNING_MESSAGE);
    while (scanf("%d", &choice) != -1) {
        switch (choice) {
            case ADD_NEW_CLIENT:
                record_pointer = fopen(CLIENTS_FILENAME , "a+");
                if (record_pointer == NULL) {
                    puts("Cannot access record.dat");
                } else {
                    write_to_file(record_pointer, client_data);
                    fclose(record_pointer);
                }
                break;
            case ADD_NEW_TRANSACTION:
                transaction_pointer = fopen(TRANSACTION_FILENAME, "r+");
                if (transaction_pointer == NULL) {
                    puts("Cannot access transaction.dat");
                } else {
                    transactionWrite(transaction_pointer, transfer);
                    fclose(transaction_pointer);
                }
                break;
            case UPDATE_DATABASE:
                record_pointer = fopen(CLIENTS_FILENAME , "r");
                transaction_pointer = fopen(TRANSACTION_FILENAME, "r");
                up_record_pointer = fopen(UPDATED_CLIENTS_FILENAME, "w");

                if (record_pointer == NULL || transaction_pointer == NULL || up_record_pointer == NULL) {
                    puts("Cannot access one of the files");
                } else {
                    blackRecord(record_pointer, transaction_pointer, up_record_pointer, client_data, transfer);
                    //free(Ptr);
                    fclose(record_pointer);
                    fclose(transaction_pointer);
                    fclose(up_record_pointer);
                }
                break;
            default:
                puts("Invalid choice, try again.");
                break;
        }
        puts_info_message(BEGINNING_MESSAGE);
    }
    return 0;
}



