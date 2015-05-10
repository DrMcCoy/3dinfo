ACLOCAL_AMFLAGS = -I m4 --install

EXTRA_DIST =

SUBDIRS = \
          glew \
          src \
          $(EMPTY)

doxygen:
	doxygen

doxygen-clean:
	rm -rf doc/doxygen/html/
	rm -rf doc/doxygen/latex/
	rm -rf doc/doxygen/man/
	rm -rf doc/doxygen/*.tmp
	rm -rf doc/doxygen/*.db
