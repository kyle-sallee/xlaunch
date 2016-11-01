bindir		=	$(prefix)/bin$(SUFFIX)
sources		=	$(wildcard *.c)
executable	=	$(sources:.c=)
.PHONY		:	all clean install
LDFLAGS		+=	-L../lib -lX11 -lxl
all		:	$(executable)
clean		:
	rm -f		$(executable)
xl-auto		:	xl-auto.c
	$(CC) $(CFLAGS)  $(LDFLAGS)  $^ -o $@ -lXext
xl-gamma	:	xl-gamma.c
	$(CC) $(CFLAGS)  $(LDFLAGS)  $^ -o $@ -lXxf86vm
xl-lock		:	xl-lock.c
	$(CC) $(CFLAGS)  $(LDFLAGS)  $^ -o $@ -lXext -lpam
install		:	all
	install -vm  755 -d $(DESTDIR)$(bindir)/
	install -vm  755 -t $(DESTDIR)$(bindir) $(executable)
	chmod      04755    $(DESTDIR)$(bindir)/xl-lock
