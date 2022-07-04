#include <iostream>
#include "argument/NoiseStreamerArgument.h"
#include "argument/PlaylistAudioSourceArgument.h"

using namespace std;

int main(int argc, char* argv[])
{
    cout << "noisestreamer Library" << endl << endl;

    ArgumentProvider provider(argc, argv);
    NoiseStreamerArgument noiseStreamerArgs(&provider);
    noiseStreamerArgs.registerArguments();

    PlaylistAudioSourceArgument playlistAudioSourceArgs(&provider);
    playlistAudioSourceArgs.registerArguments();

    cout << noiseStreamerArgs.help() << endl << endl;
    cout << playlistAudioSourceArgs.help() << endl;

    cout << "Bye Bye.." << endl;
}
