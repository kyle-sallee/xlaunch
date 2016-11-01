/*	By Kyle Sallee in 2015 copyrighted.
	xl-sire.c is an X11 hot key program.
	The license file follows.
	/usr/share/doc/pellucid-doc/license.txt
*/

#define		_GNU_SOURCE
#include	<X11/Xlib.h>
#include	<X11/cursorfont.h>
#include	<X11/keysym.h>
#include	<X11/XKBlib.h>
#include	<error.h>
#include	<sched.h>
#include	<signal.h>
#include	<sys/signalfd.h>
#include	<sys/stat.h>
#include	<sys/types.h>
#include	<sys/wait.h>
#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<unistd.h>
#include	<wordexp.h>
#include	"../lib/config_file.h"
#include	"../lib/deadline.h"
#include	"../lib/exe.h"
#include	"../lib/mod_parse.h"
#include	"../lib/overlay.h"
#include	"../lib/var.h"
#include	"../lib/window_find.h"
#include	"../lib/xopen.h"

static	void	dead	(int i)
{	forkexe	((char *[]){"xl-term","-e","less","/etc/xl",NULL});
	exit	(1);
}

static	inline	int	line_parse	(wordexp_t *w,KEY *k,DWA *i)
{	if		(3>w->we_wordc)			return	0;
	char	**c=	w->we_wordv;
	if		(strcasecmp(*c,"sire"))		return	0;
	c++;
	if		(strcmp(*c,k->string))		return	0;
	unsigned int	m,n;
	for		(m=0,c++;*c;c++,m|=n)
	if		((n=mod_parse(*c))==0)		break;
	if		(m!=k->state)			return	0;
	XUnmapWindow	(i->d,i->w);
	XFlush		(i->d);
	forkexe		(c);
	return		1;
}

static	inline	int	key_press	(CONFIG *c,KEY *k,DWA *i)
{	k->string=
	XKeysymToString		(XkbKeycodeToKeysym(i->d,k->keycode,0,0));
	int			r,x;
	wordexp_t		*w;
	for			(r=0,w=c->word,x=c->lines;x;x--,w++)
	r|=	line_parse	(w,k,i);
	return			r;
}

static	int	xerr	(Display *d,XErrorEvent *e)
{	char		b[0x20];
	XGetErrorText	(d,e->error_code,b,0x20);
	errx		(1,b);
}

static	inline	int	event	(void)
{	CONFIG		c=
	config_parse	();
	DWA		r=
	xopen		(xerr);

	if		(window_find(r.d,r.w,"xl input sire")!=None)
	return		1;

	DWA		i=
	overlay		(&r,XC_hand2,KeyPressMask,"xl input sire");
	XMapRaised	(r.d,i.w);

	again:;
	XEvent		e;
	XNextEvent	(r.d,&e);
	switch		(e.type)
	{	case	KeyPress:
		if	(key_press(&c,&((KEY){e.xkey.state,e.xkey.keycode}),&i))
		return	0;
	}
	goto		again;
}

char	main	(int argc,char **argv)
{	signal		(SIGCHLD,SIG_IGN);
	deadline_start	(DELAY,dead);
	return
	event		();
}
