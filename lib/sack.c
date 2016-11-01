/*	By Kyle Sallee in 2015 copyrighted.	*/

#include	"sack.h"
char	xl_sack		(DWA const * const w)
{	if		(w->w==None)
	goto		not;
	char		*n=	NULL;
	XFetchName	(w->d,w->w,&n);
	if		(n==NULL)
	goto		not;
	char		r=
	strncmp		(n,"xl sack ",8);
	XFree		(n);
	return		r;
	not:
	return		1;
}
