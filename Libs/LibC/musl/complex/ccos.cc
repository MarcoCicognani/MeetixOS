#include "libm.h"

/* cos(z) = cosh(i z) */

double _Complex ccos(double _Complex z) {
    return ccosh(CMPLX(-cimag(z), creal(z)));
}
