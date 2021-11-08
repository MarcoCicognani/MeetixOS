#include "libm.h"

// FIXME: Hull et al. "Implementing the _Complex arcsine and arccosine functions using exception
// handling" 1997

/* acos(z) = pi/2 - asin(z) */

double _Complex cacos(double _Complex z) {
    z = casin(z);
    return CMPLX(M_PI_2 - creal(z), -cimag(z));
}
