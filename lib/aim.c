/*	By Kyle Sallee in 2015 copyrighted.	*/

#include	"aim.h"
Window	aim1	(Display * const d,Window const w)
{	Window		r,p;
	int		rx,ry,wx,wy,m;
	XQueryPointer	(d,w,&r,&p,&rx,&ry,&wx,&wy,&m);
	return		p;
}

static	inline	void	aim2			(AIM *a,Window w)
{	DWA	r=				*a->w;
	Display	*d=				r.d;
	Window					n=	w;
	while					(n=aim1(d,n))
	{	a->w[a->t++]=	dwa		(&r,n);
		r.w=				n;
		if				(xl_sack(&r)==0)
		{	Window			*t,*T;
			int			x=
			xtree			(d,n,&t);
			for			(T=t;x;x--,t++)
			a->w[a->t++]=	dwa	(&r,*t);
			r.w=	n=		*--t;
			XFree			(T);
		}
	}
	a->w[a->t].w=				None;
}

AIM	aim		(DWA const * const r,int const s)
{	AIM		a;
	memset		(&a,0,sizeof(a));
	a.w[0]=		*r;
	a.s=		s;
	a.t=		1;
	aim2		(&a,a.w[0].w);
	return		a;
}

Window	aim_valid	(AIM * const a)
{	Window	w=	a->w[a->s+1].w;
	if		(w)
	return		(w);
	errx		(1,"An in window pointer is required.");
}

DWA	*aim_sack	(AIM * const a)
{	DWA		*w;
	for		(w=a->w+1;w->w;w++)
	if		(xl_sack(w)==0)
	return		w;
	return		NULL;
}
