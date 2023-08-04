#include <stdio.h>

#include "upd_base_client.h"
#include "read_write_mes_func.h"
#include " create_new_client.h"
#include "write_transaction.h"

#define ADD_NEW_CLIENT 1
#define ADD_NEW_TRANSACTION 2
#define UPDATE_DATABASE 3

#define CLIENTS_FILENAME "record.dat"
#define TRANSACTION_FILENAME "transaction.dat"
#define UPDATED_CLIENTS_FILENAME "blackrecord.dat"

int main(void) {
    int choice = 0;
    puts_info_message(BEGINNING_MESSAGE);
    while (scanf("%d", &choice) != -1) {
        switch (choice) {
            case ADD_NEW_CLIENT:
                    write_client_to_file(CLIENTS_FILENAME);
                break;
            case ADD_NEW_TRANSACTION:
                    write_transaction_to_file(TRANSACTION_FILENAME);

                break;
            case UPDATE_DATABASE:
                    write_transfer_cash(CLIENTS_FILENAME, TRANSACTION_FILENAME, UPDATED_CLIENTS_FILENAME);
                break;
            default:
                puts("Invalid choice, try again.");
                break;
        }
        puts_info_message(BEGINNING_MESSAGE);
    }
    return 0;
}
