CFLAGS		+=	-fPIC
LDFLAGS		+=	-lX11 -lXext -lXxf86vm
incdir		=	$(prefix)/include$(SUFFIX)
libdir		=	$(prefix)/lib$(SUFFIX)
source		=	$(wildcard *.c)
object		=	$(source:.c=.o)
symlink		=	libxl.so
library		=	libxl.so.1
.PHONY		:	all install
all		:	$(library)
clean		:
	rm -f		$(library) $(object) $(symlink)
install		:	all
	install -vm  755 -d $(DESTDIR)$(libdir)/ $(DESTDIR)$(incdir)/xl2/
	install -vm  644 -t $(DESTDIR)$(incdir)/xl2/ *.h
	install -vm  755 -t $(DESTDIR)$(libdir) $(library)
	ln	-vs $(library) $(DESTDIR)$(libdir)/$(symlink)
$(library)	:	$(object)
	$(CC) $(LDFLAGS) -shared -Wl,-soname,$(library) $^ -o $@
	ln	-vs	$(library)	libxl.so

