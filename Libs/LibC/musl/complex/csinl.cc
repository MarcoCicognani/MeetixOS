#include "libm.h"

#if LDBL_MANT_DIG == 53 && LDBL_MAX_EXP == 1024
long double _Complex csinl(long double _Complex z) {
    return csin(z);
}
#else
long double _Complex csinl(long double _Complex z) {
    z = csinhl(CMPLXL(-cimagl(z), creall(z)));
    return CMPLXL(cimagl(z), -creall(z));
}
#endif
