/*	By Kyle Sallee in 2015 copyrighted.	*/
#ifndef		_DEADLINE_H
#define		_DEADLINE_H
#include	<signal.h>
#include	<stdio.h>
#include	<stdlib.h>
#include	<sys/time.h>
void	deadline_arm	(void);
void	deadline_disarm	(void);
void	deadline_start	(const time_t,void (*)(int));
#endif
