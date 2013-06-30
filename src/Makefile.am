AUTOMAKE_OPTIONS = subdir-objects
ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS} -I .

bin_PROGRAMS = shell
shell_SOURCES = \
shell.cxx \
Chain.cxx \
Command.cxx

AM_CPPFLAGS = $(DEPS_CFLAGS)

shell_LDADD = $(DEPS_LIBS) 
