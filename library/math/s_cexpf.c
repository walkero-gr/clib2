/*
 * $Id: math_s_cexpf.c,v 1.1 2023-07-19 11:33:52 clib4devs Exp $
 */

#ifndef _COMPLEX_HEADERS_H
#include "complex_headers.h"
#endif /* _COMPLEX_HEADERS_H */

#ifndef _MATH_HEADERS_H
#include "math_headers.h"
#endif /* _MATH_HEADERS_H */

static const uint32_t
        exp_ovfl = 0x42b17218,        /* MAX_EXP * ln2 ~= 88.722839355 */
        cexp_ovfl = 0x43400074;        /* (MAX_EXP - MIN_DENORM_EXP) * ln2 */

float complex
cexpf(float complex z) {
    float x, y, exp_x;
    uint32_t hx, hy;

    x = crealf(z);
    y = cimagf(z);

    GET_FLOAT_WORD(hy, y);
    hy &= 0x7fffffff;

    /* cexp(x + I 0) = exp(x) + I 0 */
    if (hy == 0)
        return (CMPLXF(expf(x), y));
    GET_FLOAT_WORD(hx, x);
    /* cexp(0 + I y) = cos(y) + I sin(y) */
    if ((hx & 0x7fffffff) == 0)
        return (CMPLXF(cosf(y), sinf(y)));

    if (hy >= 0x7f800000) {
        if ((hx & 0x7fffffff) != 0x7f800000) {
            /* cexp(finite|NaN +- I Inf|NaN) = NaN + I NaN */
            return (CMPLXF(y - y, y - y));
        } else if (hx & 0x80000000) {
            /* cexp(-Inf +- I Inf|NaN) = 0 + I 0 */
            return (CMPLXF(0.0, 0.0));
        } else {
            /* cexp(+Inf +- I Inf|NaN) = Inf + I NaN */
            return (CMPLXF(x, y - y));
        }
    }

    if (hx >= exp_ovfl && hx <= cexp_ovfl) {
        /*
         * x is between 88.7 and 192, so we must scale to avoid
         * overflow in expf(x).
         */
        return (__ldexp_cexpf(z, 0));
    } else {
        /*
         * Cases covered here:
         *  -  x < exp_ovfl and exp(x) won't overflow (common case)
         *  -  x > cexp_ovfl, so exp(x) * s overflows for all s > 0
         *  -  x = +-Inf (generated by exp())
         *  -  x = NaN (spurious inexact exception from y)
         */
        exp_x = expf(x);
        return (CMPLXF(exp_x * cosf(y), exp_x * sinf(y)));
    }
}
