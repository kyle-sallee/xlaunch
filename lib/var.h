/*	By Kyle Sallee in 2015 copyrighted.	*/
#ifndef		_VAR_H
#define		_VAR_H
#include	<X11/Xlib.h>
typedef		struct
{	Display			*d;
	Window			w;
	XWindowAttributes	a;
/*	For root window .a.root is used.	*/
}	DWA;

typedef		struct
{	unsigned int	state,keycode;
	char		*string;
}	KEY;

typedef		struct
{	int	x,y;				/*	coordinates	*/
}	PAIR;

typedef		struct
{	DWA			d,i,r;		/*	desk,input,root	*/
	char			nd[0x30];	/*	name desk	*/
	char			ni[0x30];	/*	name input	*/
}	DESK;

typedef		struct
{	DWA			w[0x100];
	int			s,t;		/*	skip, total	*/
}	AIM;

typedef		struct
{	char			ni[0x30];	/*	name input	*/
	char			ns[0x30];	/*	name sack	*/
	AIM			a;
	DWA			i,s;		/*	input, sack	*/
	int			made;
}	SACK;

typedef		struct
{	DWA			i;		/*	input		*/
	AIM			a;
	PAIR			A,B,C;
/*	A is the anchor coordinate.
	B is the move   change.
	C is the size   change.
*/
	int			mode,modulo;
}	MOD;
#endif
