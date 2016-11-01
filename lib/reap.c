/*	By Kyle Sallee in 2015 copyrighted.	*/

#include	"reap.h"

static	pid_t	epid=	-1;

static	void	schld	(int i)
{	pid_t		r;
	int		a;
	while		((r=waitpid(-1,&a,WNOHANG))>0)
	if		(r==epid)
	{	int	b;
		if	(b=WEXITSTATUS(a))
		exit	(b);
	}
}

void	forkexesafe	(char **c)
{	epid=
	forkexe	(c);
}

void	reap	(void)
{	struct sigaction	sa;
	sigemptyset	(&sa.sa_mask);
	sa.sa_flags=	SA_RESTART|SA_NODEFER;
	sa.sa_handler=	schld;
	sigaction	(SIGCHLD,&sa,NULL);
}
