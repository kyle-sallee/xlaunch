/*	By Kyle Sallee in 2015 copyrighted.	*/

#ifdef	__GNUC__
#define	  likely(x)	__builtin_expect((x),1)
#define	unlikely(x)	__builtin_expect((x),0)
#else
#define	  likely(x)	(x)
#define	unlikely(x)	(x)
#warning	"gcc provided __builtin_expect is unavailable."
#endif

#include	"mod_parse.h"
int	mod_parse	(char const * const s)
{	if	(unlikely(strcasecmp(s,"ShiftMask"  )==0))	return	ShiftMask;
	if	(unlikely(strcasecmp(s,"LockMask"   )==0))	return	LockMask;
	if	(unlikely(strcasecmp(s,"ControlMask")==0))	return	ControlMask;
	if	(unlikely(strcasecmp(s,"Mod1Mask"   )==0))	return	Mod1Mask;
	if	(unlikely(strcasecmp(s,"Mod2Mask"   )==0))	return	Mod2Mask;
	if	(unlikely(strcasecmp(s,"Mod3Mask"   )==0))	return	Mod3Mask;
	if	(unlikely(strcasecmp(s,"Mod4Mask"   )==0))	return	Mod4Mask;
	if	(unlikely(strcasecmp(s,"Mod5Mask"   )==0))	return	Mod5Mask;
	return	0;
}
