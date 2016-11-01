/*	By Kyle Sallee in 2015 copyrighted.	*/

#ifndef		_SACKN_H
#define		_SACKN_H
#include	<string.h>
#include	"../lib/overlay.h"
#include	"../lib/var.h"

static	inline	void	sack_create	(SACK *s)
{	s->i.w=
	overlay			(s->i,XC_sailboat,ButtonPressMask,s->ni);

	XSetAttributes		as=
	{	.event_mask=	SubstructureNotifyMask	};
	Display			*d=	s->i.d;
	Window			r=	s->i.a.root;
	s->s.w=
	XCreateWindow		(d,r,s->s.a.x,s->s.a.y,s->s.a.width,s->s.a.height,0,
				CopyFromParent,InputOutput,CopyFromParent,
				CWEventMask,&s->as);
	XMapRaised		(d,s->s.w);
	XStoreName		(d,s->s.w,s->ns);
}
#endif
