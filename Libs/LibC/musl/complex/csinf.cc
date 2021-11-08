#include "libm.h"

float _Complex csinf(float _Complex z) {
    z = csinhf(CMPLXF(-cimagf(z), crealf(z)));
    return CMPLXF(cimagf(z), -crealf(z));
}
