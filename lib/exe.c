/*	By Kyle Sallee in 2015 copyrighted.	*/

#include	"exe.h"
pid_t	forkme	(void)
{	pid_t	p;
	if	((p=fork())!=-1)
	return	p;
	err	(errno,"fork");
}

noreturn	void	exe	(char **c)
{	execvp	(c[0],c);
	err	(errno,"execvp: %s",c[0]);
}

pid_t	forkexe	(char **c)
{	pid_t	p;
	if	(p=forkme())
	return	p;
	exe	(c);
}

void	forkexeself	(char **c)
{	pid_t	p;
	if	(p=forkme())
	exe	(c);
}
