#include "case3.h"

int prime_check(unsigned int number) {
    if (number == 0 || number == 1) {
        return 0;
    }
    for ( int i = 2; i * i <= number; i++ ) {
        if ( number % i == 0 ) {
            return 0;
        }
    }
    return 1;
}
