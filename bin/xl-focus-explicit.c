/*	By Kyle Sallee in 2015 copyrighted.
	The license file follows.
	/usr/share/doc/pellucid-doc/license.txt
*/

#define		_GNU_SOURCE
#include	<X11/Xlib.h>
#include	<stdio.h>
#include	<stdlib.h>
#include	<unistd.h>
#include	"../lib/xopen.h"
#define		TransitMask	EnterWindowMask|LeaveWindowMask
#define		SnoopMask	TransitMask|SubstructureNotifyMask

static	inline	void	event	(Display *d)
{	again:;
	XEvent		e;
	XNextEvent	(d,&e);
	switch		(e.type)
	{	case	EnterNotify:
			XSetInputFocus	(d,e.xcrossing.window,RevertToPointerRoot,CurrentTime);
			break;
		case	LeaveNotify:
			XSetInputFocus	(d,PointerRoot,None,CurrentTime);
			break;
		case	MapNotify:
			XSelectInput	(d,e.xmap.window,TransitMask);
	}
	goto		again;
}

/*	Focus locking inner     windows creates problems
	Focus locking container windows suffices
*/
static	inline	void	snoop	(Display *d,Window r)
{	Window				rr,pr,*cr,*cp;
	unsigned int			c;
	XQueryTree			(d,r,&rr,&pr,&cr,&c);
	if				(c)
	{	for			(cp=cr;c;c--,cp++)
		{	XSelectInput	(d,*cp,SnoopMask);
		}
		XFree		(cr);
	}
}

char	main		(int argc,char *argv[])
{	DWA		r=
	xopen		(NULL);
	XSelectInput	(r.d,r.w,SnoopMask);
	snoop		(r.d,r.w);
	event		(r.d);
}
