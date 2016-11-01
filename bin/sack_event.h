/*	By Kyle Sallee in 2015 copyrighted.	*/

#ifndef		_SACK_EVENT_H
#define		_SACK_EVENT_H
#include	<err.h>
#include	<stdio.h>
#include	"../lib/aim.h"
#include	"../lib/overlay.h"
#include	"../lib/sack.h"
#include	"../lib/warp.h"
#include	"../lib/window_find.h"

static	inline	void	sack_adopt	(SACK *S,Window w)
{	XEvent			v=
	{	.xbutton.type=	ButtonPress,
		.xbutton.button=0xff,
		.xbutton.window=w
	};
	XSendEvent		(S->i.d,S->i.w,False,ButtonPressMask,&v);
}

static	void	sack_add	(SACK *S,Window w)
{	Display			*d=	S->i.d;
	XAddToSaveSet		(d,w);
	XResizeWindow		(d,w,S->s.a.width,S->s.a.height);
	XSetWindowBorderWidth	(d,w,0);
	XReparentWindow		(d,w,S->s.w,0,0);
	XLowerWindow		(d,  S->s.w);
}


static	inline	void	sack_tile	(SACK *S)
{	Display	*d=			S->i.d;
	Window				*p;
	int	t=	xtree		(d,S->s.w,&p)-1;
	int				i;
	for				(i=1;t>=(i*i);i++);

	Window	r=			S->a.w[0].w;
	/* For the aim root window the attributes were not set?	*/

	int				w,h;
	{	XWindowAttributes	a;
		r=			DefaultRootWindow(d);
		XGetWindowAttributes	(d,r,&a);
		w=			a.width /i;
		h=			a.height/i;
	}

	w=				1920/i;
	h=				1080/i;
	int				a,b,x,y;
	for				(y=0,b=i;b;b--,y+=h)
	for				(x=0,a=i;a;a--,x+=w)
	if				(t>=0)
	{	t--;
		Window	n=		*p++;
		XReparentWindow		(d,n,r,x,y);
		XMoveWindow		(d,n,x,y);
		XResizeWindow		(d,n,w,h);
	}

/*	XFree				(P);
	XKillClient			(d,s);
	XDestroyWindow			(d,w);
*/
}

static	void	sack_liberate	(SACK *S)
{	Display	*d=			S->s.d;
	Window	r=			S->s.a.root;

	Window				w;
	int				x,y;
	{	int			I;
		Window			W;
		XQueryPointer		(d,r,&W,&w,&x,&y,&I,&I,&I);
	}
	if				(w==S->s.w)
	sack_tile			(S);
	else
	{	Window			*p;
		int	t=	xtree	(d,S->s.w,&p);
		XReparentWindow		(d,p[t-1],r,x,y);
		XFree			(p);
	}
}

static	inline	SACK	sack_init	(DWA *R,DWA *W,char *c,int skip)
{	SACK				S;
	memset				(&S,0,sizeof(S));
	S.i=	dwa			(R,None);
	S.s=	dwa			(W,None);
	strcpy				(S.ns,"xl sack ");
	strcpy				(S.ni,"xl input sack ");
	S.a.s=				skip;

	Display	*d=			R->d;
	Window	r=			R->w;
	if				(c)
	goto				named;

	S.a=		aim		(R,skip);
	DWA	*P=	aim_sack	(&S.a);

	if				(P==NULL)
	goto				numbered;
	S.s=				*P;
	char				*n;
	XFetchName			(d,S.s.w,&n);
	strncpy				(S.ns+0x8,n+8,0x20);
	strncpy				(S.ni+0xd,n+8,0x20);
	XFree				(n);
	S.i=	dwa			(R,window_find(d,r,S.ni));
	goto				ret;

	named:
	strncpy				(S.ns+0x8,c,0x20);
	strncpy				(S.ni+0xd,c,0x20);
	S.s=	dwa			(R,window_find(d,r,S.ns));
	S.i=	dwa			(R,window_find(d,r,S.ni));
	if				(S.i.w)
	goto				ret;
	else
	goto				make;

	numbered:
	W=				&S.a.w[S.a.s+1];
	int	i=			0;
	do	sprintf			(S.ns+8,"%i",++i);
	while				(window_find(d,r,S.ns));
	sprintf				(S.ni+0xd,"%i",i);

	make:
	S.i=
	overlay				(R,XC_sailboat,ButtonPressMask,S.ni);

	XSetWindowAttributes		as=
	{	.event_mask=		SubstructureNotifyMask	};

	S.s.w=
	XCreateWindow			(d,r,W->a.x,W->a.y,W->a.width,W->a.height,0,
					CopyFromParent,InputOutput,CopyFromParent,
					CWEventMask,&as);
	S.s.a=				W->a;
	XStoreName			(d,S.s.w,S.ns);
	XMapRaised			(d,S.s.w);
	S.made++;

	ret:
	if				(W->w)
	if				(S.made)
	sack_add			(&S,W->w);
	else
	sack_adopt			(&S,W->w);
	return				S;
}

