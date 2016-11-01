/* By Kyle Sallee in 2015 copyrighted. */

/* By xl-lock.c screen is blanked, pointer and keyboard are grabbed;
   and for release PAM approved authentication is required.
/*

/* link with -lX11 -lXext -lpam */

extern	char	*optarg;
extern	int	optind,opterr,optopt;

#define		_GNU_SOURCE
#include	<X11/Xlib.h>
#include	<X11/extensions/dpms.h>
#include	<X11/keysym.h>
#include	<X11/XKBlib.h>
#include	<err.h>
#include	<errno.h>
#include	<getopt.h>
#include	<pwd.h>
#include	<sched.h>
#include	<security/pam_appl.h>
#include	<signal.h>
#include	<stdio.h>
#include	<stdlib.h>
#include	<sys/types.h>
#include	<sys/wait.h>
#include	<string.h>
#include	<unistd.h>
#include	"../lib/xopen.h"

extern char	**environ;

static	int	conv	(int num_msg,const struct pam_message **msgm,
			 struct pam_response **response,void *appdata_ptr)
{	struct	pam_response	*r;
	*response=
	r=		(struct pam_response *)
	calloc		(num_msg,sizeof(struct pam_response));
	if		(r==NULL)
	{	warnx	("calloc");
		return	PAM_CONV_ERR;
	}

	for		(num_msg--;num_msg>=0;num_msg--)
	{	switch	(msgm[num_msg]->msg_style)
		{	case	PAM_PROMPT_ECHO_OFF:
			case	PAM_PROMPT_ECHO_ON:
			r[num_msg].resp=strdup(appdata_ptr);
			default:
			r[num_msg].resp_retcode=0;
		}
	}
	return	PAM_SUCCESS;
}

static	inline	void	ungrab	(Display *d)
{	XUngrabKeyboard	(d,CurrentTime);
	XUngrabPointer	(d,CurrentTime);
}

static	void	try		(char *s,char *pp)
{	pam_handle_t		*pamh;
	struct	pam_conv	c={conv,pp};

	int			e=
	pam_start		("system-auth",s,&c,&pamh);
	if			(e!=PAM_SUCCESS)
	errx			(1,pam_strerror(pamh,e));

	e=
	pam_authenticate	(pamh,PAM_DISALLOW_NULL_AUTHTOK|PAM_SILENT);
	if			(e==PAM_SUCCESS)
	exit			(0);
	pam_end			(pamh,e);
}

static	inline	char	*username	(void)
{	struct	passwd	*sp=
	getpwuid	(getuid());
	if		(sp==NULL)
	err		(errno,"getpwuid");
	return		sp->pw_name;
}

static	inline	int	OnKeyRelease	(KeySym s,char *pp,int *pi)
{	switch				(s)
	{	case	XK_BackSpace:
			if		(*pi)	(*pi)--;
			return		0;
		case	XK_Return:
			pp[*pi]=	0;
			*pi=		0;
			try		(username(),pp);
			try		("root",pp);
			return		1;
	}

	if				(((s>0x20)&&(s<0x7f))||(s==0x1b))
	{	pp[*pi]=		((char) s);
		(*pi)++;
		if			(*pi==PAM_MAX_MSG_SIZE)
		*pi=			0;
	}
	return	0;
}

static	inline	void	blank	(Display *d,Window n)
{	char			*pp=
	malloc			(PAM_MAX_MSG_SIZE);
	if			(pp==NULL)
	err			(errno,"malloc");

	int		pi=	0;
	unsigned long	rgb=	0;

	again:;
	XEvent			e;
	XSetWindowBackground	(d,n,rgb);
	XClearWindow		(d,n);
	XNextEvent		(d,&e);
	if			(e.type==KeyRelease)
	switch			(OnKeyRelease(XkbKeycodeToKeysym(d,e.xkey.keycode,0,0),pp,&pi))
	{	case	0:	rgb=	0;		break;
		case	1:	rgb=	0xff0000;	break;
	}
	goto			again;
}

static	inline	int	kgrab	(Display *d,Window n)
{	return
	XGrabKeyboard	(d,n,True,GrabModeAsync,GrabModeAsync,CurrentTime);
}

static	inline	int	pgrab	(Display *d,Window n)
{	return
	XGrabPointer	(d,n,True,0, GrabModeAsync,GrabModeAsync,n,None,CurrentTime);
}

static	inline	void	grab	(Display *d,Window n)
{	if	(kgrab(d,n)!=GrabSuccess)	errx	(1,"XGrabKeyboard");
	if	(pgrab(d,n)!=GrabSuccess)	errx	(1,"XGrabPointer");
}

static	inline	Window	lock_initialize	(DWA *r)
{	Window			n=
	XCreateSimpleWindow	(r->d,r->w,0,0,r->a.width,r->a.height,0,0,0);
	XMapRaised		(r->d,n);
	XFlush			(r->d);
	return			n;
}

char	main		(int argc,char **argv)
{	sleep		(1);
	DWA		r=
	xopen		(NULL);
	int		locker=	0;
	int		oi=	0,
			opt=	0;
	const	struct	option	los[]=
	{	{"blank",	no_argument,0,'b'},
		{"dpms",	no_argument,0,'d'},
		{"lock",	no_argument,0,'l'},
		{"off",		no_argument,0,'o'},
		{0,0,0,0}
	};
	while			((opt=getopt_long(argc,argv,"bdlo",los,&oi))!=-1)
	switch			(opt)
	{	case	'b':	XForceScreenSaver	(r.d,ScreenSaverActive);	break;
		case	'd':	DPMSForceLevel		(r.d,DPMSModeSuspend);	break;
		case	'o':	DPMSForceLevel		(r.d,DPMSModeOff);	break;
		case	'l':	locker++;	break;
		default:	goto	help;
	}

	if			(locker)
	{	Window		w=
		lock_initialize	(&r);
		grab		(r.d,w);
		blank		(r.d,w);
	}
	XCloseDisplay		(r.d);
	return			0;

	help:
	errx	(1,
	"Usage xl-lock [PARAMETERS]\n\n"
	"-b, --blank	screen saver activates\n"
	"-d, --dpms	DPMS suspend mode activates\n"
	"-l, --lock	pointer and keyboard are grabbed; pass phrase is required\n"
	"-o, --off	DPMS off mode activates\n"
	);
}
