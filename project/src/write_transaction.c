#include <stdio.h>

#include "utils.h"
#include "write_transaction.h"
#include "read_write_mes_func.h"
#include "client_record.h"

void transactionWrite(FILE *ofPtr, data_t transfer) {
    puts_info_message(TRANSACTION_DATA_MESSAGE);
    while (scanf("%d %lf", &transfer.Number, &transfer.cash_payments) != -1) {
        fprintf(ofPtr, "%-3d%-6.2f\n", transfer.Number, transfer.cash_payments);
        puts_info_message(TRANSACTION_DATA_MESSAGE);
    }
}
