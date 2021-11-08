#include "libm.h"

/* acosh(z) = i acos(z) */

double _Complex cacosh(double _Complex z) {
    z = cacos(z);
    return CMPLX(-cimag(z), creal(z));
}
