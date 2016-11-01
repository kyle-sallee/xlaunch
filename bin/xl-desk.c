/*	By Kyle Sallee in 2015 copyrighted.
	The license file follows.
	/usr/share/doc/pellucid-doc/license.txt
*/

extern	char	*optarg;
extern	int	optind,opterr,optopt;

#define		_GNU_SOURCE
#include	<getopt.h>
#include	<stdio.h>
#include	<stdlib.h>
#include	<unistd.h>
#include	"desk_event.h"
#include	"../lib/exe.h"
#include	"../lib/xopen.h"

int		cdump,cadopt,cflip,creturn;
char		*name;

char	main	(int argc,char *argv[])
{	unsigned long	bp=	0;
	int		oi=	0,
			opt=	0;
	const	struct	option	los[]=
	{	{"adopt",	      no_argument,0,'a'},
		{"color",	required_argument,0,'c'},
		{"dump",	      no_argument,0,'d'},
		{"flip",	      no_argument,0,'f'},
		{"name",	required_argument,0,'n'},
		{"return",	      no_argument,0,'r'},
		{0,0,0,0}
	};
	while			((opt=getopt_long(argc,argv,"ac:dfn:r",los,&oi))!=-1)
	switch			(opt)
	{	case	'n':	name=		optarg;		break;
		case	'a':	cadopt=		1;		break;
		case	'd':	cdump=		1;		break;
		case	'f':	cflip=		1;		break;
		case	'r':	creturn=	1;		break;
		case	'c':	sscanf	(optarg,"%li",&bp);	break;
		default:	goto	help;
	}

	if			(name==NULL)
	goto			help;

	if			(cadopt|cflip)
	if			(forkme())
	return			0;

	DWA			r=
	xopen			(NULL);

	AIM			a;
	if			(cadopt)
	a=	aim		(&r,0);

	DESK			S=
	desk_init		(&r,name);

	if			((S.i.w)&&(S.d.w))
	goto			desk_have;

	if			((cadopt|cflip)==0)
	goto			fini;

	desk_new		(&S,bp);
warnx			("desk=%x\n",S.d.w);
	if			(cadopt)	desk_add	(&S,aim_valid(&a));
	if			(cflip)		desk_flip	(&S);
	desk_event		(&S);
	goto			fini;

	desk_have:
	if			(cdump)		desk_dump	(&S);
	if			(cadopt)	request_add	(&S,aim_valid(&a));
	if			(cflip)		request_flip	(&S);
	if			(creturn)	desk_interact	(&S);

	fini:
	XCloseDisplay		(r.d);
	return			0;

	help:
	errx	(1,
	"Usage xl-desk [PARAMETERS]\n\n"
	"-a, --adopt	to the desk window the beneath window pointer is added\n"
	"-c, --color=	a hexadecimal integer 0xRRGGBB background color is set\n"
	"-d, --dump	to the root window all desk window content is sent\n"
	"-f, --flip	root window content and desk window content are exchanged\n"
	"-n, --name=	the desk name is specified\n"
	"-r, --return	to the root window desk window content is sent.\n"
	"During desk creation only; the background color is set.\n"
	"A desk name is required.\n"
	"For adopt dump flip and return; concurrent specification is allowed.\n"
	"In the following order commands execute: dump; adopt; flip; return.\n"
	"By man 1 xl-desk; return interaction is explained.\n"
	);
}
