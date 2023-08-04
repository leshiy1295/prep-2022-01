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
    fclose(test_file);

    if (memcmp(&expected_data, &got_data, sizeof(data_t)) != 0) {
        printf("TEST NOT PASSED\n");
        return -1;
    }
    printf("TEST PASSED\n");
    return 0;
}
