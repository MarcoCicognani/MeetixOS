#include "libm.h"

// FIXME

float _Complex cacosf(float _Complex z) {
    z = casinf(z);
    return CMPLXF((float)M_PI_2 - crealf(z), -cimagf(z));
}
