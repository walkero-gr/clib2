/*
 * $Id: shcrtbegin.c,v 1.0 2021-02-01 17:22:03 clib2devs Exp $
 *
 * :ts=4
 *
 * Handles global constructors and destructors for the OS4 GCC build.
 *
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

#ifndef _STDLIB_HEADERS_H
#include "stdlib_headers.h"
#endif /* _STDLIB_HEADERS_H */

#include <proto/exec.h>

/* Avoid gcc warnings.. */
void __shlib_call_constructors(void);
void __shlib_call_destructors(void);
void _init(void);
void _fini(void);

static void (*__CTOR_LIST__[1])(void) __attribute__((used, section(".ctors"), aligned(sizeof(void (*)(void)))));
static void (*__DTOR_LIST__[1])(void) __attribute__((used, section(".dtors"), aligned(sizeof(void (*)(void)))));

void _init(void) {}
void _fini(void) {}
static BOOL success = FALSE;

#define MIN_OS_VERSION 52

static BOOL
open_libraries(VOID) {
    /* Open the minimum required libraries. */
    DOSBase = (struct Library *)OpenLibrary("dos.library", MIN_OS_VERSION);
    if (DOSBase == NULL)
        goto out;

    __UtilityBase = OpenLibrary("utility.library", MIN_OS_VERSION);
    if (__UtilityBase == NULL)
        goto out;

    /* Obtain the interfaces for these libraries. */
    IDOS = (struct DOSIFace *)GetInterface(DOSBase, "main", 1, 0);
    if (IDOS == NULL)
        goto out;

    __IUtility = (struct UtilityIFace *)GetInterface(__UtilityBase, "main", 1, 0);
    if (__IUtility == NULL)
    goto out;

    success = TRUE;

out:

    return(success);
}

void __shlib_call_constructors(void) {
    if (open_libraries()) {
        int i = 0;

        while (__CTOR_LIST__[i + 1]) {
            i++;
        }

        while (i > 0) {
            Printf("i1 = %ld\n", i);
            __CTOR_LIST__[i--]();
        }
    }
}

void __shlib_call_destructors(void) {
    if (success) {
        int i = 1;

        while (__DTOR_LIST__[i]) {
            __DTOR_LIST__[i++]();
        }
    }
}
