/*	By Kyle Sallee in 2015 copyrighted.	*/

#include	"newmap.h"

static	inline	int	override	(Display * const d,Window const w)
{	XWindowAttributes	a;
	XGetWindowAttributes	(d,w,&a);
	return			a.override_redirect;
}

Window	newmap	(Display * const d)
{	XEvent	e;
	while	(XNextEvent(d,&e)==0)
	if	(e.type==MapNotify)
	if	(override(d,e.xmap.window)==False)
	return	e.xmap.window;
/*	else
	return	None;
*/
}
