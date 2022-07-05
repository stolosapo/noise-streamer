#include <iostream>
#include <noisekernel/Signal.h>
#include <noisekernel/Exception.h>
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
    cout << "noisestreamer" << endl << endl;

    try {
        // Parse Arguments
        ArgumentProvider provider(argc, argv);
        NoiseStreamerArgument noiseStreamerArgs(&provider);
        PlaylistAudioSourceArgument playlistAudioSourceArgs(&provider);
        noiseStreamerArgs.registerArguments();
        playlistAudioSourceArgs.registerArguments();

        cout << noiseStreamerArgs.help() << endl;
        cout << playlistAudioSourceArgs.help() << endl;

        // Setup Signals
        SignalAdapter signalAdapter;
        signalAdapter.registerSignals();
        signalAdapter.setup();

        // Setup Logger
        LogService logger = buildLogService();

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

    delete playlistAudioSource;
}
