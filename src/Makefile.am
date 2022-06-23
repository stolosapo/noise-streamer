AUTOMAKE_OPTIONS = subdir-objects
ACLOCAL_AMFLAGS = $(ACLOCAL_FLAGS)

bin_PROGRAMS = noisestreamer

mainincludedir = $(includedir)/noisestreamer
maininclude_HEADERS = \
    NoiseStreamer.h

common_sources = \
    NoiseStreamer.cpp \
    \
    libshout/LibShout.h libshout/LibShout.cpp \
    libshout/LibShoutAudio.cpp \
    libshout/LibShoutCommunication.cpp \
    libshout/LibShoutConnection.cpp \
    libshout/LibShoutError.cpp \
    libshout/LibShoutFormat.cpp \
    libshout/LibShoutGeneralProperties.cpp \
    libshout/LibShoutInitialization.cpp \
    libshout/LibShoutMetadata.cpp \
    libshout/LibShoutProtocol.cpp

noisestreamer_SOURCES = main.cpp $(common_sources)
noisestreamer_LDADD = libnoisestreamer.la
noisestreamer_CXXFLAGS = $(AM_CXXFLAGS) -lshout

lib_LTLIBRARIES = libnoisestreamer.la
libnoisestreamer_la_LDFLAGS = -version-info 1:0:0 -pthread -lshout
libnoisestreamer_la_CXXFLAGS = $(AM_CXXFLAGS) -pthread -lshout
libnoisestreamer_la_SOURCES = $(common_sources)

TESTS = noisestreamer
