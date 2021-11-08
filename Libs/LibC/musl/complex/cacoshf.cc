#include "libm.h"

float _Complex cacoshf(float _Complex z) {
    z = cacosf(z);
    return CMPLXF(-cimagf(z), crealf(z));
}
