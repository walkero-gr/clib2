/*
 * $Id: stdio_vsprintf.c,v 1.7 2006-01-08 12:04:25 clib2devs Exp $
*/

#ifndef _STDIO_HEADERS_H
#include "stdio_headers.h"
#endif /* _STDIO_HEADERS_H */

int
vsprintf(char *s, const char *format, va_list arg) {
    struct iob string_iob;
    int result = EOF;
    char buffer[32] = {0};

    assert(s != NULL && format != NULL);

    if (__check_abort_enabled)
        __check_abort();

    if (s == NULL || format == NULL) {
        __set_errno(EFAULT);
        goto out;
    }

    __initialize_iob(&string_iob, __vsprintf_hook_entry,
                     NULL,
                     buffer, sizeof(buffer),
                     -1,
                     -1,
                     IOBF_IN_USE | IOBF_WRITE | IOBF_BUFFER_MODE_NONE | IOBF_INTERNAL,
                     NULL);

    string_iob.iob_String = (STRPTR) s;

    result = vfprintf((FILE * ) &string_iob, format, arg);

    /* Put a \0 at the end */
    if (__putc('\0', (FILE * ) &string_iob, IOBF_BUFFER_MODE_NONE) == EOF) {
        result = EOF;
    }

out:

    return (result);
}
