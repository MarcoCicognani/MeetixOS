#include "libm.h"

#if LDBL_MANT_DIG == 53 && LDBL_MAX_EXP == 1024
long double _Complex catanhl(long double _Complex z) {
    return catanh(z);
}
#else
long double _Complex catanhl(long double _Complex z) {
    z = catanl(CMPLXL(-cimagl(z), creall(z)));
    return CMPLXL(cimagl(z), -creall(z));
}
#endif
