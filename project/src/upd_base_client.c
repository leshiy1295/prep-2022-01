#include <stdio.h>

#include "upd_base_client.h"
#include "read_write_mes_func.h"
#include "client_record.h"


void  write_transfer_cash(char *client_base_filename, char *transaction_filename, char *upd_base_filename) {
    FILE* record_pointer = fopen(client_base_filename, "r");
    FILE* transaction_pointer = fopen(transaction_filename, "r");
    FILE* up_record_pointer = fopen(upd_base_filename, "w");
    if (record_pointer == NULL || transaction_pointer == NULL || up_record_pointer == NULL) {
        puts("Cannot access one of the files");
    } else {
        data_t client_data, transfer;

        while (read_from_file(record_pointer,&client_data, CLIENT_DATA) == CLIENT_DATA_SIZE) {
            while (read_from_file(transaction_pointer,&transfer, TRANSFER_DATA) == TRANSFER_DATA_SIZE) {
                if (client_data.Number == transfer.Number && transfer.cash_payments != 0) {
                    client_data.credit_limit += transfer.cash_payments;
                }
            }
            write_to_file(up_record_pointer,& client_data,CLIENT_DATA);
            rewind(transaction_pointer);
        }
        fclose(record_pointer);
        fclose(transaction_pointer);
        fclose(up_record_pointer);
    }
}
