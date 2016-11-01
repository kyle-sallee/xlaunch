/*	By Kyle Sallee in 2015 copyrighted.	*/
#ifndef		_WARP_H
#define		_WARP_H
#include	"var.h"
PAIR	pointer_at	(Display * const d,Window const w);
void	pointer_warp	(Display * const d,Window const w,PAIR const p);
void	limit_warp	(Display * const d,Window const w);
#endif
