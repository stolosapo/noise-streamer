# <autotools-cpp-lib-template>
A Template Repository for C++ Libraries using Autotools

### TODO: Replace this across the project..
```
<repo-owner>                 = the GitHub owner
<owner-email>                = the owner contact email
<autotools-cpp-lib-template> = the current repository name
<autotoolscpplibtemplate>    = the library name, to be installed
```

![Build](https://github.com/<repo-owner>/<autotools-cpp-lib-template>/actions/workflows/makefile.yml/badge.svg) ![GitHub release (latest by date)](https://img.shields.io/github/v/release/<repo-owner>/<autotools-cpp-lib-template>)


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
<autotoolscpplibtemplate>
```

## Clean all untracked files
```bash
git clean -xdf
```

### Use Library
If everything installed fine then pass `-l<autotoolscpplibtemplate>` parameter in linker, when build your application.
Then use it in your app like in `examples/`.
