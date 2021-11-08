#include "libm.h"

long double _Complex conjl(long double _Complex z) {
    return CMPLXL(creall(z), -cimagl(z));
}
