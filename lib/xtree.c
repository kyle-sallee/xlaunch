/*	By Kyle Sallee in 2015 copyrighted.	*/

#include	"xtree.h"

unsigned int	xtree	(Display * const d,Window const w,Window **cr)
{	Window		rr,pr;
	unsigned int	c;
	XQueryTree	(d,w,&rr,&pr,cr,&c);
	return		c;
}

unsigned int	xtreec	(Display * const d,Window const w)
{	Window		*cr,rr,pr;
	unsigned int	c;
	XQueryTree	(d,w,&rr,&pr,&cr,&c);
	XFree		(cr);
	return		c;
}
