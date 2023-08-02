#include <stdio.h>

#include "read_write_mes_func.h"
#include "client_record.h"
#include "write_transaction.h"


void  write_to_file(FILE *ofPTR, data_t Client) {
    puts_info_message(CLIENT_DATA_MESSAGE);
    while (scanf("%12d%11s%11s%16s%20s%12lf%12lf%12lf", &Client.Number, Client.Name, Client.Surname,
                 Client.addres, Client.TelNumber, &Client.indebtedness, &Client.credit_limit,
                 &Client.cash_payments) != -1) {
        fprintf(ofPTR, "%-12d%-11s%-11s%-16s%20s%12.2f%12.2f%12.2f\n", Client.Number, Client.Name,
                Client.Surname, Client.addres, Client.TelNumber, Client.indebtedness,
                Client.credit_limit, Client.cash_payments);
        puts_info_message(CLIENT_DATA_MESSAGE);
    }
}

void puts_info_message(int printf_type) {
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
