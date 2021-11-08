#include "libm.h"

/* atanh = -i atan(i z) */

double _Complex catanh(double _Complex z) {
    z = catan(CMPLX(-cimag(z), creal(z)));
    return CMPLX(cimag(z), -creal(z));
}
