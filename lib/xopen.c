/*	By Kyle Sallee in 2015 copyrighted.	*/

#include	"xopen.h"

static	inline	int	xerr	(Display *d,XErrorEvent *e)
{	char		b[0x40];
	XGetErrorText	(d,e->error_code,b,0x40);
	warnx		(b);
	return		0;
}

DWA	xopen				(int (*f)(Display *,XErrorEvent *))
{	if				(f==NULL)
	f=				xerr;
	XSetErrorHandler		(f);

	DWA				r;
	r.d=	XOpenDisplay		(display_env());
	if				(r.d)
	{	r.w=
		DefaultRootWindow	(r.d);
		XGetWindowAttributes	(r.d,r.w,&r.a);
		return			r;
	}
	errx				(1,"XOpenDisplay\n");
}
