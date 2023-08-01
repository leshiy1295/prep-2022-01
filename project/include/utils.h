#ifndef UTILS_H
#define UTILS_H

#include "client_record.h"

void transactionWrite(FILE *ofPTR, data_t transfer);
void  blackRecord(FILE *ofPTR, FILE *ofPTR_2, FILE *blackrecord, data_t client_data, data_t transfer);

#endif //UTILS_H
