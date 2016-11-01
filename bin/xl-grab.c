/*	By Kyle Sallee in 2015 copyrighted.
	xl-grab.c is an X11 hot key program.
	The license file follows.
	/usr/share/doc/pellucid-doc/license.txt
*/

#define		_GNU_SOURCE
#include	<X11/Xlib.h>
#include	<X11/keysym.h>
#include	<X11/XKBlib.h>
#include	<sched.h>
#include	<signal.h>
#include	<sys/signalfd.h>
#include	<sys/types.h>
#include	<sys/wait.h>
#include	<wordexp.h>
#include	"../lib/config_file.h"
#include	"../lib/display_env.h"
#include	"../lib/exe.h"
#include	"../lib/mod_parse.h"
#include	"../lib/var.h"
#include	"../lib/xopen.h"

static	inline	void	line_parse	(wordexp_t *w,Display *d,Window r)
{	if		(2>w->we_wordc)
	goto		parsed;
	if		(strcasecmp(*w->we_wordv,"grab"))
	goto		parsed;
	char		**c=	w->we_wordv+1;
	KeyCode		k=
	XKeysymToKeycode(d,XStringToKeysym(*c));
	int		m,n;
	for		(m=0,c++;*c;c++,m|=n)
	if		((n=mod_parse(*c))==0)	break;
	XGrabKey	(d,k,m,r,False,GrabModeAsync,GrabModeAsync);
	parsed:
	if		(w->we_wordv)
	wordfree	(w);
}

static	inline	void	keysym_parse	(Display *d,Window r)
{	CONFIG		c=
	config_parse	();
	wordexp_t	*w;
	int		x;
	for		(w=c.word,x=c.lines;x;x--,w++)
	line_parse	(w,d,r);
	free		(c.word);
}

static	inline	char	event_loop	(Display *d,Window r)
{	again:;
	XEvent		e;
	XNextEvent	(d,&e);
	if		(e.type==KeyPress)
	forkexe		((char *[]){"xl-sire",NULL});
	goto		again;
}

char	main		(int argc,char **argv)
{	signal		(SIGCHLD,SIG_IGN);
	reap		();
	DWA		R=
	xopen		(NULL);
	keysym_parse	(R.d,R.w);
	event_loop	(R.d,R.w);
}
