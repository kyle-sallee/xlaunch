/*	By Kyle Sallee in 2015 copyrighted.	*/
#ifndef		_WINDOW_NAMECMP_H
#define		_WINDOW_NAMECMP_H
#include	<signal.h>
#include	<string.h>
#include	<X11/Xlib.h>
/* if the name matches then 0 is returned.	*/
char	window_namecmp	(Display * const,Window const,char const * const);
#endif
