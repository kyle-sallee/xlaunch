/*	By Kyle Sallee in 2015 copyrighted.
	The license file follows.
	/usr/share/doc/pellucid-doc/license.txt
*/

extern	char	*optarg;
extern	int	optind,opterr,optopt;

#define		_GNU_SOURCE
#include	<X11/Xlib.h>
#include	<X11/extensions/xf86vmode.h>
#include	<getopt.h>
#include	<stdio.h>
#include	<stdlib.h>
#include	<sys/stat.h>
#include	<sys/types.h>
#include	<unistd.h>
#include	<wordexp.h>
#include	"../lib/xopen.h"

static	void	expose		(Display *d,Window w,GC g,XWindowAttributes *a)
{	XClearWindow		(d,w);
	XGetWindowAttributes	(d,w,a);

	int	step_x=		a->width /6;
	int	step_y=		a->height/6;

	unsigned long		i;
	int			t,y;
	for			(y=step_y,t=a->height-y;t>y;y++)
	{	i=		0xff   *y/t;
		XSetForeground	(d,g,i+i*0x100+i*0x10000);	XDrawLine	(d,w,g,step_x  ,y,step_x*2,y);
		XSetForeground	(d,g,i);			XDrawLine	(d,w,g,step_x*2,y,step_x*3,y);
		XSetForeground	(d,g,i*0x100);			XDrawLine	(d,w,g,step_x*3,y,step_x*4,y);
		XSetForeground	(d,g,i*0x10000);		XDrawLine	(d,w,g,step_x*4,y,step_x*5,y);
	}
}

static	inline	float	gama	(Display *d,Window w,XWindowAttributes *a)
{	Window		z;
	int		r,y;
	XQueryPointer	(d,w,&z,&z,&y,&y,&y,&r,&y);

	float	f=	(float)r;
	return		0.5+(2*f)/a->height;
}

static	inline	void	calibrate	(DWA *R,int s,XF86VidModeGamma *G,char *n)
{	Display		*d=	R->d;
	XSetWindowAttributes	x=
	{	.background_pixel=	0xffffff,
		.event_mask=		ButtonPressMask|ExposureMask|
					PointerMotionMask
	};

	Window			w=
	XCreateWindow		(d,R->w,0,0,R->a.width,R->a.height,0,
				CopyFromParent,InputOutput,CopyFromParent,
				CWBackPixel|CWEventMask,&x);
	XMapRaised		(d,w);
	XGCValues		v=
	{	.background=	0xffffff,
		.foreground=	0
	};
	GC			g=
	XCreateGC		(d,w,GCForeground|GCBackground,&v);

	XWindowAttributes	a;
	expose			(d,w,g,&a);

	again:;
	XEvent			e;
	XSync			(d,True);
	XNextEvent		(d,&e);
	switch			(e.type)
	{	case	ButtonPress:	goto	complete;
		case	Expose:		expose	(d,w,g,&a);
		case	MotionNotify:	G->red=
					G->green=
					G->blue=
			gama			(d,w,&a);
			XF86VidModeSetGamma	(d,s,G);
		break;
	}
	goto			again;

	complete:;
	FILE	*f=	fopen	(n,"w");
	fprintf			(f,"%4.2f	%4.2f	%4.2f\n",G->red,G->green,G->blue);
	fclose			(f);
}

static	inline	void	restore	(Display *d,int s,XF86VidModeGamma *G,char *n)
{	FILE	*f=	fopen	(n,"r");
	if			(f==NULL)	return;
	fscanf			(f,"%f%f%f",&G->red,&G->green,&G->blue);
}

char	main			(int argc,char *argv[])
{	wordexp_t		D,F;
	wordexp			("~/.config/xl-gamma",	&F,WRDE_NOCMD);
	wordexp			("~/.config/",		&D,WRDE_NOCMD);
	mkdir			(D.we_wordv[0],0700);

	DWA			R=
	xopen			(NULL);
	int			s=
	DefaultScreen		(R.d);
	XF86VidModeGamma	G;
	XF86VidModeGetGamma	(R.d,s,&G);

	float		f;
	int		oi=	0,
			opt=	0;
	const	struct	option	los[]=
	{	{"accretion",	required_argument,0,'a'},
		{"calibrate",	      no_argument,0,'c'},
		{"query",	      no_argument,0,'q'},
		{"restore",	      no_argument,0,'r'},
		{"set",		required_argument,0,'s'},
		{0,0,0,0}
	};
	while			((opt=getopt_long(argc,argv,"a:cqrs:",los,&oi))!=-1)
	switch			(opt)
	{	case	'a':	if		(sscanf(optarg,"%f",&f)==1)
				{	G.red+=		f;
					G.green+=	f;
					G.blue+=	f;
				}
				else
				goto		help;		break;
		case	'c':	calibrate	(&R,s,&G,F.we_wordv[0]);
								break;
		case	'q':	printf		("%4.2f	%4.2f	%4.2f\n",G.red,G.green,G.blue);
								break;
		case	'r':	restore		(R.d,s,&G,F.we_wordv[0]);
								break;
		case	's':	if		(sscanf(optarg,"%f%f%f",&G.red,&G.green,&G.blue)!=3)
				goto		help;		break;
		default:	goto		help;
	}

	XF86VidModeSetGamma	(R.d,s,&G);
	XCloseDisplay		(R.d);
	return			0;

	help:
	errx	(1,
	"Usage xl-gamma [PARAMETERS]\n\n"
	"-a, --accretion=	red green and blue float gamma values increment\n"
	"-c, --calibrate		to set and save where the black begins click\n"
	"-q, --query		red green and blue float gamma values print\n"
	"-r, --restore		previous calibrated gamma is restored\n"
	"-s, --set=		red green and blue float gamma values set\n"
	);
}
