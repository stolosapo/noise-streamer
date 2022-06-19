# noise-streamer
A remotely controlled client for the Icecast Streaming Server. Can stream mp3 playlists providing good control of what is playing. 

![Build](https://github.com/stolosapo/noise-streamer/actions/workflows/makefile.yml/badge.svg) ![GitHub release (latest by date)](https://img.shields.io/github/v/release/stolosapo/noise-streamer)


### Prepare compilation
```bash
./autogen.sh
```

### Installation
```bash
./configure
make
sudo make install
sudo ldconfig /usr/local/lib
```

### Uninstallation
```bash
sudo make uninstall
```

### Run
```bash
noisestreamer
```

## Clean all untracked files
```bash
git clean -xdf
```

### Use Library
If everything installed fine then pass `-lnoisestreamer` parameter in linker, when build your application.
Then use it in your app like in `examples/`.
