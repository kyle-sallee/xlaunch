/*	By Kyle Sallee in 2015 copyrighted.
	By xl-start.c Xorg or suitable X server can be launched.
	The license file follows.
	/usr/share/doc/pellucid-doc/license.txt
*/

#define		_GNU_SOURCE
#include	<X11/Xlib.h>
#include	<err.h>
#include	<errno.h>
#include	<stdio.h>
#include	<stdlib.h>
#include	<sys/types.h>
#include	<sys/stat.h>
#include	<unistd.h>
#include	"../lib/copyright.h"
#include	"../lib/display_env.h"
#include	"../lib/deadline.h"
#include	"../lib/exe.h"

char	d[]=		":0";
char	*xorg[]=	{"Xorg",(char *) &d,"-quiet","-terminate",NULL};
char	lockfile[]=	"/tmp/.X0-lock";

static	void	dead	(int i)
{	errx		(1,"XOpenDisplay");	}

static	inline	void	xwait	(void)
{	deadline_start	(60,dead);
	char		*n=
	display_env	();
	Display		*d;
	while		((d=XOpenDisplay(n))==NULL)
	usleep		(0x10000);
	deadline_disarm	();
}

char	main	(int argc,char **argv)
{	if		(getenv("DISPLAY"))
	goto		xl_grab;

	char	**p;
	for		(p=argv;*p;p++)
	if		(p[0][0]==':')
	{	forkexe	(argv+1);
		setenv	("DISPLAY",*p,1);
		goto	xl_grab;
	}

	int			i;
	for			(i=10;i;i--)
	{	struct	stat	b;
		if		(stat(lockfile,&b))
		goto		start;
		lockfile[7]++;
		d[1]++;
	}
	errx			(1,"10 /tmp/.X?-lock files exist?\n");

	start:
	setenv			("DISPLAY",d,1);
	forkexe			(xorg);
	xwait			();
	xl_grab:
	forkexe			((char *[]){"xl-auto",NULL});
	exe			((char *[]){"xl-grab",NULL});
}
