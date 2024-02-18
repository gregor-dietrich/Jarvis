# Jarvis

Learning project - not recommended for use in production environments under any circumstances, absolutely no warranty.

## Dependencies

### Compiling

The following libraries are required to build the project (all platforms):

- [Boost](https://www.boost.org) (recommended: latest stable version - tested: [1.84.0](https://www.boost.org/users/history/version_1_84_0.html))
- [OpenSSL](https://www.openssl.org) (recommended: latest stable version - tested: [3.2.1](https://slproweb.com/products/Win32OpenSSL.html))

### Runtime

The following libraries are required at runtime by the executable:

- Windows: libcrypto-3-x64.dll & libssl-3-x64.dll
- Linux: libcrypto.so & libssl.so
- macOS: libcrypto.dylib & libssl.dylib

This means they must either reside in the same directory as the executable or be located within a directory specified in your PATH environment variable.

## Building

### Windows

- Visual Studio 2022
- Toolset v143,  C++ 20
- Set "-logLevel 4" in Debugging arguments
- Adjust dependency paths (see "C/C++ -> General" and "Linker -> General")

You can (probably?) also use CMake (with or without VSCode+extensions) instead.

### Linux

- gcc-12 won't work, use gcc-11 instead
- tested with gcc-11.3.0-12 on a Debian 12 VM:

```
mount -t vboxsf -o uid=1000,gid=1000 Jarvis /home/<username>/workspace/Jarvis 
cd /home/<username>/workspace/Jarvis && mkdir build && cd build && cmake .. && make 
mv ./Jarvis ../Jarvis && cd .. && ./Jarvis -logLevel 4 -port 8080 && rm ./Jarvis && rm -rf build
```

### macOS

- Generally the same as building on Linux (see above)
- recommended: use VSCode with CMake extensions
- recommended: install CMake, gcc/clang etc. using [Homebrew](https://brew.sh)
