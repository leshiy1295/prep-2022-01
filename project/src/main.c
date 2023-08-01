#include <stdio.h>
#include <stdlib.h>


#include "utils.h"
#include "client_record.h"
#include "read_write_mes_func.h"

int main(void) {
    int choice = 0;
    FILE *Ptr, *Ptr_2, *blackrecord;
    data_t client_data, transfer;
    printf("%s", "please enter action\n"
                 "1 enter data client:\n"
                 "2 enter data transaction:"
                 "\n3 update base\n");

    while (scanf("%d", &choice) != -1) {
        switch (choice) {
            case 1:
                Ptr = fopen("record.dat", "r+");
                if (Ptr == NULL) {
                    puts("Not acess");
                } else {
                    write_to_file(Ptr, client_data);
                    fclose(Ptr);
                }
                break;
            case 2:
                Ptr = fopen("filename", "r+");
                if (Ptr == NULL) {
                    puts("Not acess");
                } else {
                    transactionWrite(Ptr, transfer);
                    fclose(Ptr);
                }
                break;
            case 3:
                Ptr = fopen("record.dat", "r");
                Ptr_2 = fopen("transaction.dat", "r");
                blackrecord = fopen("blackrecord.dat", "w");

                if (Ptr == NULL || Ptr_2 == NULL || blackrecord == NULL) {
                    puts("exit");
                } else {
                    blackRecord(Ptr, Ptr_2, blackrecord, client_data, transfer);
                    free(Ptr);
                    fclose(Ptr);
                    fclose(Ptr_2);
                    fclose(blackrecord);
                }
                break;
            default:
                puts("error");
                break;
        }
        printf("%s", "please enter action\n1 enter data client:\n2 enter data transaction:\n3 update base\n");
    }
    return 0;
}

//RHRTH FGHRT

