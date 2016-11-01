/*	By Kyle Sallee in 2015 copyrighted.	*/
/*
ButtonPressMask|ButtonReleaseMask|KeyPressMask|KeyReleaseMask|PointerMotionMask
*/

#include	"overlay.h"
DWA	overlay			(DWA const * const D,Cursor c,long const m,char const * const a)
{	XSetWindowAttributes	s={ .event_mask=m, .override_redirect=True };
	DWA			N;
	N=			*D;
	N.a.root=		D->w;
	N.a.override_redirect=	True;
	N.w=
	XCreateWindow		(D->d,D->w,D->a.x,D->a.y,D->a.width,D->a.height,0,CopyFromParent,InputOnly,CopyFromParent,CWEventMask|CWOverrideRedirect,&s);
	XDefineCursor		(D->d,N.w,XCreateFontCursor(D->d,c));
	XStoreName		(D->d,N.w,a);
	XFlush			(D->d);
	return			N;
}
