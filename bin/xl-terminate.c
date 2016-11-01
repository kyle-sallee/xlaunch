/*	By Kyle Sallee in 2015 copyrighted.	*/

extern	char	*optarg;
extern	int	optind,opterr,optopt;

#include	<err.h>
#include	<getopt.h>
#include	<glob.h>
#include	<signal.h>
#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<sys/types.h>
#include	"../lib/copyright.h"
#include	"../lib/display_env.h"
#include	"../lib/exe.h"

static	inline	void	slay	(char *n,pid_t a)
{	n+=	6;
	char	*e=
	strchr	(n,'/');
	e[0]=	0;
	pid_t	p;
	sscanf	(n,"%li",&p);
	if	(p!=a)
	kill	(p,SIGTERM);
}

static	inline	char	*	name	(char *n)
{	char	*r=	NULL;
	FILE		*f;
	if		((f=fopen(n,"r"))==NULL)
	goto		ret;
	char	*line=	NULL;
	size_t	size=	0;
	getdelim	(&line,&size,0,f);
	fclose		(f);
	ret:
	return		r;
}

static	void	doom	(char *c,pid_t a)
{	glob_t		t;
	glob		("/proc/*/cmdline",0,NULL,&t);	/**/
	int		i;
	for		(i=0;t.gl_pathc>i;i++)
	if		(strcmp(c,name(t.gl_pathv[i]))==0)
	slay		(c,a);
}

static	inline	void	grab_term	(void)
{	doom		("xl-grab",0);	}

static	inline	void	grab_restart	(void)
{	pid_t		p=
	forkexe		((char *[]){"xl-grab",NULL});
	sleep		(1);
	doom		("xl-grab",p);
}

static	inline	char	*	lockfile	(char *d)
{	char		*n=
	malloc		(  0x100);
	snprintf	(n,0x100,"/tmp/.X%s-lock",d+1);
	return		n;
}

static	inline	pid_t	pidread	(char *n)
{	FILE	*f=
	fopen	(n,"r");
	if	(f==NULL)
	err	(errno,"fopen %s",n);
	pid_t	p;
	fscanf	(f,"%i",&p);
	return	p;
}

static	inline	void	xserver_term	(void)
{	kill	((pidread(lockfile(display_env()))),SIGTERM);	}

char	main	(int argc,char *argv[])
{	copyright		();
	if			(argc!=2)
	goto			help;
	int			oi=		0,
				opt=		0;
	const	struct	option	los[]=
	{	{"restart",	      no_argument,0,'r'},
		{"self",	      no_argument,0,'s'},
		{"xserver",	      no_argument,0,'x'},
		{0,0,0,0}
	};
	while			((opt=getopt_long(argc,argv,"rsx",los,&oi))!=-1)
	switch			(opt)
	{	case	'r':	grab_restart	();	break;
		case	's':	grab_term	();	break;
		case	'x':	xserver_term	();	break;
		default:	goto	help;
	}
	return	0;

	help:
	errx			(1,
	"Usage xl-terminate PARAMETER\n\n"
	"-r, --restart	xl-grab is restarted.\n"
	"-s, --self	xl-grab is terminated.\n"
	"-x, --xserver	xserver is termianted.\n\n"
	"On this computer in this PID name space by this account\n"
	"if   xl-grab was not started\n"
	"then xl-grab can not be terminated."
	"On this computer in this PID name space by this account\n"
	"if   the xserver was not started\n"
	"then the xserver can not be terminated.\n"
	"If   the xserver is termianted\n"
	"then all attached clients typically terminate.\n"
	"One parameter only is required.\n"
	);
}
