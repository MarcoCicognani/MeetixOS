#include "libm.h"

#if LDBL_MANT_DIG == 53 && LDBL_MAX_EXP == 1024
long double cargl(long double _Complex z) {
    return carg(z);
}
#else
long double cargl(long double _Complex z) {
    return atan2l(cimagl(z), creall(z));
}
#endif
