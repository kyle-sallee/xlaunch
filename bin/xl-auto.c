/*	By Kyle Sallee in 2015 copyrighted.
	By xl-auto.c the configuration file is read
	and lines that begin with the word auto are executed.
*/

#define		_GNU_SOURCE
#include	<X11/Xlib.h>
#include	<X11/cursorfont.h>
#include	<X11/extensions/dpms.h>
#include	<X11/Xutil.h>
#include	<err.h>
#include	<errno.h>
#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<unistd.h>
#include	<wordexp.h>
#include	"../lib/config_file.h"
#include	"../lib/exe.h"
#include	"../lib/xopen.h"

static	inline	void	tileset	(Display *d,Window w,char *f)
{	unsigned int		hr,wr;
	int			xh,yh;
	Pixmap			bm;

	int			e=
	XReadBitmapFile		(d,w,f,&wr,&hr,&bm,&xh,&yh);
	if			(e!=BitmapSuccess)
	errx			(1,"XReadBitmapFile");

	XGCValues		xgcv=
	{	.background=	0xffffff,
		.foreground=	0
	};

	GC			gc=
	XCreateGC		(d,w,GCBackground|GCForeground,&xgcv);

	Pixmap			pm=
	XCreatePixmap		(d,w,wr,hr,DefaultDepth(d,DefaultScreen(d)));
	XCopyPlane		(d,bm,pm,gc,0,0,wr,hr,0,0,(unsigned long)1);
	XSetWindowBackgroundPixmap
				(d,w,pm);
	XClearWindow		(d,w);
/*	XFreePixmap		(d,bm);
	XFreePixmap		(d,pm);
	XFreeGC			(d,gc);
*/
}

static	inline	void	blanket	(Display *d)
{	XSetScreenSaver	(d,7200,60,DefaultBlanking,DefaultExposures);
	DPMSSetTimeouts	(d,7260,7320,7380);
}

static	inline	void	rootset	(void)
{	DWA		r=
	xopen		(NULL);
	XDefineCursor	(r.d,r.w,XCreateFontCursor(r.d,XC_trek));
	tileset		(r.d,r.w,"/usr/include/X11/bitmaps/escherknot");
	blanket		(r.d);
	XCloseDisplay	(r.d);
}

static	inline	void	line_parse	(wordexp_t *w)
{	if		(2>w->we_wordc)			return;
	char	**c=	w->we_wordv;
	if		(strcasecmp(*c,"auto"))		return;
	c++;
	forkexe		(c);
}

static	inline	void	auto_launch	(void)
{	CONFIG		c=
	config_parse	();
	int		x;
	wordexp_t	*w;
	for		(w=c.word,x=c.lines;x;x--,w++)
	line_parse	(w);
}

char	main		(int argc,char **argv)
{	rootset		();
	auto_launch	();
}
