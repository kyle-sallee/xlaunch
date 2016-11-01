/*	By Kyle Sallee in 2015 copyrighted.	*/
#ifndef		_REAP_H
#define		_REAP_H
#include	<signal.h>
#include	<stdlib.h>
#include	<sys/types.h>
#include	<sys/wait.h>
#include	"exe.h"
void	forkexesafe	(char **);
void	reap		(void);
#endif
