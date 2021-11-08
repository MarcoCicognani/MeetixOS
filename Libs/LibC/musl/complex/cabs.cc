#include "libm.h"

double cabs(double _Complex z) {
    return hypot(creal(z), cimag(z));
}
