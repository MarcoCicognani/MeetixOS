#include "libm.h"

double carg(double _Complex z) {
    return atan2(cimag(z), creal(z));
}
