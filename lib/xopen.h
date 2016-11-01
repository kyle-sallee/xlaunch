/*	By Kyle Sallee in 2015 copyrighted.	*/
#ifndef		_XOPEN_H
#define		_XOPEN_H
#include	<err.h>
#include	"delay.h"
#include	"display_env.h"
#include	"var.h"
DWA	xopen	(int (*)(Display *,XErrorEvent *));
#endif
