/*	By Kyle Sallee in 2015 copyrighted.	*/
#ifndef		_CONFIG_FILE_H
#define		_CONFIG_FILE_H
#include	<err.h>
#include	<errno.h>
#include	<fcntl.h>
#include	<stdlib.h>
#include	<string.h>
#include	<sys/stat.h>
#include	<sys/types.h>
#include	<unistd.h>
#include	<wordexp.h>
typedef		struct
{	int			file;
	char		*	buf;
	wordexp_t	*	word;
	off_t			size;
	int			lines;
}	CONFIG;
CONFIG	config_parse	(void);
#endif
