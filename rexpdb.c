/********************************************
rexpdb.c
copyright 2008,2010, Thomas E. Dickey
copyright 1991,1993, Michael D. Brennan

This is a source file for mawk, an implementation of
the AWK programming language.

Mawk is distributed without warranty under the terms of
the GNU General Public License, version 2, 1991.
********************************************/

/*
 * $MawkId: rexpdb.c,v 1.11 2010/12/10 17:00:00 tom Exp $
 * @Log: rexpdb.c,v @
 * Revision 1.2  1993/07/23  13:21:51  mike
 * cleanup rexp code
 *
 * Revision 1.1.1.1  1993/07/03  18:58:28  mike
 * move source to cvs
 *
 * Revision 3.2  1991/08/13  09:10:09  brennan
 * VERSION .9994
 *
 * Revision 3.1  91/06/07  10:33:30  brennan
 * VERSION 0.995
 *
*/

#include "rexp.h"
#include <ctype.h>

/*  print a machine for debugging  */

static const char *xlat[] =
{
    "M_STR",
    "M_CLASS",
    "M_ANY",
    "M_START",
    "M_END",
    "M_U",
    "M_1J",
    "M_2JA",
    "M_2JB",
    "M_SAVE_POS",
    "M_2JC",
    "M_ACCEPT"};

void
REmprint(PTR m, FILE *f)
{
    STATE *p = (STATE *) m;
    const char *end_on_string;

    while (1) {
	if (p->s_type >= END_ON) {
	    p->s_type = (SType) (p->s_type - END_ON);
	    end_on_string = "$";
	} else
	    end_on_string = "";

	if (p->s_type < 0 || p->s_type >= END_ON) {
	    fprintf(f, "unknown STATE type %d\n", (int) (p->s_type));
	    return;
	}

	fprintf(f, "%-10s", xlat[((UChar) (p->s_type)) % U_ON]);
	switch (p->s_type) {
	case M_STR:
	    da_string(f, p->s_data.str, (size_t) p->s_len);
	    break;

	case M_1J:
	case M_2JA:
	case M_2JB:
	case M_2JC:
	    fprintf(f, "%d", p->s_data.jump);
	    break;
	case M_CLASS:
	    {
		UChar *q = (UChar *) p->s_data.bvp;
		UChar *r = q + sizeof(BV);
		while (q < r)
		    fprintf(f, "%2x ", *q++);
	    }
	    break;
	}
	fprintf(f, "%s\n", end_on_string);
	if (end_on_string[0])
	    p->s_type = (SType) (p->s_type + END_ON);
	if (p->s_type == M_ACCEPT)
	    return;
	p++;
    }
}
