# $Id$

TOPDIR = .
include Config.mk

.PHONY: all build-bin clean install makefiles distclean

all: build-bin

makefiles: Makefile.in
	cd bin && for i in *; do \
		sed "s/@output@/$$i/g" ../Makefile.in | sed "s/@objects@/`cd $$i && ls -d *.c | tr '\n' ' ' | sed -E 's/\.c/.o/g'`/g" | sed "s/@ifreq@//g" > $$i/Makefile ; \
	done

build-bin: makefiles
	TOPDIR="`pwd`" && cd bin && for i in *; do \
		$(MAKE) -C $$i TOPDIR="$$TOPDIR" || break ; \
	done

clean: makefiles
	TOPDIR="`pwd`" && cd bin && for i in *; do \
		$(MAKE) -C $$i clean TOPDIR="$$TOPDIR" ; \
	done

install: all
	TOPDIR="`pwd`" && cd bin && for i in *; do \
		$(MAKE) -C $$i install TOPDIR="$$TOPDIR" ; \
	done

distclean: makefiles clean
	rm -f */*/Makefile
