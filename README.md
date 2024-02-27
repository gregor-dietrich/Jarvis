# Jarvis

Learning project - not recommended for use in production environments under any circumstances, absolutely no warranty.

## Dependencies

### Compiling

The following libraries are required to build the project (all platforms):

- [Boost](https://www.boost.org) (min. 1.83.0)
- [OpenSSL](https://www.openssl.org) (min. 3.0.2)
- [Mustache](https://github.com/no1msd/mstch)

### Runtime

The following OpenSSL libraries are required at runtime by the executable:

- Windows: libcrypto-3-x64.dll & libssl-3-x64.dll
- Linux: libcrypto.so & libssl.so
- macOS: libcrypto.dylib & libssl.dylib

This means they must either reside in the same directory as the executable or be located within a directory specified in your PATH environment variable.

## Building

On any OS, I recommend to first clone [Mustache](https://github.com/no1msd/mstch) inside the lib subdirectory:

```sh
mkdir lib
cd lib
git clone https://github.com/no1msd/mstch.git
```

### Windows

- Visual Studio 2022, Toolset v143, C++20
- Set "-logLevel 4" in Debugging arguments
- Adjust dependency paths (see "C/C++ -> General" and "Linker -> General")
  - Alternatively, you can create SymLinks for Boost and OpenSSL in the lib subdirectory:

```cmd
cd lib
mklink /j boost C:\Source\boost_1_84_0
mklink /j openssl C:\Source\OpenSSL
```

### Unix

#### Preparing

```sh
sudo apt update && sudo apt install build-essential cmake git libssl-dev openssl tar wget
wget https://boostorg.jfrog.io/artifactory/main/release/1.84.0/source/boost_1_84_0.tar.bz2
tar --bzip2 -xf boost_1_84_0.tar.bz2 && rm boost_1_84_0.tar.bz2
sudo mv boost_1_84_0 /usr/local/boost_1_84_0
mkdir ~/workspace && cd ~/workspace
git clone https://github.com/gregor-dietrich/Jarvis && cd Jarvis && mkdir build
```

- g++-12 seems to have some issues with Boost, so I used g++-11 instead:

```sh
sudo apt install g++-11
sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-11 60
g++ --version
```

#### Building

```sh
cd build && cmake .. && make && cd ..
```

#### Running

```sh
mv build/Jarvis ./Jarvis && ./Jarvis -logLevel 4 -port 8080 && mv ./Jarvis build/Jarvis
```
