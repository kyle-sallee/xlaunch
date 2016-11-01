/*	By Kyle Sallee in 2015 copyrighted.	*/
#ifndef		_AIM_H
#define		_AIM_H
#include	<err.h>
#include	<string.h>
#include	"dwa.h"
#include	"sack.h"
#include	"xtree.h"
#include	"var.h"
Window	aim1		(Display     * const,Window);
AIM	aim		(DWA   const * const,const int);
Window	aim_valid	(AIM * const);
DWA	*aim_sack	(AIM * const);
#endif
