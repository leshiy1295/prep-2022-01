#include <stdio.h>

#include "read_write_mes_func.h"
#include "client_record.h"

int read_from_file(FILE* file_pointer, data_t* client_data, int read_type){
    if (file_pointer == NULL) {
        puts("Cannot access to file.\n");
        return 0;
    } else {
        switch (read_type) {
            case CLIENT_DATA:
                return fscanf(file_pointer,"%12d%11s%11s%16s%20s%12lf%12lf%12lf", &client_data->Number,
                              client_data->Name, client_data->Surname,client_data->addres,
                              client_data->TelNumber, &client_data->indebtedness, &client_data->credit_limit,
                              &client_data->cash_payments);
            case TRANSFER_DATA:
                return fscanf(file_pointer,"%d %lf", &client_data->Number, &client_data->cash_payments);
            default:
                return -1;
        }
    }
}
void write_to_file(FILE* file_pointer, const data_t* client_data, int write_type){
    if (file_pointer == NULL) {
        puts("Cannot access to file.\n");
        return;
    }
    switch (write_type) {
        case CLIENT_DATA:
            fprintf(file_pointer,"%12d%11s%11s%16s%20s%12lf%12lf%12lf\n",client_data->Number,
                    client_data->Name, client_data->Surname,client_data->addres,
                    client_data->TelNumber, client_data->indebtedness, client_data->credit_limit,
                    client_data->cash_payments);
            break;
        case TRANSFER_DATA:
            fprintf(file_pointer,"%d %lf\n", client_data->Number, client_data->cash_payments);
            break;
        default:
            break;
    }
}

void puts_info_message(enum MessageType printf_type) {
    switch (printf_type) {
        case BEGINNING_MESSAGE:
            printf("%s", "please enter action\n"
                         "1 enter data client:\n"
                         "2 enter data transaction:"
                         "\n3 update base\n");
            break;
        case CLIENT_DATA_MESSAGE:
            printf("%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n\n",
                   "1 Number account: ",
                   "2 Client name: ",
                   "3 Surname: ",
                   "4 Addres client: ",
                   "5 Client Telnum: ",
                   "6 Client indebtedness: ",
                   "7 Client credit limit: ",
                   "8 Client cash payments: ");
            break;
        case TRANSACTION_DATA_MESSAGE:
            printf("%s\n%s\n",
                   "1 Number account: ",
                   "2 Client cash payments: ");
            break;
        default:
            break;
    }
}
