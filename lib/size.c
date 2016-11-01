/*	By Kyle Sallee in 2015 copyrighted.	*/
#include	"size.h"
static	inline	int	pixel_min	(int a,int b)
{	if		(a>=b)	return	a;
	else			return	b;
}
void	size	(AIM * const A,int const width,int const height)
{	Display		*d=	A->w->d;
	DWA		*P=	A->w+A->s+1;
	for			( ;P->w;P++)
	XResizeWindow		(d,P->w,
	pixel_min		(P->a.width +width ,0x20),
	pixel_min		(P->a.height+height,0x20));
}
void	size_set	(AIM * const A,int const width,int const height)
{	Display		*d=	A->w->d;
	DWA		*P=	A->w+A->s+1;
	for			( ;P->w;P++)
	XResizeWindow		(d,P->w,width,height);
}
