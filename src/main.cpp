#include <iostream>
#include <noisekernel/Signal.h>
#include "exception/NoiseStreamerException.h"
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
#include "runmode/AgentMode.h"
#include "runmode/AgentProtocol.h"
#include "runmode/ClientMode.h"

using namespace std;
using namespace NoiseKernel;

void run(LogService* logSrv, SignalAdapter* sigSrv, NoiseStreamerArgument* noiseStreamerArgs, NoiseStreamer* noiseStreamer);
void runStandalone(LogService* logSrv, NoiseStreamer* noiseStreamer);
void runInteractive(LogService* logSrv, SignalAdapter* sigSrv, NoiseStreamerArgument* noiseStreamerArgs, NoiseStreamer* noiseStreamer);
void runAgent(LogService* logSrv, SignalAdapter* sigSrv, NoiseStreamerArgument* noiseStreamerArgs, NoiseStreamer* noiseStreamer);
void runClient(LogService* logSrv, SignalAdapter* sigSrv, NoiseStreamerArgument* noiseStreamerArgs);

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

        // Run It!!
        run(&logger, &signalAdapter, &noiseStreamerArgs, &noiseStreamer);
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

void run(
    LogService* logSrv,
    SignalAdapter* sigSrv,
    NoiseStreamerArgument* noiseStreamerArgs,
    NoiseStreamer* noiseStreamer)
{
    try
    {
        RunMode runMode = noiseStreamerArgs->getRunMode();
        switch (runMode)
        {
        case STANDALONE:
            runStandalone(logSrv, noiseStreamer);
            break;

        case INTERACTIVE:
            runInteractive(logSrv, sigSrv, noiseStreamerArgs, noiseStreamer);
            break;

        case AGENT:
            runAgent(logSrv, sigSrv, noiseStreamerArgs, noiseStreamer);
            break;

        case CLIENT:
            runClient(logSrv, sigSrv, noiseStreamerArgs);
            break;

        default:
            logSrv->warn("No valid RunMode found. Try 'help'");
            break;
        }
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

void runStandalone(LogService* logSrv, NoiseStreamer* noiseStreamer)
{
    noiseStreamer->start();
}

void runInteractive(
    LogService* logSrv,
    SignalAdapter* sigSrv,
    NoiseStreamerArgument* noiseStreamerArgs,
    NoiseStreamer* noiseStreamer)
{
    if (noiseStreamerArgs->runOnBackground())
    {
        throw DomainException(GNR0003);
    }

    if (!noiseStreamerArgs->shouldLogToFile())
    {
        throw DomainException(GNR0004);
    }

    InteractiveMode interactive(logSrv, sigSrv, noiseStreamer);
    interactive.start();
}

void runAgent(
    LogService* logSrv,
    SignalAdapter* sigSrv,
    NoiseStreamerArgument* noiseStreamerArgs,
    NoiseStreamer* noiseStreamer)
{
    TcpServerConfig config(
        1,
        "NoiseStreamerAgent",
        "The NoiseStreamer Agent Server",
        TcpServerConfig::DEFAULT_HOSTNAME,
        TcpServerConfig::DEFAULT_PORT,
        TcpServerConfig::DEFAULT_THREAD_POOL_SIZE
    );

    AgentProtocol protocol(true);

    AgentMode agent(
        logSrv,
        sigSrv,
        &config,
        &protocol,
        noiseStreamer);

    agent.serve();
}

void runClient(
    LogService* logSrv,
    SignalAdapter* sigSrv,
    NoiseStreamerArgument* noiseStreamerArgs)
{
    TcpClientConfig config(
        1,
        "NoiseStreamerClient",
        "The NoiseStreamer Agent's Client",
        NoiseKernel::TcpClientConfig::DEFAULT_SERVERNAME,
        NoiseKernel::TcpClientConfig::DEFAULT_PORT
    );

    AgentProtocol protocol(false);

    ClientMode client(
        logSrv,
        sigSrv,
        &config,
        &protocol);

    client.action();
}
