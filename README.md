# Jarvis

## Building

Boost is required on all platforms.

### Windows

- Visual Studio 2022
- Toolset v143,  C++ 20
- Set "-logLevel 3" in Debugging arguments
- Adjust Boost path

You can (probably?) also use CMake (with or without VSCode+extensions) instead.

### Linux

- gcc-12 won't work, use gcc-11 instead
- tested with gcc-11.3.0-12 on a Debian 12 VM:

```
mount -t vboxsf -o uid=1000,gid=1000 Jarvis /home/<username>/workspace/Jarvis 
cd /home/<username>/workspace/Jarvis && mkdir build && cd build && cmake .. && make 
mv ./Jarvis ../Jarvis && cd .. && ./Jarvis -logLevel 3 -port 8080 && rm ./Jarvis && rm -rf build
```

### macOS

- recommended: install CMake, gcc/clang etc. using [Homebrew](https://brew.sh)
- recommended: use VSCode with CMake extensions