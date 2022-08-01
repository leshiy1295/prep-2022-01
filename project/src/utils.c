#include "utils.h"
    size_t timer_from(int from) {
    size_t counter = 0;
    for (int i = from; i >= 0; --i) {
        ++counter;
        if (i == 0)
        printf("%d\n", i);
        else
        printf("%d ", i);
  }
    return counter;
}
int custom_pow(int base, int power) {
    if (power == 0) {
        return 1;
    }
    if (base == 1) {
        return 1;
    }
    int result = 1;
        for (int i = 1; i <= power; i++) {
           result*=base;
        }
    return result;
    }
