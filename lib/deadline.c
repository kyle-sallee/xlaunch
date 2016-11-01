/*	By Kyle Sallee in 2015 copyrighted.	*/
#include	"deadline.h"

struct	itimerval	timer_dead_val;
struct	itimerval	timer_disable_val;

static	void	dead	(int i)
{	exit	(0);	}

void	deadline_arm	(void)
{	setitimer	(ITIMER_REAL,&timer_dead_val,NULL);	}

void	deadline_disarm	(void)
{	setitimer	(ITIMER_REAL,&timer_disable_val,NULL);	}

void	deadline_start			(const time_t i,void (*f)(int))
{	struct	sigaction		sa;
	sigemptyset			(&sa.sa_mask);
	sa.sa_flags=			SA_RESTART|SA_NODEFER;
	if				(f)
	sa.sa_handler=			f;
	else
	sa.sa_handler=			dead;
	sigaction			(SIGALRM,&sa,NULL);
	timer_dead_val.it_value.tv_sec=	i;
	deadline_arm			();
}
