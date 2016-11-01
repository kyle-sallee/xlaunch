/*	By Kyle Sallee in 2015 copyrighted.
	The license file follows.
	/usr/share/doc/pellucid-doc/license.txt
*/

extern	char	*optarg;
extern	int	optind,opterr,optopt;

#define		_GNU_SOURCE
#include	<X11/Xlib.h>
#include	<X11/XKBlib.h>
#include	<X11/cursorfont.h>
#include	<X11/keysym.h>
#include	<getopt.h>
#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<sys/wait.h>
#include	<unistd.h>
#include	"aim_overlay.h"
#include	"../lib/deadline.h"
#include	"../lib/exe.h"
#include	"../lib/overlay.h"
#include	"../lib/size.h"
#include	"../lib/warp.h"
#include	"../lib/xopen.h"

const	PAIR	cycle[]=\
{	{64,64},	{320,200},	{320,240},	{640,480},
	{800,600},	{1280,720},	{1024,768},	{1600,900},
	{1280,1024},	{1920,1080},	{2048,1152},	{2560,1440},
	{3200,1800},	{3840,2160},	{4096,2304},	{5120,2880},
	{7680,4320},	{8192,4608},	{0,0}
};

static	inline	void	size_cycle	(AIM *A)
{	XWindowAttributes	*X=&A->w[A->s+1].a;
	XWindowAttributes	*x=&A->w[0].a;
	PAIR	const	*p=	cycle;

	if			((x->height>X->height)&&(x->width>X->width))
	for			(;(p->y)&&(X->height>=p->y);p++);

	int	w=		p->x;
	int	h=		p->y;

	if			((h==0)||(h>x->height)||(w>x->width))
	{	w=		x->width;
		h=		x->height;
	}
	size			(A,w-X->width,h-X->height);
}

static	inline	int	override_toggle	(Display *d,Window w)
{	XWindowAttributes	g;
	XSetWindowAttributes	s;

	XGetWindowAttributes	(d,w,&g);
	if			(g.override_redirect==True)
	s.override_redirect=	False;
	else
	s.override_redirect=	True;
	XChangeWindowAttributes	(d,w,CWOverrideRedirect,&s);
	XCloseDisplay		(d);
	return			0;
}

static	inline	int	near	(int i,int s)
{	return	(i/s)*s;
}

static	inline	void	revert	(AIM *a)
{	DWA			*P;
	for			(P=a->w+1;P->w;P++)
	{	XMoveWindow	(P->d,P->w,P->a.x,    P->a.y);
		XResizeWindow	(P->d,P->w,P->a.width,P->a.height);
	}
}

static	void	next		(MOD *S)
{	Display		*d=	S->a.w[0].d;
	PAIR			T=
	pointer_at		(d,S->a.w[0].w);
	int			c;
	switch			(S->mode)
	{	case	0:
		S->B.x=		T.x-S->A.x;
		S->B.y=		T.y-S->A.y;	/* move offset saved */
		S->A.x=		T.x-S->C.x;
		S->A.y=		T.y-S->C.y;	/* size anchor set */
		S->mode++;
		c=		XC_sizing;
		break;
		case	1:
		S->C.x=		T.x-S->A.x;
		S->C.y=		T.y-S->A.y;	/* size offset saved */
		S->mode++;
		c=		XC_watch;
		break;
		case	2:
		S->A.x=		T.x-S->B.x;
		S->A.y=		T.y-S->B.y;	/* move anchor set */
		S->mode=0;
		c=		XC_tcross;
		break;
	}
	XDefineCursor		(d,S->i.w,XCreateFontCursor(d,c));
}

static	inline	void	event	(MOD *S)
{	Display	*d=		S->a.w[0].d;
	DWA	R=		S->i;
	R.w=			S->i.a.root;

	restart:;
	Window			 w=	 S->a.w[S->a.s+1].w;
	XWindowAttributes	*x=	&S->a.w[S->a.s+1].a;

	S->i=
	overlay			(S->a.w,XC_tcross,
				ButtonPressMask|KeyPressMask|PointerMotionMask,
				"xl input move");
	XMapRaised		(d,S->i.w);

	reversion:
	S->A=
	pointer_at		(d,S->i.w);
	S->B.x=
	S->B.y=
	S->C.x=
	S->C.y=			0;

	again:;
	XEvent			e;

	XSync			(d,True);
	deadline_arm		();
	XNextEvent		(d,&e);
	switch			(e.type)
	{	case	MotionNotify:
			if		(S->mode==0)
			XMoveWindow	(d,
					 w,
			near		( x->x+e.xmotion.x_root-S->A.x,S->modulo),
			near		( x->y+e.xmotion.y_root-S->A.y,S->modulo));
			else if		(S->mode==1)
			size		(&S->a,e.xmotion.x_root-S->A.x,e.xmotion.y_root-S->A.y);
		break;
		case	ButtonPress:
		switch	(e.xbutton.button)
		{	case	1:				return;
			case	3:	next		(S);	break;
			case	4:	XRaiseWindow	(d,w);	break;
			case	5:	XLowerWindow	(d,w);
		}
		break;
		case	KeyPress:
		switch	(XkbKeycodeToKeysym(d,e.xkey.keycode,0,0))
		{	case	XK_space:	next	(S);	break;
			case	XK_a:
				XDestroyWindow		(d,S->i.w);
				deadline_disarm		();
				S->a=	aim_overlay	(&R);
				goto		restart;
			case	XK_1:		S->modulo=1;	break;
			case	XK_2:		S->modulo=2;	break;
			case	XK_4:		S->modulo=4;	break;
			case	XK_8:		S->modulo=8;	break;
			case	XK_F1:		S->a.s=	0;	break;
			case	XK_F2:		S->a.s=	1;	break;
			case	XK_F3:		S->a.s=	2;	break;
			case	XK_F4:		S->a.s=	3;	break;
			case	XK_BackSpace:	revert		(&S->a);
						goto		reversion;
			case	XK_Return:			return;
		}
	}
	goto			again;
}

