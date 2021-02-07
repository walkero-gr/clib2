/*
 * $Id: unistd_sysconf.c,v 1.0 2021-01-19 10:09:27 apalmate Exp $
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

#ifndef _UNISTD_HEADERS_H
#include "unistd_headers.h"
#endif /* _UNISTD_HEADERS_H */

#include <sys/syslimits.h>
#include <netdb.h>

long 
sysconf(int name) {
    int retval = -1;
    ULONG query;

    switch (name) {
        case _SC_ARG_MAX:
            return ARG_MAX;
        case _SC_HOST_NAME_MAX:
            return MAXHOSTNAMELEN;
        case _SC_CLK_TCK:
            return CLK_TCK;
        case _SC_OPEN_MAX:
            return FOPEN_MAX;
        case _SC_PAGESIZE:
            GetCPUInfoTags(GCIT_ExecPageSize, (ULONG)&query, TAG_DONE);
            break;
        case _SC_TZNAME_MAX:
            return MAX_TZSIZE;
        case _SC_NPROCESSORS_CONF:
            GetCPUInfoTags(GCIT_NumberOfCPUs, (ULONG)&query, TAG_DONE);
            break;
        default:
            __set_errno(EINVAL);
            break;
    }
    retval = query;

    return retval;
}