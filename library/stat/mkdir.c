/*
 * $Id: stat_mkdir.c,v 1.7 2006-01-08 12:04:24 obarthel Exp $
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

#ifndef _STAT_HEADERS_H
#include "stat_headers.h"
#endif /* _STAT_HEADERS_H */

int mkdir(const char *path_name, mode_t mode)
{
#if defined(UNIX_PATH_SEMANTICS)
	struct name_translation_info path_name_nti;
#endif /* UNIX_PATH_SEMANTICS */
	ULONG protection;
	int result = ERROR;
	BPTR dir_lock;

	ENTER();

	SHOWSTRING(path_name);
	SHOWVALUE(mode);

	assert(path_name != NULL);

	if (__check_abort_enabled)
		__check_abort();

    if (path_name == NULL)
    {
        SHOWMSG("invalid path name parameter");

        __set_errno(EFAULT);
        goto out;
    }

#if defined(UNIX_PATH_SEMANTICS)
	if (__global_clib2->__unix_path_semantics)
	{
		if (__translate_unix_to_amiga_path_name(&path_name, &path_name_nti) != 0)
			goto out;

		if (path_name_nti.is_root)
		{
			__set_errno(EACCES);
			goto out;
		}
	}
#endif /* UNIX_PATH_SEMANTICS */

	D(("trying to create '%s'", path_name));

	dir_lock = CreateDir((STRPTR)path_name);
	if (dir_lock == ZERO)
	{
		SHOWMSG("that didn't work");

		__set_errno(__translate_access_io_error_to_errno(IoErr()));
		goto out;
	}

	UnLock(dir_lock);

	protection = 0;

	if (FLAG_IS_SET(mode, S_IRUSR))
		SET_FLAG(protection, FIBF_READ);

	if (FLAG_IS_SET(mode, S_IWUSR))
	{
		SET_FLAG(protection, FIBF_WRITE);
		SET_FLAG(protection, FIBF_DELETE);
	}

	if (FLAG_IS_SET(mode, S_IXUSR))
		SET_FLAG(protection, FIBF_EXECUTE);

	if (FLAG_IS_SET(mode, S_IRGRP))
		SET_FLAG(protection, FIBF_GRP_READ);

	if (FLAG_IS_SET(mode, S_IWGRP))
	{
		SET_FLAG(protection, FIBF_GRP_WRITE);
		SET_FLAG(protection, FIBF_GRP_DELETE);
	}

	if (FLAG_IS_SET(mode, S_IXGRP))
		SET_FLAG(protection, FIBF_GRP_EXECUTE);

	if (FLAG_IS_SET(mode, S_IROTH))
		SET_FLAG(protection, FIBF_OTR_READ);

	if (FLAG_IS_SET(mode, S_IWOTH))
	{
		SET_FLAG(protection, FIBF_OTR_WRITE);
		SET_FLAG(protection, FIBF_OTR_DELETE);
	}

	if (FLAG_IS_SET(mode, S_IXOTH))
		SET_FLAG(protection, FIBF_OTR_EXECUTE);

	SHOWSTRING(path_name);
	SHOWVALUE(protection);

	SetProtection((STRPTR)path_name, (LONG)(protection ^ (FIBF_READ | FIBF_WRITE | FIBF_EXECUTE | FIBF_DELETE)));

	result = OK;

out:

	RETURN(result);
	return (result);
}