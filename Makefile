export	CC	?=	gcc
export	prefix	=	/usr
export	CFLAGS	?=	-march=native -Os -pipe -Wunused -DDELAY=5
export	LDFLAGS	?=	-Wl,-s -Wl,-O1
MAKEFLAGS	=	-j
sub		=	lib bin etc man script
.PHONY		:	all clean install
subdirs:
	for dir in $(sub); do $(MAKE) -C $$dir;		done
all:
	for dir in $(sub); do $(MAKE) -C $$dir     all;	done
install:
	for dir in $(sub); do $(MAKE) -C $$dir install;	done
clean:
	for dir in $(sub); do $(MAKE) -C $$dir   clean;	done
