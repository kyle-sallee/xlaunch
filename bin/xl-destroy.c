/*	By Kyle Sallee in 2015 copyrighted.
	The license file follows.
	/usr/share/doc/pellucid-doc/license.txt
*/

extern	char	*optarg;
extern	int	optind,opterr,optopt;

#define		_GNU_SOURCE
#include	<getopt.h>
#include	<X11/Xlib.h>
#include	<string.h>
#include	<stdlib.h>
#include	"../lib/aim.h"
#include	"../lib/xopen.h"

static	inline	void	xdestroy	(Display *d,Window w)
{	Atom				e,*p,s;
	XEvent				v;
	int				c;

	e=	XInternAtom		(d,"WM_DELETE_WINDOW",False);
	s=	XInternAtom		(d,"WM_PROTOCOLS",    False);

	if				(XGetWMProtocols(d,w,&p,&c))
	for				(c--;c>=0;c--)
	if				(p[c]==e)
	{	v.type=			ClientMessage;
		v.xclient.window=	w;
		v.xclient.message_type=	s;
		v.xclient.format=	32;
		v.xclient.data.l[0]=	e;
		v.xclient.data.l[1]=	CurrentTime;
		XSendEvent		(d,w,False,NoEventMask,&v);
		XFree			(p);
		return;
	}
	XFree				(p);
	XKillClient			(d,w);
}

char	main			(int argc,char *argv[])
{	DWA			r=
	xopen			(NULL);
	AIM			a=
	aim			(&r,0);
	if			(2>a.t)
	errx			(1,"An in window pointer is required.");

	int			oi=				0,
				opt=				0;
	const	struct	option	los[]=
	{	{"destroy",	      no_argument,0,'d'},
		{"kill",	      no_argument,0,'k'},
		{0,0,0,0}
	};
	while			((opt=getopt_long(argc,argv,"ac:dfn:rs:",los,&oi))!=-1)
	switch			(opt)
	{	case	'd':	xdestroy	(r.d,a.w[a.t-1].w);	break;
		case	'k':	XKillClient	(r.d,a.w[a.t-1].w);	break;
		default:	goto	help;
	}
	XCloseDisplay		(r.d);
	return			0;
	help:
	errx	(1,
	"Usage xl-destroy [PARAMETERS]\n\n"
	"-d, --destroy	the pointer containing window         is destroyed.\n"
	"-k, --kill	the pointer containing window process is killed.\n\n"
	"Because by killing the process also terminates\n"
	"more often than not destroying a window only is desired.\n"
	"If the window can not be destroyed\n"
	"then XKillClient is invoked.\n"
	);
}
