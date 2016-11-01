/*	By Kyle Sallee in 2015 copyrighted.	*/

#include	"config_file.h"

static	inline	int	config_open	(void)
{	int		f;
	wordexp_t	t;
	wordexp		("~/.config/xl",&t,0);
	if		((f=open	(t.we_wordv[0],O_RDONLY))==-1)
	if		((f=open	(    "/etc/xl",O_RDONLY))==-1)
	err		(errno,"File ~/.config/xl or file /etc/xl is required.");
	wordfree	(&t);
	return		f;
}

static	inline	int	lf_to_zero	(char *c)
{	int	t;
	for	(t=0;c=strchr(c,'\n');t++)
	*c++=	0;
	return	t;
}

static	inline	off_t	file_size	(int f)
{	struct	stat	b;
	if		(fstat(f,&b)==0)
	return		b.st_size;
	err		(errno,"fstat");
}

static	inline	void	wordify	(char *b,wordexp_t *w,int i)
{	for			(;i;i--,w++)
	{	wordexp		(b,w,WRDE_NOCMD);
		b+=		1+
		strlen		(b);
	}
}

CONFIG	config_parse	(void)
{	CONFIG			c;
	c.file=	config_open	();
	c.size=	file_size	(c.file);
	c.buf=	calloc		(c.size+1,1);
	if			(c.buf==NULL)
	goto			err_calloc;
	if			(read(c.file,c.buf,c.size)!=c.size)
	err			(errno,"read");
	close			(c.file);
	c.lines=lf_to_zero	(c.buf);
	c.word=	calloc		(c.lines,sizeof(wordexp_t));
	if			(c.word==NULL)
	goto			err_calloc;
	wordify			(c.buf,c.word,c.lines);
	free			(c.buf);
	return			(c);
	err_calloc:
	err			(errno,"calloc");
}