static	inline	int	sack_ignore	(SACK *S)
{	XUnmapWindow	(S->i.d,S->i.w);	}

static	inline	void	sack_up		(SACK *S)
{	XCirculateSubwindowsUp	(S->s.d,S->s.w);	}

static	inline	void	sack_down	(SACK *S)
{	XCirculateSubwindowsDown(S->s.d,S->s.w);	}

static	inline	void	sack_meld	(SACK *s,Window sb)
{	Window			rr,pr;
	Window			*cr,*cp;
	unsigned int		c;
	XQueryTree		(s->i.d,sb,&rr,&pr,&cr,&c);
	if			(c)
	{	for		(cp=cr;c>0;c--,cp++)
		sack_add	(s,*cp);
		XFree		(cr);
	}
}

static	inline	void	sack_dump	(SACK *s)
{	Window			rr,pr;
	Window			*cr,*cp;
	unsigned int		c;

	Display			*d=	s->i.d;
	Window			r=	s->i.a.root;
	Window			w=	s->s.w;
	XWindowAttributes	*a=	&s->s.a;

	XQueryTree		(d,w,&rr,&pr,&cr,&c);
	if			(c)
	{	for		(cp=cr;c>0;c--)
		XReparentWindow	(d,*cp++,r,a->x,a->y);
/*		XFree		(cr);		*/
	}

/*	XKillClient		(d,s);
	XDestroyWindow		(d,w);
*/
}

static	inline	void	buttonp	(SACK *S)
{	Display			*d=	S->s.d;
	Window			k=	S->s.w;
	Window			i=	S->i.w;
	S->a=
	aim			(&(DWA){.d=d, .w=S->i.a.root},S->a.s);
	DWA			*p=
	aim_sack		(&S->a);

	if			(p)
	{	if		(p->w==k)
		sack_ignore	(S);
		else
		sack_meld	(S,p->w);
		return;
	}

	Window			w=	S->a.w[S->a.s+1].w;

	if			(w)
	{	if		(w==i)
		{	XCloseDisplay
				(d);
			exit	(0);
		}
		else
		sack_add	(S,w);
	}
}

static	inline	int	sack_event	(SACK *S)
{	Display		*d=	S->i.d;
	Window		i=	S->i.w;
	Window		s=	S->s.w;

	do
	{	resume:
		XRaiseWindow	(d,i);
		XEvent		e;
		XNextEvent	(d,&e);
		XLowerWindow	(d,i);
		switch		(e.type)
	{
/*		case	CirculateNotify:
		case	ConfigureNotify:
		case	CreateNotify:
		case	GravityNotify:
		case	MapNotify:
		case	UnmapNotify:
		goto	resume;
*/
		case	ButtonPress:
		switch	(e.xbutton.button)
	{	case	1:	buttonp		(S);			break;
		case	3:	sack_liberate	(S);			break;
		case	4:	sack_up		(S);		goto	resume;
		case	0xff:	sack_add	(S,e.xbutton.window);	break;
		default:	sack_down	(S);		goto	resume;
	}
	}
	}
	while			(xtreec(d,s));
	return			0;
}
#endif
