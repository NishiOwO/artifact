# $Id$

PREFIX = /usr/local

CC = gcc
CFLAGS = -std=c99 -g -D_DEFAULT_SOURCE -D_XOPEN_SOURCE=600 -DPREFIX=\"$(PREFIX)\" -fcommon -I. -Werror -I$(TOPDIR)
LDFLAGS =
LIBS =

# include $(TOPDIR)/makefiles/musl-host.mk
include $(TOPDIR)/makefiles/musl-gcc.mk
