/*	By Kyle Sallee in 2015 copyrighted.
	The license file follows.
	/usr/share/doc/pellucid-doc/license.txt
*/

extern	char	*optarg;
extern	int	optind,opterr,optopt;

#define		_GNU_SOURCE
#include	<X11/Xlib.h>
#include	<fcntl.h>
#include	<getopt.h>
#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<unistd.h>
#include	"../lib/deadline.h"
#include	"../lib/exe.h"
#include	"../lib/newmap.h"
#include	"../lib/reap.h"
#include	"../lib/sack.h"
#include	"sack_event.h"
#include	"../lib/xopen.h"

static	inline	DWA	launch	(DWA *R,char *v[])
{	Display	*d=	R->d;
	Window	r=	R->w;
	XSelectInput	(d,r,SubstructureNotifyMask);
	reap		();
	forkexesafe	(v+optind);
	deadline_start	(60,NULL);
	Window		w=
	newmap		(d);
	deadline_disarm	();
	XSelectInput	(d,r,NoEventMask);
	return
	dwa		(R,w);
}

char		*name;
int		cadopt,cgeo,clower,craise,cdump,cinteract;

char	main			(int argc,char *argv[])
{	int			skip=	0;
	int			oi=	0,
				opt=	0;
	DWA			K;
	const	struct	option	los[]=
	{	{"adopt",	      no_argument,0,'a'},
		{"dump",	      no_argument,0,'d'},
		{"geometry",	required_argument,0,'g'},
		{"interact",	      no_argument,0,'i'},
		{"lower",	      no_argument,0,'l'},
		{"name",	required_argument,0,'n'},
		{"raise",	      no_argument,0,'r'},
		{"skip",	required_argument,0,'s'},
		{0,0,0,0}
	};
	while			((opt=getopt_long(argc,argv,"adg:iln:rs:",los,&oi))!=-1)
	switch			(opt)
	{	case	'a':	cadopt=		1;		break;
		case	'd':	cdump=		1;		break;
		case	'i':	waitpid		(forkexe((char *[]){"xl-info","-s","sack mode",NULL}),NULL,0);
				cinteract=	1;		break;
		case	'l':	clower=		1;		break;
		case	'r':	craise=		1;		break;
		case	'n':	name=		optarg;		break;
		case	's':	sscanf	(optarg,"%i",&skip);	break;
		case	'g':	if	(sscanf(optarg,"%ix%i+%i+%i",&K.a.width,&K.a.height,&K.a.x,&K.a.y)==4)
				cgeo=		1;		break;
		default:	goto	help;
	}

	int			p[2];
	pipe			(p);
	fcntl			(p[0],F_SETFD,FD_CLOEXEC);
	fcntl			(p[1],F_SETFD,FD_CLOEXEC);
	if			(forkme())
	{	close		(p[1]);
		read		(p[0],p,1);
		return		0;
	}

	DWA			R,T;
	R=	T=	xopen	(NULL);

	T.w=			0;
	if			(cadopt)
	T=		launch	(&R,argv);

	if			(cgeo)
	{	T.a.height=	K.a.height;
		T.a.width=	K.a.width;
		T.a.x=		K.a.x;
		T.a.y=		K.a.y;
	}

	SACK			S=
	sack_init		(&R,&T,name,skip);
	if			(clower)	sack_down	(&S);
	if			(craise)	sack_up		(&S);

	if			(cdump)		sack_dump	(&S);
	if			(cinteract)	XMapRaised	(R.d,S.i.w);

	if			(S.made)
	if			(cadopt||cinteract)
	{	close		(p[0]);
		close		(p[1]);
		sack_event	(&S);
	}
	XCloseDisplay		(R.d);
	return			0;
	help:
	errx			(1,
	"Usage xl-sack [PARAMETERS]\n\n"
	"-a, --adopt		a client program starts and window is adopted\n"
	"-d, --dump		to the root window all sack window content is sent\n"
	"-g, --geometry=	from integers XxY+W+H window geometry is set"
	"-i, --interact		sack interaction begins\n"
	"-l, --lower		sack windows cycle\n"
	"-n, --name=		the sack name is set\n"
	"-r, --raise		sack windows cycle\n"
	"-s, --skip=		integer number of windows are ignored\n"
	"During sack creation only; the name and geometry can be set.\n"
	"If not provided then a sack name is discovered or generated.\n"
	"If not provided then for the skip value 0 is assumed.\n"
	"Parameters adopt dump interact lower and raise are mutually exclusive.\n"
	"By man 1 xl-sack; interaction is explained.\n"
	);
}
