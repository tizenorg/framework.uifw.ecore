MAINTAINERCLEANFILES = Makefile.in

AM_CPPFLAGS = \
-I$(top_srcdir)/src/lib/ecore \
-DPACKAGE_LIB_DIR=\"$(libdir)\" \
@EFL_ECORE_IMF_BUILD@ \
@EVIL_CFLAGS@ \
@EINA_CFLAGS@

AM_CFLAGS = @WIN32_CFLAGS@

lib_LTLIBRARIES = libecore_imf.la
includes_HEADERS = Ecore_IMF.h
includesdir = $(includedir)/ecore-@VMAJ@

libecore_imf_la_SOURCES = \
ecore_imf.c \
ecore_imf_context.c \
ecore_imf_module.c

libecore_imf_la_LIBADD = \
$(top_builddir)/src/lib/ecore/libecore.la \
@EINA_LIBS@ \
@EVIL_LIBS@

libecore_imf_la_LDFLAGS = -no-undefined @lt_enable_auto_import@ -version-info @version_info@ @release_info@

EXTRA_DIST = ecore_imf_private.h
