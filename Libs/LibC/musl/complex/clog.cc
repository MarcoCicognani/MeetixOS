#include "libm.h"

// FIXME

/* log(z) = log(|z|) + i arg(z) */

double _Complex clog(double _Complex z) {
    double r, phi;

    r   = cabs(z);
    phi = carg(z);
    return CMPLX(log(r), phi);
}
