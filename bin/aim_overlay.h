/*	By Kyle Sallee in 2015 copyrighted.	*/

#ifndef		_AIM_OVERLAY_H
#define		_AIM_OVERLAY_H
#include	<X11/XKBlib.h>
#include	<X11/keysym.h>
#include	<sys/types.h>
#include	<sys/wait.h>
#include	"../lib/aim.h"
#include	"../lib/exe.h"
#include	"../lib/overlay.h"

static	void	aim_border_off	(AIM *A)
{	DWA	*D=		&A->w[A->s+1];
	XSetWindowBorderWidth	(D->d,D->w,0);
}

static	void	aim_border_on	(AIM *A)
{	DWA	*D=		&A->w[A->s+1];
	XSetWindowBorderWidth	(D->d,D->w,16);
/*	XRaiseWindow		(D->d,D->w);	*/
}

static	inline	void	aim_inform	(AIM *A,Display *d)
{	Window		w=	A->w[A->s+1].w;
	char		*n,*N;
	XFetchName	(d,w,&n);
	if		(n==NULL)
	N=		"";
	else
	N=		n;
	char		m[0x40];
	snprintf	(m,0x40,"aim mode %x %s",w,N);
	if		(n)
	XFree		(n);
	waitpid		(forkexe((char *[]){"xl-info","-s",m,NULL}),NULL,0);
}

static	AIM	aim_overlay	(DWA *R)
{	Display	*d=	R->d;
	DWA		I=
	overlay		(R,XC_target,ButtonPressMask|KeyPressMask,"xl input aim");
	Window	i=	I.w;
	AIM	A=	{	.s=	-1	};
	goto		skip;
	again:
	aim_border_off	(&A);
	skip:
	if		((++A.s)>=(A.t-1))
	A.s=		0;
	XUnmapWindow	(d,i);
	A=	aim	(R,A.s);
	aim_inform	(&A,d);
	aim_border_on	(&A);
	XMapRaised	(d,i);
	XEvent		e;
	XNextEvent	(d,&e);
	switch		(e.type)
	{	case	ButtonPress:
		switch	(e.xbutton.button)
		{	case	1:		goto	ret;	}
		break;
		case	KeyPress:
		switch	(XkbKeycodeToKeysym(d,e.xkey.keycode,0,0))
		{	case	XK_Return:	goto	ret;	}
	}
	goto		again;
	ret:
	XDestroyWindow	(d,i);
	aim_border_off	(&A);
	return		A;
}
#endif
