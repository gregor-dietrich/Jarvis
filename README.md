# Jarvis

Learning project - not recommended for use in production environments under any circumstances, absolutely no warranty.

## Dependencies

### Compiling

The following libraries are required to build the project (all platforms):

- [Boost](https://www.boost.org) 1.83.0
- [OpenSSL](https://www.openssl.org) 3.0.2

### Runtime

The following OpenSSL libraries are required at runtime by the executable:

- Windows: libcrypto-3-x64.dll & libssl-3-x64.dll
- Linux: libcrypto.so & libssl.so
- macOS: libcrypto.dylib & libssl.dylib

This means they must either reside in the same directory as the executable or be located within a directory specified in your PATH environment variable.

## Building

### Windows

- Visual Studio 2022, Toolset v143, C++20
- Set "-logLevel 4" in Debugging arguments
- Adjust dependency paths (see "C/C++ -> General" and "Linker -> General")

### Unix

- Use CMake (oldest version tested: 3.22.1)
- gcc-12 seemed to have some issues for me, use gcc-11 instead (tested with gcc-11.2.0 and gcc-11.3.0)

```
apt install gcc cmake openssl libssl-dev
mount -t vboxsf -o uid=1000,gid=1000 Jarvis ~/workspace/Jarvis 
cd ~/workspace/Jarvis && mkdir build && cd build && cmake .. && make && cd ..
mv build/Jarvis ./Jarvis && ./Jarvis -logLevel 4 -port 8080 && mv ./Jarvis build/Jarvis
```