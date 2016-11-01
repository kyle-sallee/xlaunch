/*	By Kyle Sallee in 2015 copyrighted.	*/

#include	"display_env.h"
char	*display_env	(void)
{	copyright	();
	char		*n=
	getenv		("DISPLAY");
	if		(n)
	return		n;
	errx		(1,
			"The DISPLAY environment variable must exist.\n"
			"xl-start ran?");
}
