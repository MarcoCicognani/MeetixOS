#include "libm.h"

#if LDBL_MANT_DIG == 53 && LDBL_MAX_EXP == 1024
long double _Complex cprojl(long double _Complex z) {
    return cproj(z);
}
#else
long double _Complex cprojl(long double _Complex z) {
    if ( isinf(creall(z)) || isinf(cimagl(z)) )
        return CMPLXL(INFINITY, copysignl(0.0, creall(z)));
    return z;
}
#endif
