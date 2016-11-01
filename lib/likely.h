/*	By Kyle Sallee in 2015 copyrighted.	*/
#if		defined (__GNUC__)||defined (__LLVM__)||defined (__CLANG__)
#define		  likely(x)	__builtin_expect((x),1)
#define		unlikely(x)	__builtin_expect((x),0)
#else
#define		  likely(x)	(x)
#define		unlikely(x)	(x)
#warning	"gcc/llvm provided __builtin_expect is unavailable."
#endif
