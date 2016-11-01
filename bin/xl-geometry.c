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
#include	<unistd.h>
#include	"../lib/aim.h"
#include	"../lib/deadline.h"
#include	"../lib/exe.h"
#include	"../lib/newmap.h"
#include	"../lib/reap.h"
#include	"../lib/size.h"
#include	"../lib/warp.h"
#include	"../lib/xopen.h"

char	main			(int argc,char *argv[])
{	int			oi=			0,
				opt=			0;
	int			skip=			0;

	const	struct	option	los[]=
	{	{"geometry",	required_argument,0,'g'},
		{"modify",	      no_argument,0,'m'},
		{"skip",	required_argument,0,'s'},
		{0,0,0,0}
	};
	PAIR			p=	{	.x=-1,	.y=0	};
	int			w=0,h=0,c=0;
	char			m=0;
	while			((opt=getopt_long(argc,argv,"g:ms:",los,&oi))!=-1)
	switch			(opt)
	{	case	'g':
		if	((c=sscanf(optarg,"%dx%d+%d+%d",&w,&h,&p.x,&p.y))>1)
		break;
		goto	help;
		case	'm':	m++;
		break;
		case	's':
		sscanf	(optarg,"%i",&skip);
		break;
		default:
		goto	help;
	}

	DWA			r=
	xopen			(NULL);

	XSelectInput		(r.d,r.w,SubstructureNotifyMask);

	Window			n;
	AIM			a;
	if			(argc>optind)
	{	reap		();
		forkexesafe	(argv+optind);
		deadline_start	(60,NULL);
		n=
		newmap		(r.d);
	}
	else
	{	a=
		aim		(&r,skip);
		n=
		aim_valid	(&a);
	}

	switch			(c)
	{	case	0:
		p=
		pointer_at	(r.d,r.w);
		case	4:
		if		(p.x!=-1)
		XMoveWindow	(r.d,n,p.x,p.y);
	}

	if			(c>1)
	if			(w>=0x20)
	if			(h>=0x20)
	size_set		(&a,w,h);
	if			(m==0)
	{	XCloseDisplay	(r.d);
		return		0;
	}

	p=
	pointer_at		(r.d,r.w);
	XMoveWindow		(r.d,n,p.x,p.y);
	XCloseDisplay		(r.d);
	exe			((char *[]){"xl-mod",NULL});
	/*	exe does not return.	*/

	help:
	errx	(1,
	"Usage xl-geometry [PARAMETERS] -- filename parameters\n\n"
	"-g, --geometry=	to XxY+W+H the window is set.\n"
	"-s,  --skip=	to omit window manager provided windows.\n\n"
	"If a file name is specified then the file name is executed.\n"
	"For the next window map the geometry is changed.\n"
	"If a file name is not specified\n"
	"then the beneath pointer window geometry is changed."
	);
}
