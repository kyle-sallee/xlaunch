/*	By Kyle Sallee in 2015 copyrighted.	*/
#ifndef		_EXE_H
#define		_EXE_H
#include	<err.h>
#include	<errno.h>
#include	<fcntl.h>
#include	<stdnoreturn.h>
#include	<unistd.h>
		pid_t	forkme		(void);
noreturn	void	exe		(char **c);
		pid_t	forkexe		(char **c);
		void	forkexeself	(char **c);
#endif
