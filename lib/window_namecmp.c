/*	By Kyle Sallee in 2015 copyrighted.	*/

#include	"window_namecmp.h"
char	window_namecmp		(Display * const d,Window const w,char const * const m)
{	if		(w==None)
	goto		not;

	char		*n;
	if		(XFetchName(d,w,&n)==0)
	goto		not;

	if		(n)
	{	char	r=
		strcmp	(n,m);
		XFree	(n);
		return	r;
	}

	not:
	return		1;
}
