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
#include "runmode/InteractiveMode.h"

using namespace std;
using namespace NoiseKernel;

void runStandalone(LogService* logSrv, NoiseStreamer* noiseStreamer);
void runInteractive(LogService* logSrv, SignalAdapter* sigSrv, NoiseStreamer* noiseStreamer);

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

        // Build configs
        NoiseStreamerConfig noiseStremerConfig = buildNoiseStreamerConfig(&noiseStreamerArgs);
        PlaylistAudioSourceConfig playlistAudioSourceConfig = buildPlaylistAudioSourceConfig(&playlistAudioSourceArgs);
        NoiseStreamerHealthPolicy healthPolicy;

        PlaylistAudioSource* playlistAudioSource = new PlaylistAudioSource(
            &logger,
            &signalAdapter,
            &playlistAudioSourceConfig);

        // Build NoiseStreamer
        NoiseStreamer noiseStreamer(
            &logger,
            &signalAdapter,
            &noiseStremerConfig,
            (AudioSource*) playlistAudioSource,
            &healthPolicy);

        // Run Standalone
        runStandalone(&logger, &noiseStreamer);

        // Run Interactive
        // TODO: should now allowed to run on background
        // runInteractive(&logger, &signalAdapter, &noiseStreamer);
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

void runStandalone(LogService* logSrv, NoiseStreamer* noiseStreamer)
{
    try
    {
        noiseStreamer->initialize();
        noiseStreamer->connect();
        noiseStreamer->stream();
        noiseStreamer->disconnect();
        noiseStreamer->shutdown();
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
}

void runInteractive(
    LogService* logSrv,
    SignalAdapter* sigSrv,
    NoiseStreamer* noiseStreamer)
{
    try
    {
        InteractiveMode interactive(logSrv, sigSrv, noiseStreamer);
        interactive.start();
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
}
