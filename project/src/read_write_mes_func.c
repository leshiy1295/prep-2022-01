#include <stdio.h>

#include "read_write_mes_func.h"
#include "client_record.h"


void  write_to_file(FILE *ofPTR, data_t Client) {
    printf("%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n\n",
           "1 Number account: ",
           "2 Client name: ",
           "3 Surname: ",
           "4 Addres client: ",
           "5 Client Telnum: ",
           "6 Client indebtedness: ",
           "7 Client credit limit: ",
           "8 Client cash payments: ");
    while (scanf("%12d%11s%11s%16s%20s%12lf%12lf%12lf", &Client.Number, Client.Name, Client.Surname,
                 Client.addres, Client.TelNumber, &Client.indebtedness, &Client.credit_limit,
                 &Client.cash_payments) != -1) {
        fprintf(ofPTR, "%-12d%-11s%-11s%-16s%20s%12.2f%12.2f%12.2f\n", Client.Number, Client.Name,
                Client.Surname, Client.addres, Client.TelNumber, Client.indebtedness,
                Client.credit_limit, Client.cash_payments);
        printf("%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n\n",
               "1 Number account: ",
               "2 Client name: ",
               "3 Surname: ",
               "4 Addres client: ",
               "5 Client Telnum: ",
               "6 Client indebtedness: ",
               "7 Client credit limit: ",
               "8 Client cash payments:"
        );
    }
}