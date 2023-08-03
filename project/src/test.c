#include <stdio.h>
#include <string.h>

#include "client_record.h"
#include "read_write_mes_func.h"

int main(void) {
    const char *filename = "test.dat";

    data_t expected_data = {
            .Number = 17,
            .Name = "blin",
            .Surname = "Galmykov",
            .addres = "tincaviar",
            .TelNumber = "536456",
            .indebtedness = 2347,
            .credit_limit = 569956,
            .cash_payments = 12270,
    };
    FILE *test_file = fopen(filename, "w+");
    write_to_file(test_file, &expected_data, CLIENT_DATA);
    data_t got_data;
    rewind(test_file);
    read_from_file(test_file, &got_data, CLIENT_DATA);
    if (expected_data.Number != got_data.Number ||
        strcmp(expected_data.Name, got_data.Name) != 0 ||
        strcmp(expected_data.Surname, got_data.Surname) != 0 ||
        strcmp(expected_data.addres, got_data.addres) != 0 ||
        strcmp(expected_data.TelNumber, got_data.TelNumber) != 0 ||
        expected_data.indebtedness != got_data.indebtedness ||
        expected_data.credit_limit != got_data.credit_limit ||
        expected_data.cash_payments != got_data.cash_payments) {
        printf("TEST NOT PASSED\n");
        return -1;
    }
    printf("TEST PASSED\n");
    return 0;
}