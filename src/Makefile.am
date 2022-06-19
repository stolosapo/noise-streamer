AUTOMAKE_OPTIONS = subdir-objects
ACLOCAL_AMFLAGS = $(ACLOCAL_FLAGS)

bin_PROGRAMS = noisestreamer

mainincludedir = $(includedir)/noisestreamer
maininclude_HEADERS = \
    NoiseStreamer.h

common_sources = \
    NoiseStreamer.cpp

noisestreamer_SOURCES = main.cpp $(common_sources)
noisestreamer_LDADD = libnoisestreamer.la
noisestreamer_CXXFLAGS = $(AM_CXXFLAGS)

lib_LTLIBRARIES = libnoisestreamer.la
libnoisestreamer_la_LDFLAGS = -version-info 1:0:0 -pthread
libnoisestreamer_la_CXXFLAGS = $(AM_CXXFLAGS) -pthread
libnoisestreamer_la_SOURCES = $(common_sources)

TESTS = noisestreamer