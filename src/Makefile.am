AUTOMAKE_OPTIONS = subdir-objects
ACLOCAL_AMFLAGS = $(ACLOCAL_FLAGS)

bin_PROGRAMS = noisestreamer

mainincludedir = $(includedir)/noisestreamer
maininclude_HEADERS = \
    NoiseStreamer.h \
    exception/NoiseStreamerException.h \
    config/NoiseStreamerConfig.h \
    config/PlaylistAudioSourceConfig.h \
    audio/AudioBitrate.h \
    audio/AudioEncodeMode.h \
    audio/source/AudioSourceType.h \
    audio/source/AudioSource.h \
    audio/tag/AudioTag.h \
    audio/playlist/PlaylistStrategyType.h


common_sources = \
    NoiseStreamer.cpp \
    NoiseStreamerTaskRunner.h NoiseStreamerTaskRunner.cpp \
    \
    runmode/RunMode.h runmode/RunMode.cpp \
    runmode/InteractiveMode.h runmode/InteractiveMode.cpp \
    runmode/AgentMode.h runmode/AgentMode.cpp \
    runmode/AgentModeTask.h runmode/AgentModeTask.cpp \
    runmode/ClientMode.h runmode/ClientMode.cpp \
    \
    excecutor/Excecutor.h excecutor/Excecutor.cpp \
    banner/Banner.h banner/Banner.cpp \
    \
    config/NoiseStreamerConfig.cpp \
    config/PlaylistAudioSourceConfig.cpp \
    \
    logger/Logger.h logger/Logger.cpp \
    \
    exception/NoiseStreamerException.cpp \
    \
    audio/AudioBitrate.cpp \
    audio/source/AudioMetadataChangedEventArgs.h audio/source/AudioMetadataChangedEventArgs.cpp \
    audio/source/AudioSource.cpp \
    audio/source/PlaylistAudioSource.h audio/source/PlaylistAudioSource.cpp \
    audio/source/PlaylistAudioSourceItem.h audio/source/PlaylistAudioSourceItem.cpp \
    audio/source/PlaylistAudioSourceItemContext.h \
    audio/source/PlaylistAudioSourceNavigator.h audio/source/PlaylistAudioSourceNavigator.cpp \
    audio/tag/AudioTag.cpp \
    audio/tag/AudioTagReader.h audio/tag/AudioTagReader.cpp \
    audio/playlist/PlaylistStrategyType.cpp \
    audio/playlist/PlaylistHandler.h audio/playlist/PlaylistHandler.cpp \
    audio/playlist/PlaylistItem.h audio/playlist/PlaylistItem.cpp \
    audio/playlist/PlaylistStrategy.h audio/playlist/PlaylistStrategy.cpp \
    audio/playlist/PlaylistStrategyFactory.h audio/playlist/PlaylistStrategyFactory.cpp \
    audio/playlist/RandomOncePlaylistStrategy.h audio/playlist/RandomOncePlaylistStrategy.cpp \
    audio/playlist/SimplePlaylistStrategy.h audio/playlist/SimplePlaylistStrategy.cpp \
    \
    audio/encode/liblame/LibLame.h audio/encode/liblame/LibLame.cpp \
    audio/encode/NoiseStreamerEncoder.h audio/encode/NoiseStreamerEncoder.cpp \
    audio/encode/EncodeContext.h \
    \
    health/NoiseStreamerHealthPolicy.h health/NoiseStreamerHealthPolicy.cpp \
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
    argument/NoiseStreamerArgument.h argument/NoiseStreamerArgument.cpp \
    argument/PlaylistAudioSourceArgument.h argument/PlaylistAudioSourceArgument.cpp \
    \
    utils/StringHelper.h utils/StringHelper.cpp \
    utils/FileHelper.h utils/FileHelper.cpp \
    utils/File.h utils/File.cpp \
    utils/SynchronizedQueue.h \
    utils/SynchronizedPQueue.h

noisestreamer_SOURCES = main.cpp $(common_sources)
noisestreamer_LDADD = libnoisestreamer.la
noisestreamer_CXXFLAGS = $(AM_CXXFLAGS) -lshout -lnoisekernel -lmp3lame -DHAVE_LAME

lib_LTLIBRARIES = libnoisestreamer.la
libnoisestreamer_la_LDFLAGS = -version-info 1:0:0 -pthread -lshout -lnoisekernel
libnoisestreamer_la_CXXFLAGS = $(AM_CXXFLAGS) -pthread -lshout -lnoisekernel -lmp3lame -DHAVE_LAME
libnoisestreamer_la_SOURCES = $(common_sources)

TESTS = noisestreamer
