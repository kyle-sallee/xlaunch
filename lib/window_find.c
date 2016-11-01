/*	By Kyle Sallee in 2015 copyrighted.	*/

#include	"window_find.h"
Window	window_find	(Display * const d,Window const r,char const * const c)
{	Window		*t;
	unsigned int	x=
	xtree		(d,r,&t);
	Window	f=	None;
	Window	*p=	t;
	for		(;x;x--,p++)
	if		(window_namecmp(d,*p,c)==0)
	{	f=	*p;
		break;
	}
	XFree		(t);
	return		f;
}
