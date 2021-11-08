#include "libm.h"

float _Complex cprojf(float _Complex z) {
    if ( isinf(crealf(z)) || isinf(cimagf(z)) )
        return CMPLXF(INFINITY, copysignf(0.0, crealf(z)));
    return z;
}
