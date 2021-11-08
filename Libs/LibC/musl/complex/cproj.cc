#include "libm.h"

double _Complex cproj(double _Complex z) {
    if ( isinf(creal(z)) || isinf(cimag(z)) )
        return CMPLX(INFINITY, copysign(0.0, creal(z)));
    return z;
}
