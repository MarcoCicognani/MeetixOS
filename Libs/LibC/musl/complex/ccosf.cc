#include "libm.h"

float _Complex ccosf(float _Complex z) {
    return ccoshf(CMPLXF(-cimagf(z), crealf(z)));
}
