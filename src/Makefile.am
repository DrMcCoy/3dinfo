include $(top_srcdir)/Makefile.common

SUBDIRS =

noinst_HEADERS = \
                 capability.h \
                 $(EMPTY)

bin_PROGRAMS = 3dinfo

3dinfo_SOURCES = \
                 3dinfo.cpp \
                 capability.cpp \
                 $(EMPTY)

3dinfo_LDADD = \
               ../glew/libglew.la \
               $(LDADD) \
               $(EMPTY)
