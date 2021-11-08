#include "libm.h"

float _Complex casinhf(float _Complex z) {
    z = casinf(CMPLXF(-cimagf(z), crealf(z)));
    return CMPLXF(cimagf(z), -crealf(z));
}
