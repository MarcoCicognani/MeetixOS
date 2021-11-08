#include "libm.h"

float cargf(float _Complex z) {
    return atan2f(cimagf(z), crealf(z));
}
