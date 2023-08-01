#ifndef MY_HW_VK23_CLIENT_RECORD_H
#define MY_HW_VK23_CLIENT_RECORD_H

struct client_record {
    int Number;
    char Name[20];
    char Surname[20];
    char addres[30];
    char TelNumber[15];
    double indebtedness;
    double credit_limit;
    double cash_payments;
};
typedef struct client_record data_t;

#endif //MY_HW_VK23_CLIENT_RECORD_H
