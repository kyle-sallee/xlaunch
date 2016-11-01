/*	By Kyle Sallee in 2015 copyrighted.	*/

#include	"dwa.h"
DWA	dwa	(DWA const * const d,Window const w)
{	DWA				D;
	memset				(&D,0,sizeof(D));
	D.d=				d->d;
	if				(w)
	{	D.w=			w;
		XGetWindowAttributes	(d->d,w,&D.a);
	}
	else
	{	if			(d->a.root)
		D.a=			d->a;
		else
		XGetWindowAttributes	(d->d,d->w,&D.a);
		D.a.root=		d->w;
	}
	return				D;
}
