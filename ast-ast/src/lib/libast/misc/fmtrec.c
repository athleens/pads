/***********************************************************************
*                                                                      *
*               This software is part of the ast package               *
*          Copyright (c) 1985-2008 AT&T Intellectual Property          *
*                      and is licensed under the                       *
*                  Common Public License, Version 1.0                  *
*                    by AT&T Intellectual Property                     *
*                                                                      *
*                A copy of the License is available at                 *
*            http://www.opensource.org/licenses/cpl1.0.txt             *
*         (with md5 checksum 059e8cd6165cb4c31e351f2b69388fd9)         *
*                                                                      *
*              Information and Software Systems Research               *
*                            AT&T Research                             *
*                           Florham Park NJ                            *
*                                                                      *
*                 Glenn Fowler <gsf@research.att.com>                  *
*                  David Korn <dgk@research.att.com>                   *
*                   Phong Vo <kpv@research.att.com>                    *
*                                                                      *
***********************************************************************/
#pragma prototyped

/*
 * return the record format string given a format descriptor
 */

#include <recfmt.h>
#include <ctype.h>

char*
fmtrec(Recfmt_t f, int fs)
{
	char*	b;
	char*	e;
	char*	s;
	long	n;
	char	del[2];

	b = s = fmtbuf(n = 32);
	e = b + n;
	switch (RECTYPE(f))
	{
	case REC_delimited:
		*s++ = 'd';
		if ((del[0] = REC_D_DELIMITER(f)) != '\n')
		{
			del[1] = 0;
			if (fs)
				sfsprintf(s, e - s, "0x%02x", *(unsigned char*)del);
			else
				sfsprintf(s, e - s, "%s", fmtquote(del, NiL, NiL, 1, 0));
		}
		else
			*s = 0;
		break;
	case REC_fixed:
		if (!fs)
			*s++ = 'f';
		sfsprintf(s, e - s, "%lu", REC_F_SIZE(f));
		break;
	case REC_variable:
		*s++ = 'v';
		if (n = REC_V_SIZE(f))
			s += sfsprintf(s, e - s, "%lu", n);
		if (REC_V_HEADER(f) != 4)
			s += sfsprintf(s, e - s, "h%u", REC_V_HEADER(f));
		if (REC_V_OFFSET(f) != 0)
			s += sfsprintf(s, e - s, "o%u", REC_V_OFFSET(f));
		if (REC_V_LENGTH(f) != 2)
			s += sfsprintf(s, e - s, "z%u", REC_V_LENGTH(f));
		if (REC_V_LITTLE(f) != 0)
			*s++ = 'l';
		if (REC_V_INCLUSIVE(f) == 0)
			*s++ = 'n';
		*s = 0;
		break;
	case REC_method:
		*s++ = 'm';
		switch (n = REC_M_INDEX(f))
		{
		case REC_M_data:
			sfsprintf(s, e - s, "data");
			break;
		case REC_M_path:
			sfsprintf(s, e - s, "path");
			break;
		default:
			sfsprintf(s, e - s, "%lu", n);
			break;
		}
		break;
	case REC_none:
		*s++ = 'n';
		*s = 0;
		break;
	default:
		sfsprintf(s, e - s, "u%u.0x%07x", RECTYPE(f), REC_U_ATTRIBUTES(f));
		break;
	}
	return b;
}
