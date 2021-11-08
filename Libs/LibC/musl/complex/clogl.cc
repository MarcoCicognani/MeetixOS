#include "libm.h"

#if LDBL_MANT_DIG == 53 && LDBL_MAX_EXP == 1024
long double _Complex clogl(long double _Complex z) {
    return clog(z);
}
#else
// FIXME
long double _Complex clogl(long double _Complex z) {
    long double r, phi;

    r   = cabsl(z);
    phi = cargl(z);
    return CMPLXL(logl(r), phi);
}
#endif
