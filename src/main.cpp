#include <iostream>
#include <noisekernel/Signal.h>
#include <noisekernel/Exception.h>
#include "banner/Banner.h"
#include "excecutor/Excecutor.h"
#include "logger/Logger.h"
#include "argument/NoiseStreamerArgument.h"
#include "argument/PlaylistAudioSourceArgument.h"
#include "NoiseStreamer.h"
#include "config/NoiseStreamerConfig.h"
#include "config/PlaylistAudioSourceConfig.h"
#include "health/NoiseStreamerHealthPolicy.h"
#include "audio/source/PlaylistAudioSource.h"

using namespace std;
using namespace NoiseKernel;

void buildAndRunNoiseStreamer(
    LogService* logSrv,
    SignalAdapter* sigAdapter,
    NoiseStreamerArgument* noiseStreamerArgs,
    PlaylistAudioSourceArgument* playlistAudioSourceArgs);

int main(int argc, char* argv[])
{
    try {
        Banner banner;
        cout << banner.getRandom() << endl << endl;

        // Parse Arguments
        ArgumentProvider provider(argc, argv);
        NoiseStreamerArgument noiseStreamerArgs(&provider);
        PlaylistAudioSourceArgument playlistAudioSourceArgs(&provider);
        noiseStreamerArgs.registerArguments();
        playlistAudioSourceArgs.registerArguments();

        if (noiseStreamerArgs.noArgs() || provider.hasArg("help"))
        {
            cout << noiseStreamerArgs.help() << endl;
            cout << playlistAudioSourceArgs.help() << endl;
            exit(0);
        }

        if (noiseStreamerArgs.runOnBackground())
        {
            Excecutor exec(noiseStreamerArgs.pidFile());
            // Start up new session, to lose old session and process group
            exec.forkAndExit();
        }

        // Setup Signals
        SignalAdapter signalAdapter;
        signalAdapter.registerSignals();
        signalAdapter.setup();

        // Setup Logger
        LogService logger = buildLogService();
        logLevel = noiseStreamerArgs.getLogLevel();
        shouldLogOnFile = noiseStreamerArgs.shouldLogToFile();
        logFile = noiseStreamerArgs.logFile();
        logger.info("LogLevel set to: " + convertLogLevelToString(logLevel));

        // Run
        buildAndRunNoiseStreamer(
            &logger,
            &signalAdapter,
            &noiseStreamerArgs,
            &playlistAudioSourceArgs);
    }
    catch (DomainException &e)
    {
        cerr << handle(e) << endl;
        exit(1);
    }
    catch (RuntimeException &e)
    {
        cerr << handle(e) << endl;
        exit(1);
    }
    catch (exception &e)
    {
        cerr << e.what() << endl;
        exit(1);
    }

    cout << "Bye Bye.." << endl;
}

void buildAndRunNoiseStreamer(
    LogService* logSrv,
    SignalAdapter* sigAdapter,
    NoiseStreamerArgument* noiseStreamerArgs,
    PlaylistAudioSourceArgument* playlistAudioSourceArgs)
{
    NoiseStreamerConfig noiseStremerConfig = buildNoiseStreamerConfig(noiseStreamerArgs);
    PlaylistAudioSourceConfig playlistAudioSourceConfig = buildPlaylistAudioSourceConfig(playlistAudioSourceArgs);
    NoiseStreamerHealthPolicy healthPolicy;

    PlaylistAudioSource* playlistAudioSource = new PlaylistAudioSource(
        logSrv,
        sigAdapter,
        &playlistAudioSourceConfig);

    try
    {
        NoiseStreamer streamer(
            logSrv,
            sigAdapter,
            &noiseStremerConfig,
            (AudioSource*) playlistAudioSource,
            &healthPolicy);

        streamer.initialize();
        streamer.connect();
        streamer.stream();
        streamer.disconnect();
        streamer.shutdown();
    }
    catch (DomainException &e)
    {
        logSrv->error(handle(e));
    }
    catch (RuntimeException &e)
    {
        logSrv->error(handle(e));
    }
    catch (exception &e)
    {
        throw e;
    }

    delete playlistAudioSource;
}
