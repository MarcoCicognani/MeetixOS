#include "libm.h"

/* asinh(z) = -i asin(i z) */

double _Complex casinh(double _Complex z) {
    z = casin(CMPLX(-cimag(z), creal(z)));
    return CMPLX(cimag(z), -creal(z));
}
