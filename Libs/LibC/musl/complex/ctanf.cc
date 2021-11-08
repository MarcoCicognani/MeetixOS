#include "libm.h"

float _Complex ctanf(float _Complex z) {
    z = ctanhf(CMPLXF(-cimagf(z), crealf(z)));
    return CMPLXF(cimagf(z), -crealf(z));
}
