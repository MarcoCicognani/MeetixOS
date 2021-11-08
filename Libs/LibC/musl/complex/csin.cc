#include "libm.h"

/* sin(z) = -i sinh(i z) */

double _Complex csin(double _Complex z) {
    z = csinh(CMPLX(-cimag(z), creal(z)));
    return CMPLX(cimag(z), -creal(z));
}
