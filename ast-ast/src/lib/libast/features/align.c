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
 * AT&T Bell Laboratories
 *
 * generate align features
 *
 * NOTE: two's complement binary integral representation assumed
 */

#include "FEATURE/common"

#include <setjmp.h>

union _u_
{
	long			u1;
	char*			u2;
	double			u3;
	char			u4[1024];
	intmax_t		u5;
	uintmax_t		u6;
	_ast_fltmax_t		u7;
	void*			u8;
	char*			(*u9)();
	jmp_buf			u10;
};

struct _s_
{
	char		s1;
	union _u_	s2;
};

#define roundof(x,y)	(((x)+((y)-1))&~((y)-1))

static union _u_	u;
static union _u_	v;

int
main()
{
	register int	i;
	register int	j;
	register int	k;

	int		align0;
	int		align1;
	int		align2;
	unsigned long	bit1;
	unsigned long	bit2;
	unsigned long	bits0;
	unsigned long	bits1;
	unsigned long	bits2;

	u.u2 = u.u4;
	v.u2 = u.u2 + 1;
	bit1 = u.u1 ^ v.u1;
	v.u2 = u.u2 + 2;
	bit2 = u.u1 ^ v.u1;
	align0 = sizeof(struct _s_) - sizeof(union _u_);
	bits0 = 0;
	k = 0;
	for (j = 0; j < align0; j++)
	{
		u.u2 = u.u4 + j;
		bits1 = 0;
		for (i = 0; i < align0; i++)
		{
			v.u2 = u.u2 + i;
			bits1 |= u.u1 ^ v.u1;
		}
		if (!bits0 || bits1 < bits0)
		{
			bits0 = bits1;
			k = j;
		}
	}
	align1 = roundof(align0, 2);
	u.u2 = u.u4 + k;
	for (bits1 = bits0; i < align1; i++)
	{
		v.u2 = u.u2 + i;
		bits1 |= u.u1 ^ v.u1;
	}
	align2 = roundof(align0, 4);
	for (bits2 = bits1; i < align2; i++)
	{
		v.u2 = u.u2 + i;
		bits2 |= u.u1 ^ v.u1;
	}
	printf("typedef unsigned %s ALIGN_INTEGRAL;\n", sizeof(char*) >= sizeof(long) ? "long" : sizeof(char*) >= sizeof(int) ? "int" : "short");
	printf("\n");
	printf("#define ALIGN_CHUNK		%d\n", sizeof(char*) >= 4 ? 8192 : 1024);
	printf("#define ALIGN_INTEGRAL		%s\n", sizeof(char*) >= sizeof(long) ? "long" : sizeof(char*) >= sizeof(int) ? "int" : "short");
	printf("#define ALIGN_INTEGER(x)	((ALIGN_INTEGRAL)(x))\n");
	printf("#define ALIGN_POINTER(x)	((char*)(x))\n");
	if (bits2 == (align2 - 1)) printf("#define ALIGN_ROUND(x,y)	ALIGN_POINTER(ALIGN_INTEGER((x)+(y)-1)&~((y)-1))\n");
	else printf("#define ALIGN_ROUND(x,y)	ALIGN_POINTER(ALIGN_INTEGER(ALIGN_ALIGN(x)+(((y)+%d)/%d)-1)&~((((y)+%d)/%d)-1))\n", align0, align0, align0, align0);
	printf("\n");
	if (align0 == align2)
	{
		printf("#define ALIGN_BOUND		ALIGN_BOUND2\n");
		printf("#define ALIGN_ALIGN(x)		ALIGN_ALIGN2(x)\n");
		printf("#define ALIGN_TRUNC(x)		ALIGN_TRUNC2(x)\n");
	}
	else if (align0 == align1)
	{
		printf("#define ALIGN_BOUND		ALIGN_BOUND1\n");
		printf("#define ALIGN_ALIGN(x)		ALIGN_ALIGN1(x)\n");
		printf("#define ALIGN_TRUNC(x)		ALIGN_TRUNC1(x)\n");
	}
	else
	{
		printf("#define ALIGN_BOUND		1\n");
		printf("#define ALIGN_ALIGN(x)		ALIGN_POINTER(x)\n");
		printf("#define ALIGN_TRUNC(x)		ALIGN_POINTER(x)\n");
	}
	printf("\n");
	printf("#define ALIGN_BIT1		0x%lx\n", bit1);
	if (align1 == align2)
	{
		printf("#define ALIGN_BOUND1		ALIGN_BOUND2\n");
		printf("#define ALIGN_ALIGN1(x)		ALIGN_ALIGN2(x)\n");
		printf("#define ALIGN_TRUNC1(x)		ALIGN_TRUNC2(x)\n");
	}
	else
	{
		printf("#define ALIGN_BOUND1		%d\n", align1);
		printf("#define ALIGN_ALIGN1(x)		ALIGN_TRUNC1((x)+%d)\n", align1 - 1);
		printf("#define ALIGN_TRUNC1(x)		ALIGN_POINTER(ALIGN_INTEGER((x)+%d)&0x%lx)\n", align1 - 1, ~(bits0|bits1));
	}
	printf("#define ALIGN_CLRBIT1(x)	ALIGN_POINTER(ALIGN_INTEGER(x)&0x%lx)\n", ~bit1);
	printf("#define ALIGN_SETBIT1(x)	ALIGN_POINTER(ALIGN_INTEGER(x)|0x%lx)\n", bit1);
	printf("#define ALIGN_TSTBIT1(x)	ALIGN_POINTER(ALIGN_INTEGER(x)&0x%lx)\n", bit1);
	printf("\n");
	printf("#define ALIGN_BIT2		0x%lx\n", bit2);
	printf("#define ALIGN_BOUND2		%d\n", align2);
	printf("#define ALIGN_ALIGN2(x)		ALIGN_TRUNC2((x)+%d)\n", align2 - 1);
	printf("#define ALIGN_TRUNC2(x)		ALIGN_POINTER(ALIGN_INTEGER(x)&0x%lx)\n", ~(bits0|bits1|bits2));
	printf("#define ALIGN_CLRBIT2(x)	ALIGN_POINTER(ALIGN_INTEGER(x)&0x%lx)\n", ~bit2);
	printf("#define ALIGN_SETBIT2(x)	ALIGN_POINTER(ALIGN_INTEGER(x)|0x%lx)\n", bit2);
	printf("#define ALIGN_TSTBIT2(x)	ALIGN_POINTER(ALIGN_INTEGER(x)&0x%lx)\n", bit2);
	printf("\n");
	return 0;
}
