#include "libm.h"

float _Complex catanhf(float _Complex z) {
    z = catanf(CMPLXF(-cimagf(z), crealf(z)));
    return CMPLXF(cimagf(z), -crealf(z));
}
