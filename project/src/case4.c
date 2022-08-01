#include "case4.h"

void output_sequence(int finish_num) {
    int sign = finish_num > 0 ? 1 : -1;
    if (finish_num != 1) {
        output_sequence(finish_num - 1*sign);
        printf(" %d", finish_num);
    } else {
        printf("%d", finish_num);
    }
}
