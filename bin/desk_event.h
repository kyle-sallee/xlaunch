/*	By Kyle Sallee in 2015 copyrighted.	*/

#ifndef		_DESK_EVENT_H
#define		_DESK_EVENT_H
#include	<X11/Xlib.h>
#include	<X11/cursorfont.h>
#include	<err.h>
#include	<stdio.h>
#include	"../lib/aim.h"
#include	"../lib/window_find.h"

static	inline	DESK	desk_init	(DWA *R,char *c)
{	if			(c==NULL)
	errx			(1,"A desk name is required.");
	DESK			S;
	snprintf		(S.nd,0x30,"xl desk %s",c);
	snprintf		(S.ni,0x30,"xl input desk %s",c);
	S.d=	S.i=	S.r=	*R;
	S.d.w=	window_find	(R->d,R->w,S.nd);
	S.i.w=	window_find	(R->d,R->w,S.ni);
	return			S;
}

static	inline	void	desk_new	(DESK *S,unsigned long bp)
{	XSetWindowAttributes		a;
	a.background_pixel=		bp;
	a.override_redirect=		True;
	a.event_mask=			ButtonPressMask|KeyPressMask;
	S->i.w=
	XCreateWindow			(S->r.d,S->r.w,S->r.a.x,S->r.a.y,S->r.a.width,S->r.a.height,0,
					CopyFromParent,InputOnly,CopyFromParent,
					CWEventMask|CWOverrideRedirect,&a);
	XStoreName			(S->i.d,S->i.w,S->ni);
	XDefineCursor			(S->i.d,S->i.w,XCreateFontCursor(S->i.d,XC_spraycan));

	a.event_mask=			SubstructureNotifyMask;
	S->d.w=
	XCreateWindow			(S->r.d,S->r.w,S->r.a.x,S->r.a.y,S->r.a.width,S->r.a.height,0,
					CopyFromParent,InputOutput,CopyFromParent,
					CWBackPixel|CWEventMask|CWOverrideRedirect,&a);
	XStoreName			(S->r.d,S->d.w,S->nd);
}

static	void	desk_add	(DESK *S,Window w)
{	XWindowAttributes	a;
	XGetWindowAttributes	(S->r.d,w,&a);
	if			(a.override_redirect==False)
	{	XAddToSaveSet	(S->r.d,w);
		XReparentWindow	(S->r.d,w,S->d.w,a.x,a.y);
	}
}

static	void	desk_drop	(DESK *S,Window w)
{	XWindowAttributes	a;
	XGetWindowAttributes	(S->r.d,w,&a);
	XReparentWindow		(S->r.d,w,S->r.w,a.x,a.y);
}

static	inline	void	desk_dump	(DESK *S)
{	Window		*p;
	unsigned int	x=
	xtree		(S->d.d,S->d.w,&p);
	for		(;x;x--,p++)
	desk_drop	(S,*p);
}

void	desk_flip	(DESK *S)
{	Window		*desk_p;
	unsigned int	desk_x=
	xtree		(S->d.d,S->d.w,&desk_p);

	Window		*root_p;
	unsigned int	root_x=
	xtree		(S->r.d,S->r.w,&root_p);

	Window		*p;
	for		(p=root_p;root_x;root_x--,p++)
	desk_add	(S,*p);
	for		(p=desk_p;desk_x;desk_x--,p++)
	desk_drop	(S,*p);
	XFree		(root_p);
	XFree		(desk_p);
}

static	inline	void	desk_ignore	(DESK *S)
{	XUnmapWindow	(S->d.d,S->d.w);
	XUnmapWindow	(S->i.d,S->i.w);
}

static	inline	void	desk_interact	(DESK *S)
{	XMapRaised	(S->d.d,S->d.w);
	XMapRaised	(S->i.d,S->i.w);
}

void	desk_raise	(DESK *S)
{	XRaiseWindow	(S->d.d,S->d.w);
	XRaiseWindow	(S->i.d,S->i.w);
}

static	inline	Window	aimed	(DESK *S)
{	XRaiseWindow		(S->r.d,S->d.w);
	Window	w=	aim1	(S->r.d,S->d.w);
	desk_raise		(S);
	return			w;
}

static	inline	void	desk_event	(DESK *S)
{	do
	{	resume:
		desk_raise			(S);
		XEvent				e;
		XNextEvent			(S->r.d,&e);
		switch				(e.type)
		{	case	CirculateNotify:
			case	ConfigureNotify:
			case	CreateNotify:
			case	GravityNotify:
			case	MapNotify:
			case	UnmapNotify:
			goto	resume;
			case	ButtonPress:
			switch			(e.xbutton.button)
			{	case	3:
				goto		ignore;
				case	0xfe:
				desk_flip	(S);
				goto		resume;
				case	0xff:
				desk_add	(S,e.xbutton.window);
				goto		resume;
			}
			Window			a;

			if			((a=aimed(S))==None)
			{	desk_dump	(S);
				return;
			}

			switch			(e.xbutton.button)
			{	case	1:
				desk_drop	(S,a);
				desk_raise	(S);
				break;
				case	4:
				XRaiseWindow	(S->r.d,a);
				break;
				default:
				XLowerWindow	(S->r.d,a);
			}
			break;
			case	KeyPress:
			ignore:
			desk_ignore		(S);
		}
	}
	while					(xtreec(S->r.d,S->d.w));
}

static	inline	void	request_add	(DESK *S,Window w)
{	XEvent			v=
	{	.xbutton.type=	ButtonPress,
		.xbutton.button=0xff,
		.xbutton.window=w
	};
	XSendEvent		(S->i.d,S->i.w,False,ButtonPressMask,&v);
}

static	inline	void	request_flip	(DESK *S)
{	XEvent			v=
	{	.xbutton.type=	ButtonPress,
		.xbutton.button=0xfe
	};
	XSendEvent		(S->i.d,S->i.w,False,ButtonPressMask,&v);
}
#endif
