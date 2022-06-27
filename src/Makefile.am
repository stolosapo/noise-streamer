AUTOMAKE_OPTIONS = subdir-objects
ACLOCAL_AMFLAGS = $(ACLOCAL_FLAGS)

bin_PROGRAMS = noisestreamer

mainincludedir = $(includedir)/noisestreamer
maininclude_HEADERS = \
    NoiseStreamer.h \
    exception/NoiseStreamerException.h \
    config/NoiseStreamerConfig.h \
    audio/AudioBitrate.h \
    audio/AudioEncodeMode.h \
    audio/AudioSourceType.h \
    audio/AudioSource.h \
    audio/tag/AudioTag.h \


common_sources = \
    NoiseStreamer.cpp \
    \
    logger/Logger.h logger/Logger.cpp \
    \
    exception/NoiseStreamerException.cpp \
    \
    audio/AudioBitrate.cpp \
    audio/AudioSource.cpp \
    audio/tag/AudioTag.cpp \
    audio/tag/AudioTagReader.h audio/tag/AudioTagReader.cpp \
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
    libshout/LibShoutProtocol.cpp \
    \
    utils/StringHelper.h utils/StringHelper.cpp \
    utils/FileHelper.h utils/FileHelper.cpp \
    utils/File.h utils/File.cpp

noisestreamer_SOURCES = main.cpp $(common_sources)
noisestreamer_LDADD = libnoisestreamer.la
noisestreamer_CXXFLAGS = $(AM_CXXFLAGS) -lshout -lnoisekernel

lib_LTLIBRARIES = libnoisestreamer.la
libnoisestreamer_la_LDFLAGS = -version-info 1:0:0 -pthread -lshout -lnoisekernel
libnoisestreamer_la_CXXFLAGS = $(AM_CXXFLAGS) -pthread -lshout -lnoisekernel
libnoisestreamer_la_SOURCES = $(common_sources)

TESTS = noisestreamer
