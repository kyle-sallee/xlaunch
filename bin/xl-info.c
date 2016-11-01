/*	By Kyle Sallee in 2015 copyrighted.
	The license file follows.
	/usr/share/doc/pellucid-doc/license.txt
*/

extern	char	*optarg;
extern	int	optind,opterr,optopt;

#define		_GNU_SOURCE
#include	<X11/Xlib.h>
#include	<getopt.h>
#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<sys/wait.h>
#include	<unistd.h>
#include	"../lib/aim.h"
#include	"../lib/deadline.h"
#include	"../lib/exe.h"
#include	"../lib/xopen.h"

char			*message;
XFontStruct		*xfs;
AIM			targets;
Window			info;
GC			info_gc;

int			line;
int			step;

static	inline	void	over_show	(Display *d,XWindowAttributes *a,char *c)
{	char		*s;
	if		(a->override_redirect==True)
	s=		"override_redirect ";
	else
	s=		"";
	snprintf	(c,0x13,"%-18s",s);
}

static	inline	void	class_show	(Display *d,XWindowAttributes *a,char *c)
{	char		*s;
	if		(a->class==InputOutput)
	s=		"InputOutput ";
	else
	s=		"InputOnly ";
	snprintf	(c,0x0d,"%-12s",s);
}

static	inline	void	focus_show	(Display *d,Window w,char *c)
{	Window		f;
	int		v;
	XGetInputFocus	(d,&f,&v);
	char		*s;
	if		(f==w)
	s=		"focus locked ";
	else
	s=		"";
	snprintf	(c,0x0e,"%-13s",s);
}

static	inline	void	size_show	(Display *d,XWindowAttributes *a,char *c)
{	snprintf(c,0x15,"%4ix%-4i+%4i+%-4i ",a->width,a->height,a->x,a->y);
}

static	inline	void	number_show	(Display *d,Window w,char *c)
{	snprintf(c,0x0a,"%8x ",w);	}

static	inline	void	name_show	(Display *d,Window w,char *c)
{	char		*n;
	XFetchName	(d,w,&n);
	if		(n==NULL)
	n=		"";
	snprintf	(c,0x60," %-95s",n);
/*	if		(n)
	XFree		(n);
*/
}

static	inline	void	command_show	(Display *d,Window w,char *r)
{	*r=			0;
	int			c;
	char			**v;
	if			(XGetCommand(d,w,&v,&c))
	if			(v)
	{	char	**p=	v;
		for		(;c;c--,p++)
		if		(*p)
		{	strncat	(r, *p,0x100-strlen(r));
			strncat	(r," ",0x100-strlen(r));
		}
	/*	XFreeStringList	(v);	*/
	}
}

static	inline	void	splash	(Display *d,Window w)
{	char			*s=
	malloc			(0x1000);
	if			(s==NULL)
	err			(errno,"malloc");

	XWindowAttributes	a;
	XGetWindowAttributes	(d,w,&a);
	char	*p=		s;
	size_show		(d,&a,p);	p+=	0x14;
	number_show		(d,w ,p);	p+=	0x09;
	class_show		(d,&a,p);	p+=	0x0c;
	focus_show		(d,w ,p);	p+=	0x0d;
	over_show		(d,&a,p);	p+=	0x12;
	name_show		(d,w ,p);	p+=	0x5f;
	command_show		(d,w ,p);
	XDrawString		(d,info,info_gc,0,line,s,strlen(s));
	warnx			("%s\n",s);
}

static	inline	void	inform	(Display *d)
{	if		(message==NULL)		return;
	XDrawString	(d,info,info_gc,0,line,message,strlen(message));
	warnx		("%s\n",message);
}

static	void	expose	(Display *d)
{	XMapRaised	(d,info);
	XClearWindow	(d,info);
	DWA		*P;
	for		(P=targets.w;P->w;P++,line+=step)
	splash		(d,P->w);
	inform		(d);
}

static	inline	void	event	(Display *d)
{	XEvent				e;
	do
	switch				(e.type)
	{	case	Expose:	expose	(d);	}
	while				(XNextEvent(d,&e));
}

static	inline	void	initialize	(DWA *r,int up)
{	targets=
	aim			(r,0);
/*	if			(2>targets.t)
	exit			(1);
*/

	xfs=
	XLoadQueryFont		(r->d,"fixed");
	if			(xfs==NULL)
	errx			(1,"XLoadQueryFont");

	XGCValues		xgcv;
	xgcv.foreground=	0xFFFFFF;
	xgcv.background=	0;
	xgcv.font=		xfs->fid;
	line=			xfs->ascent;
	step=			xfs->ascent+xfs->descent;
	int	height=		step*(targets.t+1);

	XWindowAttributes	a;
	XGetWindowAttributes	(r->d,r->w,&a);

	int			y;
	if			(up)
	y=			0;
	else
	y=			a.height-height;

	XSetWindowAttributes	s=	{	.override_redirect=True	};
	info=
	XCreateWindow		(r->d,r->w,0,y,a.width,height,0,
				CopyFromParent,InputOutput,CopyFromParent,
				CWOverrideRedirect,&s);
	XSetWindowBackground	(r->d,info,0);

	info_gc=
	XCreateGC		(r->d,info,\
				 GCForeground|GCBackground|GCFont,\
				 &xgcv);
	expose			(r->d);
}

char	main			(int argc,char *argv[])
{	int			P[2];
	pipe			(P);
	if			(forkme())
	{	close		(P[1]);
		read		(P[0],P,1);
		return		0;
	}

	int	up=		0;
	int	duration=	DELAY;
	int	oi=		0,
		opt=		0;
	const	struct	option	los[]=\
	{	{"down",no_argument,0,'d'},
		{"up",no_argument,0,'u'},
		{"string",required_argument,0,'s'},
		{"timeout",required_argument,0,'t'},
		{0,0,0,0}
	};
	while			((opt=getopt_long(argc,argv,"dus:t:",los,&oi))!=-1)
	switch			(opt)
	{	case	'u':	up=1;					break;
		case	't':	sscanf	(optarg,"%i",&duration);	break;
		case	'd':						break;
		case	's':	message=optarg;				break;
		default:	goto	help;
	}

	DWA			r=
	xopen			(NULL);
	initialize		(&r,up);
	close			(P[0]);
	close			(P[1]);
	deadline_start		(duration,NULL);
	event			(r.d);
	return			0;

	help:
	errx	(1,
	"Usage xl-info [OPTIONS]\n\n"
	"-d, --down	at root window bottom	display\n"
	"-u, --up	at root window top	display\n"
	"-s, --string=				display\n"
	"-t, --timeout=	duration seconds\n"
	"For timeout;	an integer is required.\n"
	"For string;	a  string  is required.\n");
}
