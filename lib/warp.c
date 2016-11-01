/*	By Kyle Sallee in 2015 copyrighted.	*/

#include	"warp.h"

PAIR	pointer_at	(Display * const d,Window const w)
{	PAIR		p;
	Window		r,t;
	int		rx,ry,m;
	XQueryPointer	(d,w,&r,&t,&rx,&ry,&p.x,&p.y,&m);
	return		p;
}

void	pointer_warp	(Display * const d,Window const w,PAIR const p)
{	XWarpPointer	(d,None,w,0,0,0,0,p.x,p.y);
}

void	limit_warp	(Display * const d,Window const w)
{	XWindowAttributes	a;
	XGetWindowAttributes	(d,w,&a);
	XWarpPointer		(d,None,w,0,0,0,0,a.width,a.height);
}
