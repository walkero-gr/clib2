/*
 * $Id: math_init_exit.c,v 1.19 2006-01-08 12:04:23 obarthel Exp $
 *
 * :ts=4
 *
 * Portable ISO 'C' (1994) runtime library for the Amiga computer
 * Copyright (c) 2002-2015 by Olaf Barthel <obarthel (at) gmx.net>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *   - Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *
 *   - Neither the name of Olaf Barthel nor the names of contributors
 *     may be used to endorse or promote products derived from this
 *     software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _MATH_HEADERS_H
#include "math_headers.h"
#endif /* _MATH_HEADERS_H */

/****************************************************************************/

#ifndef _STDLIB_CONSTRUCTOR_H
#include "stdlib_constructor.h"
#endif /* _STDLIB_CONSTRUCTOR_H */

float NOCOMMON __infinity;
float NOCOMMON __nan;
float NOCOMMON __huge_val_float;
long double NOCOMMON __huge_val_long_double;
double NOCOMMON __huge_val;

MATH_CONSTRUCTOR(math_init)
{
    union ieee_double *double_x;
    union ieee_single *single_x;
    union ieee_long_double *x;

	BOOL success = FALSE;

	ENTER();

    /* Now fill in HUGE_VAL and HUGE_VALF, respectively. TODO:
       also take care of HUGE_VALL. */

    /* Exponent = +126, Mantissa = 8,388,607 */
    single_x = (union ieee_single *)&__huge_val_float;
    single_x->raw[0] = 0x7f7fffff;

    /* Exponent = +1022, Mantissa = 4,503,599,627,370,495 */
    double_x = (union ieee_double *)&__huge_val;
    double_x->raw[0] = 0x7fefffff;
    double_x->raw[1] = 0xffffffff;

    x = (union ieee_long_double *)&__huge_val_long_double;
    /* Exponent = +32766, Mantissa = 18,446,744,073,709,551,615 */
    x->raw[0] = 0x7ffe0000;
    x->raw[1] = 0xffffffff;
    x->raw[2] = 0xffffffff;

    /* Finally, fill in the constants behind INFINITY and NAN. */
    single_x = (union ieee_single *)&__infinity;
    single_x->raw[0] = 0x7f800000;

    single_x = (union ieee_single *)&__nan;
    single_x->raw[0] = 0x7fc00001;

	success = TRUE;

out:

	SHOWVALUE(success);
	LEAVE();

	if (success)
		CONSTRUCTOR_SUCCEED();
	else
		CONSTRUCTOR_FAIL();
}
