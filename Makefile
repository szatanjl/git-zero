package = git-zero
distfiles = Makefile git-zero.c git-zero-init.sh README

CC = cc
CFLAGS = -O3 -march=native -Wall -Wextra
LDFLAGS = -lssl -lcrypto

prefix = /usr/local
exec_prefix = $(prefix)
bindir = $(exec_prefix)/bin

.PHONY: all check dist clean distclean install uninstall

all: git-zero

check:

dist: clean
	mkdir $(package)
	cp -f $(distfiles) $(package)
	tar -cf $(package).tar $(package)
	gzip $(package).tar

clean:
	rm -Rf git-zero git-zero-init \
	       $(package) $(package).tar $(package).tar.gz

distclean: clean

install: all
	mkdir -p $(DESTDIR)$(bindir)
	cp -f git-zero $(DESTDIR)$(bindir)
	cp -f git-zero-init.sh $(DESTDIR)$(bindir)/git-zero-init
	chmod +x $(DESTDIR)$(bindir)/git-zero \
	         $(DESTDIR)$(bindir)/git-zero-init

uninstall:
	rm -f $(DESTDIR)$(bindir)/git-zero \
	      $(DESTDIR)$(bindir)/git-zero-init

git-zero: git-zero.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<
