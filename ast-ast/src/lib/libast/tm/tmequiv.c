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
 * Glenn Fowler
 * AT&T Research
 *
 * time_t conversion support
 */

#include <tm.h>

/*
 * use one of the 14 equivalent calendar years to determine
 * daylight savings time for future years beyond the range
 * of the local system (via tmxmake())
 */

static const short equiv[] =
{
	2006, 2012,
	2001, 2024,
	2002, 2008,
	2003, 2020,
	2009, 2004,
	2010, 2016,
	2005, 2000,
};

/*
 * return the circa 2000 equivalent calendar year for tm
 */

int
tmequiv(Tm_t* tm)
{
	return tm->tm_year < (2038 - 1900) ? (tm->tm_year + 1900) : equiv[tm->tm_wday + tmisleapyear(tm->tm_year)];
}