void	discover		(DWA *r)
{	Window			*p;
	int			t=
	xtree			(r->d,r->w,&p);
	for			(;t;t--)
	{	DWA		d=
		dwa		(r,*p++);
		if		((0x20>d.a.width)||
				 (0x20>d.a.height))
		XResizeWindow	(d.d,d.w,0x20,0x20);
		if		((0>d.a.x+d.a.width )||
				 (0>d.a.y+d.a.height)||
				 (d.a.x>r->a.width  )||
				 (d.a.y>r->a.height ))
		XMoveWindow	(d.d,d.w,0,0);
	}
}

char	main	(int argc,char *argv[])
{	XSetWindowAttributes	a=	{	.border_pixel=	-1	};
	int			bw=		-1;
	MOD			s=	{	.a.s=		0,
						.mode=		0,
						.modulo=	4	};
	int			duration=	DELAY;
	int			skip=		0;
	int			only=		0;
	int			oi=		0,
				opt=		0;
	const	struct	option	los[]=\
	{	{"color",	required_argument,0,'c'},
		{"lower",	      no_argument,0,'l'},
		{"modulo",	required_argument,0,'m'},
		{"override",	      no_argument,0,'o'},
		{"prog",	      no_argument,0,'p'},
		{"raise",	      no_argument,0,'r'},
		{"skip",	required_argument,0,'s'},
		{"time",	required_argument,0,'t'},
		{"width",	required_argument,0,'w'},
		{0,0,0,0}
	};
	while			((opt=getopt_long(argc,argv,"c:lm:prs:t:w:",los,&oi))!=-1)
	switch			(opt)
	{	case	'c':	sscanf	(optarg,"%lx",&a.border_pixel);	break;
		case	'l':	only=	1;				break;
		case	'r':	only=	2;				break;
		case	'o':	only=	3;				break;
		case	'p':	only=	4;				break;
		case	'm':	sscanf	(optarg,"%i",&s.modulo);	break;
		case	's':	sscanf	(optarg,"%i",&skip);		break;
		case	't':	sscanf	(optarg,"%i",&duration);	break;
		case	'w':	sscanf	(optarg,"%i",&bw);		break;
		default:	goto	help;
	}

	deadline_start		(duration,NULL);
	DWA			r=
	xopen			(NULL);
	s.i=
	dwa			(&r,None);
	s.a=
	aim			(&r,skip);

	Window			w=
	aim_valid		(&s.a);
	if			(a.border_pixel!=-1)
	XChangeWindowAttributes	(r.d,w,CWBorderPixel,&a);
	if			(bw!=-1)
	XSetWindowBorderWidth	(r.d,w,bw);
	waitpid			(forkexe((char *[]){"xl-info","-s","mod mode",NULL}),NULL,0);
	if			(only)
	switch			(only)
	{	case	1:	XLowerWindow	(r.d,w);	break;
		case	2:	XRaiseWindow	(r.d,w);	break;
		case	3:	override_toggle	(r.d,w);	break;
		case	4:	size_cycle	(&s.a);		break;
	}
	else
	event			(&s);
	discover		(&r);
	XCloseDisplay		(r.d);
	return			0;

	help:
	errx	(1,
	"Usage xl-mod [PARAMETERS]\n\n"
	"-c, --color=   border color\n"
	"-l, --lower	window  lowers\n"
	"-m, --modulo=	integer	pixels\n"
	"-p, --prog	progressive resolution\n"
	"-r, --raise	window  raises\n"
	"-s, --skip=	integer	window\n"
	"-t, --time=	integer	seconds duration\n"
	"-w, --width=	border width\n\n"
	"By man 1 xl-mod; interaction is explained\n"
	);
}
