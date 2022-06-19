AUTOMAKE_OPTIONS = subdir-objects
ACLOCAL_AMFLAGS = $(ACLOCAL_FLAGS)

bin_PROGRAMS = <autotoolscpplibtemplate>

mainincludedir = $(includedir)/<autotoolscpplibtemplate>
maininclude_HEADERS = \
    all_header_files.h

common_sources = \
    all_source_files.cpp

<autotoolscpplibtemplate>_SOURCES = main.cpp $(common_sources)
<autotoolscpplibtemplate>_LDADD = lib<autotoolscpplibtemplate>.la
<autotoolscpplibtemplate>_CXXFLAGS = $(AM_CXXFLAGS)

lib_LTLIBRARIES = lib<autotoolscpplibtemplate>.la
lib<autotoolscpplibtemplate>_la_LDFLAGS = -version-info 1:0:0 -pthread
lib<autotoolscpplibtemplate>_la_CXXFLAGS = $(AM_CXXFLAGS) -pthread
lib<autotoolscpplibtemplate>_la_SOURCES = $(common_sources)

TESTS = <autotoolscpplibtemplate>